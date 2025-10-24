/*
  CardioIA – Fase IoT (Parte 1): Edge Computing com ESP32 + SD Card
  =================================================================
  
  OBJETIVO:
    Sistema de monitoramento cardíaco com resiliência offline que:
    - Coleta sinais vitais simulados: temperatura/umidade (DHT22) e batimentos (botão)
    - Armazena localmente em SD Card (resiliência offline)
    - Sincroniza automaticamente quando conecta (Edge Computing)

  HARDWARE (simulado no Wokwi):
    - ESP32 DevKit C v4
    - DHT22 no GPIO 15 (temperatura e umidade)
    - Botão no GPIO 32 (simulador de batimentos cardíacos)
    - SD Card (CS=5, MOSI=23, MISO=19, SCK=18)

  TIMERS E JANELAS:
    - Leitura DHT: 2000ms (2s) - temperatura e umidade
    - Cálculo BPM: 10000ms (10s) - conta pulsos do botão
    - Alternância WiFi: 30000ms (30s) - simula conectividade

  ARMAZENAMENTO (SD Card):
    - /sensor_data.json  -> JSON por linha (append mode)
    - /sample_count.txt  -> contador persistido
    - Limite: 100 amostras (buffer circular FIFO)

  FLUXO DE CONECTIVIDADE:
    - OFFLINE (isConnected=false): coleta e armazena localmente
    - ONLINE (isConnected=true): envia direto + sincroniza dados acumulados

  COMO TESTAR:
    1) Aguarde modo offline e veja acúmulo: "💾 Stored locally (X/100)"
    2) Pressione botão para simular batimentos: "❤️ Beat: X"
    3) Aguarde conexão WiFi: "📡 WiFi: CONNECTED"
    4) Observe sincronização: "🔄 Syncing X samples..."
    5) Confirme limpeza: "✅ Synced X samples - local storage cleared"


*/

#include <DHT.h>
#include <SD.h>
#include <SPI.h>
#include <ArduinoJson.h>

// --------- Mapa de pinos e tipos de sensor ---------
#define DHT_PIN 15
#define DHT_TYPE DHT22
#define BUTTON_PIN 32

// --------- Configuração de pinos do SD Card ---------
#define SD_CS 5
#define SD_MOSI 23
#define SD_MISO 19
#define SD_SCK 18

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

// --------- Status do SD Card ---------
bool sdCardAvailable = false;                    // Flag para indicar se SD card está disponível

// --------- Armazenamento (SD Card) ---------
const int MAX_STORED_SAMPLES = 100;              // Limite de amostras (FIFO/circular buffer)
int currentSampleCount = 0;                      // Contador persistido em arquivo
const char* DATA_FILE = "/sensor_data.json";     // Um JSON por linha (append)


// ------------------------------------------------------------------
// setup(): inicializa Serial, sensores e SD Card; carrega contador e
//          inicia no modo "desconectado" para demonstrar resiliência.
// ------------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  delay(100);
  
  Serial.println("🚀 CardioIA System Starting...");
  
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  dht.begin();
  SPI.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
  
  if (!SD.begin(SD_CS)) {
    Serial.println("❌ SD Card Failed - Simulation Mode");
    sdCardAvailable = false;
  } else {
    Serial.println("✅ SD Card Ready");
    sdCardAvailable = true;
  }

  isConnected = false;
  Serial.println("📡 WiFi: DISCONNECTED");
  
  loadSampleCount();
  
  lastDhtRead = 0;
  lastBeatCalc = 0;
  lastWifiToggle = 0;
  beatCount = 0;
  bpm = 0;
  
  Serial.println("✅ System Ready - DHT: 2s, BPM: 10s, WiFi: 30s");
  Serial.println("=========================================");
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
  static unsigned long lastDebugTime = 0;
  const unsigned long debugInterval = 10000; // Status a cada 10 segundos

  // --- 1) Simula alternância de Wi-Fi (online/offline) ---
  if (now - lastWifiToggle >= wifiToggleInterval) {
    isConnected = !isConnected;
    lastWifiToggle = now;
    Serial.println("📡 WiFi: " + String(isConnected ? "CONNECTED" : "DISCONNECTED"));

    if (isConnected) {
      syncStoredData();
    }
  }

  // --- 2) Leitura do botão com debounce para contar "batimentos" ---
  int reading = digitalRead(BUTTON_PIN);
  
  if (reading != lastButtonState) {
    lastDebounceTime = now;
  }

  if ((now - lastDebounceTime) > debounceDelay) {
    static int buttonState = HIGH;
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == LOW) { // Pressionado (puxado para GND)
        beatCount++;
        Serial.println("❤️ Beat: " + String(beatCount) + " (" + String((now - lastBeatCalc)/1000) + "s)");
      }
    }
  }
  lastButtonState = reading;

  // --- 3) Cálculo de BPM ao final de cada janela (heartbeatInterval) ---
  if (now - lastBeatCalc >= heartbeatInterval) {
    bpm = (beatCount * 60000UL) / heartbeatInterval;
    beatCount = 0;
    lastBeatCalc = now;
    Serial.println("💓 BPM: " + String(bpm));
  }

  // --- 4) Leitura periódica do DHT22 e persistência da amostra ---
  if (now - lastDhtRead >= dhtInterval) {
    lastDhtRead = now;

    float temp = dht.readTemperature(); // °C
    float hum  = dht.readHumidity();    // %

    if (isnan(temp) || isnan(hum)) {
      Serial.println("⚠️ DHT Read Failed");
    } else {
      Serial.println("🌡️ Measured: " + String(temp, 1) + "°C " + String(hum, 1) + "% BPM:" + String(bpm));
      storeSensorData(temp, hum, bpm);
    }
  }
  
  // Status periódico
  if (now - lastDebugTime >= debugInterval) {
    Serial.println("📊 Status: WiFi: " + String(isConnected ? "ON" : "OFF") + " Samples:" + String(currentSampleCount) + "/" + String(MAX_STORED_SAMPLES));
    lastDebugTime = now;
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

  // Se conectado, envia direto para a nuvem (não armazena localmente)
  if (isConnected) {
    sendDataToCloud(jsonString);
    return;
  }

  // Se desconectado, armazena localmente (Edge first)
  if (sdCardAvailable) {
    if (currentSampleCount < MAX_STORED_SAMPLES) {
      File file = SD.open(DATA_FILE, FILE_APPEND); // append preserva histórico
      if (file) {
        file.println(jsonString);
        file.close();
        currentSampleCount++;
        saveSampleCount(); // persiste o contador (sobrevive a resets)
        Serial.println("💾 Stored locally (" + String(currentSampleCount) + "/" + String(MAX_STORED_SAMPLES) + ")");
      } else {
        Serial.println("❌ Storage Failed");
      }
    } else {
      // Limite atingido: aplica FIFO removendo a linha mais antiga
      Serial.println("🔄 Circular Buffer");
      implementCircularBuffer(jsonString);
      // Mantém contador consistente no limite
      currentSampleCount = MAX_STORED_SAMPLES;
      saveSampleCount();
    }
  } else {
    // SD Card não disponível - apenas simula o armazenamento
    Serial.println("💾 Simulated: " + jsonString);
    currentSampleCount++;
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
  if (!sdCardAvailable) {
    return;
  }

  // Lê conteúdo atual
  String allData = "";
  File file = SD.open(DATA_FILE, FILE_READ);
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
  file = SD.open(DATA_FILE, FILE_WRITE);
  if (file) {
    file.print(allData);
    file.close();
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
  Serial.println("🔄 Syncing " + String(currentSampleCount) + " samples...");

  if (!sdCardAvailable) {
    Serial.println("❌ No SD Card available");
    return;
  }

  File file = SD.open(DATA_FILE, FILE_READ);
  if (!file) {
    Serial.println("❌ No data to sync");
    return;
  }

  int syncedCount = 0;
  Serial.println("📤 Reading all stored data for batch sync...");
  
  // Lê todo o conteúdo do arquivo primeiro
  String fileContent = file.readString();
  file.close();
  
  Serial.println("📄 File content length: " + String(fileContent.length()));
  Serial.println("📄 File content preview: " + fileContent.substring(0, min(200, (int)fileContent.length())));
  
  // Conta linhas e prepara dados em lote
  int startPos = 0;
  int endPos = 0;
  String batchData = "[";
  bool firstEntry = true;
  
  // Processa cada linha do arquivo
  while (startPos < fileContent.length()) {
    endPos = fileContent.indexOf('\n', startPos);
    if (endPos == -1) {
      endPos = fileContent.length();
    }
    
    String line = fileContent.substring(startPos, endPos);
    line.trim();
    
    if (line.length() > 0) {
      if (!firstEntry) {
        batchData += ",";
      }
      batchData += line;
      firstEntry = false;
      syncedCount++;
    }
    
    startPos = endPos + 1;
  }
  
  batchData += "]";
  
  if (syncedCount > 0) {
    Serial.println("📤 Batch syncing " + String(syncedCount) + " samples...");
    sendBatchDataToCloud(batchData);
  }

  if (syncedCount > 0) {
    Serial.println("🗑️ Removing local file after successful sync...");
    SD.remove(DATA_FILE);
    currentSampleCount = 0;
    saveSampleCount();
    
    Serial.println("✅ Synced " + String(syncedCount) + " samples - local storage cleared");
  } else {
    Serial.println("⚠️ No data was synced");
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
  sendBatchDataToCloud():
    - Envia todos os dados em lote como um array JSON.
    - Mais eficiente que envios individuais.
    - Na Parte 2, substituir por HTTP POST com array JSON.
*/
void sendBatchDataToCloud(String batchJsonData) {
  Serial.println("☁️ Batch sending to Cloud:");
  Serial.println(batchJsonData);
  Serial.println("✅ Batch transmission completed");

  // Implementação real (Parte 2):
  //  - HTTP POST com Content-Type: application/json
  //  - Endpoint: POST /api/sensor-data/batch
  //  - Payload: array de objetos JSON
}

/*
  loadSampleCount():
    - Lê de /sample_count.txt o número de amostras armazenadas.
    - Ajuda a manter consistência após resets de energia/reboot.
*/
void loadSampleCount() {
  if (!sdCardAvailable) {
    currentSampleCount = 0;
    return;
  }

  File file = SD.open("/sample_count.txt", FILE_READ);
  if (file) {
    currentSampleCount = file.parseInt();
    file.close();
    Serial.println("📊 Loaded: " + String(currentSampleCount) + " samples");
  } else {
    currentSampleCount = 0;
  }
}

/*
  saveSampleCount():
    - Persiste o contador de amostras em /sample_count.txt.
    - Usado após cada append/flush para refletir o estado atual.
*/
void saveSampleCount() {
  if (!sdCardAvailable) {
    return;
  }

  File file = SD.open("/sample_count.txt", FILE_WRITE);
  if (file) {
    file.println(currentSampleCount);
    file.close();
  }
}
