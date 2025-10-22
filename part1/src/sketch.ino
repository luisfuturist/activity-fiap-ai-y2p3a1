/*
  CardioIA – Fase IoT (Parte 1): Edge Computing com ESP32 + SPIFFS
  Objetivo:
    - Coletar sinais vitais simulados: temperatura/umidade (DHT22) e batimentos (botão).
    - Armazenar localmente em SPIFFS (resiliência offline).
    - Ao "voltar online", sincronizar tudo para a "nuvem" (simulada via Serial) e limpar buffer.

  Hardware (simulado no Wokwi):
    - ESP32 DevKit C v4
    - DHT22 no GPIO 15 (DHT_PIN)
    - Botão no GPIO 32 (BUTTON_PIN) com INPUT_PULLUP (pressionado = LOW)

  Timers e janelas:
    - Leitura DHT a cada 2000 ms (2s)
    - Cálculo de BPM a cada 10000 ms (10s) contando pulsos do botão
    - Alternância do Wi-Fi simulado a cada 30000 ms (30s)

  Armazenamento (SPIFFS):
    - /sensor_data.json  -> arquivo texto com UMA linha JSON por amostra
    - /sample_count.txt  -> persiste quantidade de amostras acumuladas
    - Limite: 100 amostras (estratégia FIFO usando "circular buffer")

  Fluxo de conectividade (simulado com isConnected):
    - isConnected = false  -> coleta e grava localmente (Edge first)
    - isConnected = true   -> envia amostras (Serial como "cloud") e remove do SPIFFS (flush)

  Como validar rapidamente:
    1) Rodar com isConnected=false (ou aguardar o toggle) e ver acumular amostras no Serial.
    2) Quando isConnected=true, observar "Sending..." e, ao final, "Synced ... data points" e arquivo limpo.
*/

#include <DHT.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>

// --------- Mapa de pinos e tipos de sensor ---------
#define DHT_PIN 15
#define DHT_TYPE DHT22
#define BUTTON_PIN 32

DHT dht(DHT_PIN, DHT_TYPE);

// --------- Controle de temporização (leituras/cálculos) ---------
unsigned long lastDhtRead = 0;
const unsigned long dhtInterval = 2000;          // Janela de leitura do DHT (2s)

unsigned long lastBeatCalc = 0;
const unsigned long heartbeatInterval = 10000;   // Janela de cálculo de BPM (10s)

// --------- Contagem de batimentos (por botão) e BPM ---------
int beatCount = 0;   // pulsos somados na janela
int bpm = 0;         // BPM calculado ao final da janela

// --------- Debounce do botão (estabilidade das leituras) ---------
int lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;          // 50ms de debounce

// --------- Simulação de Wi-Fi (conectividade) ---------
bool isConnected = false;                        // Estado atual (simulado)
unsigned long lastWifiToggle = 0;
const unsigned long wifiToggleInterval = 30000;  // Alterna a cada 30s para simulação

// --------- Armazenamento (SPIFFS) ---------
const int MAX_STORED_SAMPLES = 100;              // Limite de amostras (FIFO/circular buffer)
int currentSampleCount = 0;                      // Contador persistido em arquivo
const char* DATA_FILE = "/sensor_data.json";     // Um JSON por linha (append)


// ------------------------------------------------------------------
// setup(): inicializa Serial, sensores e SPIFFS; carrega contador e
//          inicia no modo "desconectado" para demonstrar resiliência.
// ------------------------------------------------------------------
void setup() {
  Serial.begin(115200);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  dht.begin();

  // Monta o sistema de arquivos SPIFFS (true = formata se necessário)
  if (!SPIFFS.begin(true)) {
    Serial.println("❌ SPIFFS Mount Failed");
    return;
  }
  Serial.println("✅ SPIFFS Mounted Successfully");

  // Começa em modo "offline" (simulação)
  isConnected = false;
  Serial.println("📡 WiFi Status: DISCONNECTED (Simulation Mode)");

  // Recupera quantas amostras já estavam acumuladas (após reset)
  loadSampleCount();
}


// ------------------------------------------------------------------
// loop(): laço principal que
//   1) alterna estado do Wi-Fi simulado e dispara sync quando conecta;
//   2) trata o botão com debounce e acumula batidas na janela;
//   3) calcula BPM a cada janela definida;
//   4) lê DHT periodicamente e persiste/enfileira a amostra.
// ------------------------------------------------------------------
void loop() {
  unsigned long now = millis();

  // --- 1) Simula alternância de Wi-Fi (online/offline) ---
  if (now - lastWifiToggle >= wifiToggleInterval) {
    isConnected = !isConnected;
    lastWifiToggle = now;
    Serial.print("📡 WiFi Status: ");
    Serial.println(isConnected ? "CONNECTED" : "DISCONNECTED");

    // Se acabamos de conectar, sincroniza tudo que estava armazenado
    if (isConnected) {
      syncStoredData();
    }
  }

  // --- 2) Leitura do botão com debounce para contar "batimentos" ---
  int reading = digitalRead(BUTTON_PIN);
  if (reading != lastButtonState) {
    // Bordas provocam reinício do cronômetro de debounce
    lastDebounceTime = now;
  }

  if ((now - lastDebounceTime) > debounceDelay) {
    static int buttonState = HIGH;
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == LOW) { // Pressionado (puxado para GND)
        beatCount++;
        Serial.println("❤️ Heartbeat detected");
      }
    }
  }
  lastButtonState = reading;

  // --- 3) Cálculo de BPM ao final de cada janela (heartbeatInterval) ---
  if (now - lastBeatCalc >= heartbeatInterval) {
    // bpm = pulsos_na_janela * (60s / janela_em_ms)
    bpm = (beatCount * 60000UL) / heartbeatInterval;
    beatCount = 0;
    lastBeatCalc = now;
    Serial.print("BPM: ");
    Serial.println(bpm);
  }

  // --- 4) Leitura periódica do DHT22 e persistência da amostra ---
  if (now - lastDhtRead >= dhtInterval) {
    lastDhtRead = now;

    float temp = dht.readTemperature(); // °C
    float hum  = dht.readHumidity();    // %

    if (isnan(temp) || isnan(hum)) {
      // Leitura inválida (sensor pode falhar eventualmente)
      Serial.println("⚠️ Failed to read from DHT sensor!");
    } else {
      Serial.print("Temp: ");
      Serial.print(temp);
      Serial.print(" °C, Hum: ");
      Serial.print(hum);
      Serial.println(" %");

      // Persiste a amostra (sempre grava localmente para garantir resiliência)
      storeSensorData(temp, hum, bpm);
    }
  }
}


// ==================================================================
// ====================  ROTINAS DE PERSISTÊNCIA  ====================
// ==================================================================

/*
  storeSensorData():
    - Cria um JSON com timestamp relativo (millis) + métricas coletadas.
    - Sempre grava localmente (Edge first) em /sensor_data.json (append).
    - Se atingir MAX_STORED_SAMPLES, aplica estratégia FIFO (circular).
    - Se estiver conectado, também tenta enviar imediatamente.
*/
void storeSensorData(float temperature, float humidity, int heartRate) {
  // Monta o payload JSON (um por linha no arquivo)
  DynamicJsonDocument doc(1024);
  doc["timestamp"]  = millis();
  doc["temperature"] = temperature;
  doc["humidity"]    = humidity;
  doc["heartRate"]   = heartRate;
  doc["wifiStatus"]  = isConnected ? "connected" : "disconnected";

  String jsonString;
  serializeJson(doc, jsonString);

  // Gravação local (Edge first): sempre tentamos armazenar
  if (currentSampleCount < MAX_STORED_SAMPLES) {
    File file = SPIFFS.open(DATA_FILE, "a"); // append preserva histórico
    if (file) {
      file.println(jsonString);
      file.close();
      currentSampleCount++;
      saveSampleCount(); // persiste o contador (sobrevive a resets)
      Serial.println("💾 Data stored locally (Edge Computing)");
    } else {
      Serial.println("❌ Failed to open file for writing");
    }
  } else {
    // Limite atingido: aplica FIFO removendo a linha mais antiga
    Serial.println("⚠️ Storage limit reached - implementing circular buffer");
    implementCircularBuffer(jsonString);
    // Mantém contador consistente no limite
    currentSampleCount = MAX_STORED_SAMPLES;
    saveSampleCount();
  }

  // Se online, tenta enviar imediatamente (nesta fase via Serial)
  if (isConnected) {
    sendDataToCloud(jsonString);
  }
}

/*
  implementCircularBuffer():
    - Lê todo o arquivo de dados.
    - Remove a primeira linha (amostra mais antiga).
    - Acrescenta a nova amostra ao final.
    - Regrava o arquivo inteiro (O(n) — suficiente para 100 linhas).
    - Observação: para volumes maiores, prefira rotação de arquivos.
*/
void implementCircularBuffer(String newData) {
  // Lê conteúdo atual
  String allData = "";
  File file = SPIFFS.open(DATA_FILE, "r");
  if (file) {
    allData = file.readString();
    file.close();
  }

  // Remove a primeira linha (mais antiga)
  int firstNewline = allData.indexOf('\n');
  if (firstNewline != -1) {
    allData = allData.substring(firstNewline + 1);
  }

  // Acrescenta a nova amostra ao final
  allData += newData + "\n";

  // Regrava o arquivo com as linhas já rotacionadas
  file = SPIFFS.open(DATA_FILE, "w");
  if (file) {
    file.print(allData);
    file.close();
    Serial.println("🔄 Circular buffer updated");
  } else {
    Serial.println("❌ Failed to rewrite data file");
  }
}

/*
  syncStoredData():
    - Ao conectar, varre /sensor_data.json linha a linha.
    - "Envia" cada linha para a nuvem (nesta fase: Serial.println).
    - Ao final, remove o arquivo e reseta o contador.
    - Pequeno delay entre envios para simular latência.
*/
void syncStoredData() {
  Serial.println("🔄 Syncing stored data to cloud...");

  File file = SPIFFS.open(DATA_FILE, "r");
  if (!file) {
    Serial.println("❌ No stored data to sync");
    return;
  }

  int syncedCount = 0;
  while (file.available()) {
    String line = file.readStringUntil('\n');
    line.trim();
    if (line.length() > 0) {
      sendDataToCloud(line); // nesta fase, apenas imprime no Serial
      syncedCount++;
      delay(100);            // pequena pausa para simular rede/processamento
    }
  }
  file.close();

  // Limpa o arquivo após sincronização bem-sucedida
  if (syncedCount > 0) {
    SPIFFS.remove(DATA_FILE);
    currentSampleCount = 0;
    saveSampleCount();
    Serial.print("✅ Synced ");
    Serial.print(syncedCount);
    Serial.println(" data points to cloud");
  }
}

/*
  sendDataToCloud():
    - Simula o envio para a nuvem imprimindo o JSON no Serial.
    - Na Parte 2, substituir por publicação MQTT (broker cloud).
*/
void sendDataToCloud(String jsonData) {
  Serial.println("☁️ Sending to cloud: " + jsonData);

  // Implementação real (Parte 2):
  //  - MQTT publish (ex.: HiveMQ Cloud)
  //  - HTTP POST / WebSocket, etc.
}

/*
  loadSampleCount():
    - Lê de /sample_count.txt o número de amostras armazenadas.
    - Ajuda a manter consistência após resets de energia/reboot.
*/
void loadSampleCount() {
  File file = SPIFFS.open("/sample_count.txt", "r");
  if (file) {
    currentSampleCount = file.parseInt();
    file.close();
    Serial.print("📊 Loaded sample count: ");
    Serial.println(currentSampleCount);
  } else {
    currentSampleCount = 0;
    Serial.println("📊 Starting with fresh sample count");
  }
}

/*
  saveSampleCount():
    - Persiste o contador de amostras em /sample_count.txt.
    - Usado após cada append/flush para refletir o estado atual.
*/
void saveSampleCount() {
  File file = SPIFFS.open("/sample_count.txt", "w");
  if (file) {
    file.println(currentSampleCount);
    file.close();
  } else {
    Serial.println("❌ Failed to persist sample count");
  }
}
