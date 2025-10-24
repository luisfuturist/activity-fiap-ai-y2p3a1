/*
  ESP32 - MQTT com TLS e SPIFFS (versão mais "realista")
  - Envia dados JSON para HiveMQ Cloud
  - Armazena localmente quando offline e reenvia ao reconectar
  - Testado no Wokwi (usa setInsecure por limitação de TLS)
*/

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>

const char* WIFI_SSID = "Wokwi-GUEST";
const char* WIFI_PASS = "";

const char* MQTT_HOST = "57bb796054e7484293ac7c21ac5021e4.s1.eu.hivemq.cloud";
const uint16_t MQTT_PORT = 8883;
const char* MQTT_USER = "esp32user";
const char* MQTT_PASS = "A2b4c6d8e9f0g";
const char* TOPIC = "device1/data";

WiFiClientSecure secureClient;
PubSubClient mqtt(secureClient);

const char* UNSENT_FILE = "/unsent.txt";

unsigned long lastSend = 0;
const unsigned long SEND_INTERVAL = 3000;
unsigned long seq = 0;

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("\n[BOOT] Iniciando dispositivo...");

  if (!SPIFFS.begin(true)) {
    Serial.println("[ERRO] Falha ao montar SPIFFS.");
  } else {
    Serial.println("[OK] SPIFFS montado.");
  }

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("[WIFI] Conectando");
  unsigned long t0 = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - t0 < 10000) {
    Serial.print('.');
    delay(300);
  }
  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.printf("[WIFI] Conectado: %s\n", WiFi.localIP().toString().c_str());
  } else {
    Serial.println("[WIFI] Falha na conexão, modo simulado.");
  }

  secureClient.setInsecure(); // TLS “fake” para Wokwi

  mqtt.setServer(MQTT_HOST, MQTT_PORT);
  mqtt.setCallback(onMessage);
}

void loop() {
  unsigned long now = millis();
  if (now - lastSend < SEND_INTERVAL) {
    if (mqtt.connected()) mqtt.loop();
    return;
  }
  lastSend = now;

  if (WiFi.status() == WL_CONNECTED) {
    if (!mqtt.connected()) connectMQTT();
  }

  float temp = 36.5 + random(-100, 100) / 100.0;
  int bpm = random(60, 130);
  seq++;

  StaticJsonDocument<256> doc;
  doc["device"] = "device1";
  doc["seq"] = seq;
  doc["ts"] = (unsigned long)(now / 1000);
  doc["temperature"] = roundf(temp * 10) / 10.0;
  doc["bpm"] = bpm;

  char payload[256];
  serializeJson(doc, payload);

  Serial.printf("\n[DATA] #%lu | Temp=%.1f | BPM=%d | WiFi=%d | MQTT=%d\n", seq, temp, bpm, WiFi.status() == WL_CONNECTED, mqtt.connected());

  if (WiFi.status() == WL_CONNECTED && mqtt.connected()) {
    if (mqtt.publish(TOPIC, payload)) {
      Serial.println("[MQTT] Enviado com sucesso.");
      resendStored();
    } else {
      Serial.println("[MQTT] Falha no envio. Salvando localmente...");
      storeOffline(payload);
    }
  } else {
    Serial.println("[OFFLINE] Sem conexão. Salvando dados...");
    storeOffline(payload);
  }
}

// ==================== MQTT ====================

void connectMQTT() {
  if (mqtt.connected() || WiFi.status() != WL_CONNECTED) return;

  Serial.print("[MQTT] Conectando ao broker...");
  String clientId = "esp32-" + String((uint32_t)ESP.getEfuseMac(), HEX);

  if (mqtt.connect(clientId.c_str(), MQTT_USER, MQTT_PASS)) {
    Serial.println(" conectado.");
  } else {
    Serial.printf(" falhou (state=%d)\n", mqtt.state());
  }
}

void onMessage(char* topic, byte* payload, unsigned int len) {
  Serial.printf("[MQTT] Mensagem recebida (%s): ", topic);
  for (unsigned int i = 0; i < len; i++) Serial.print((char)payload[i]);
  Serial.println();
}

// ==================== SPIFFS ====================

void storeOffline(const String &data) {
#ifdef WOKWI_SIM
  Serial.println("[SIM] Ignorando escrita SPIFFS.");
  return;
#endif
  File f = SPIFFS.open(UNSENT_FILE, FILE_APPEND);
  if (!f) {
    Serial.println("[ERRO] Não foi possível abrir o arquivo.");
    return;
  }
  f.println(data);
  f.close();
  Serial.println("[SALVO] Dados armazenados localmente.");
}

void resendStored() {
#ifdef WOKWI_SIM
  Serial.println("[SIM] Ignorando reenvio SPIFFS.");
  return;
#endif
  if (!SPIFFS.exists(UNSENT_FILE)) return;

  File f = SPIFFS.open(UNSENT_FILE, FILE_READ);
  if (!f) return;

  std::vector<String> lines;
  while (f.available()) {
    String line = f.readStringUntil('\n');
    line.trim();
    if (line.length()) lines.push_back(line);
  }
  f.close();

  bool ok = true;
  for (auto &line : lines) {
    if (mqtt.connected() && mqtt.publish(TOPIC, line.c_str())) {
      Serial.println("[REENVIO] Mensagem reenviada.");
      delay(40);
    } else {
      ok = false;
      break;
    }
  }

  if (ok) {
    SPIFFS.remove(UNSENT_FILE);
    Serial.println("[REENVIO] Todos os dados enviados. Arquivo limpo.");
  } else {
    Serial.println("[REENVIO] Parcial. Arquivo mantido.");
  }
}
