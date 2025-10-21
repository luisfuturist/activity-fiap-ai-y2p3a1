#include <DHT.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>

#define DHT_PIN 15
#define DHT_TYPE DHT22
#define BUTTON_PIN 32

DHT dht(DHT_PIN, DHT_TYPE);

// Timing variables
unsigned long lastDhtRead = 0;
const unsigned long dhtInterval = 2000; // 2s between DHT reads

unsigned long lastBeatCalc = 0;
const unsigned long heartbeatInterval = 10000; // 10s between BPM calculations

// Heart rate variables
int beatCount = 0;
int bpm = 0;

// Button debounce variables
int lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50; // 50ms debounce

// WiFi simulation
bool isConnected = false;
unsigned long lastWifiToggle = 0;
const unsigned long wifiToggleInterval = 30000; // Toggle WiFi every 30s for simulation

// Data storage variables
const int MAX_STORED_SAMPLES = 100; // Maximum samples to store before overwriting old data
int currentSampleCount = 0;
const char* DATA_FILE = "/sensor_data.json";

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  dht.begin();
  
  // Initialize SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("❌ SPIFFS Mount Failed");
    return;
  }
  Serial.println("✅ SPIFFS Mounted Successfully");
  
  // Initialize WiFi simulation
  isConnected = false;
  Serial.println("📡 WiFi Status: DISCONNECTED (Simulation Mode)");
  
  // Load existing data count
  loadSampleCount();
}

void loop() {
  unsigned long now = millis();

  // --- Simulate WiFi connectivity changes ---
  if (now - lastWifiToggle >= wifiToggleInterval) {
    isConnected = !isConnected;
    lastWifiToggle = now;
    Serial.print("📡 WiFi Status: ");
    Serial.println(isConnected ? "CONNECTED" : "DISCONNECTED");
    
    // If just connected, try to sync stored data
    if (isConnected) {
      syncStoredData();
    }
  }

  // --- Handle button with debounce ---
  int reading = digitalRead(BUTTON_PIN);
  if (reading != lastButtonState) {
    lastDebounceTime = now;
  }

  if ((now - lastDebounceTime) > debounceDelay) {
    static int buttonState = HIGH;
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == LOW) { // Button pressed
        beatCount++;
        Serial.println("❤️ Heartbeat detected");
      }
    }
  }
  lastButtonState = reading;

  // --- Calculate BPM every heartbeatInterval ---
  if (now - lastBeatCalc >= heartbeatInterval) {
    bpm = (beatCount * 60000UL) / heartbeatInterval; // BPM based on interval
    beatCount = 0;
    lastBeatCalc = now;
    Serial.print("BPM: ");
    Serial.println(bpm);
  }

  // --- Read DHT22 sensor every dhtInterval ---
  if (now - lastDhtRead >= dhtInterval) {
    lastDhtRead = now;
    float temp = dht.readTemperature();
    float hum = dht.readHumidity();

    if (isnan(temp) || isnan(hum)) {
      Serial.println("⚠️ Failed to read from DHT sensor!");
    } else {
      Serial.print("Temp: ");
      Serial.print(temp);
      Serial.print(" °C, Hum: ");
      Serial.print(hum);
      Serial.println(" %");
      
      // Store sensor data locally
      storeSensorData(temp, hum, bpm);
    }
  }
}

// ===== SPIFFS Storage Functions =====

void storeSensorData(float temperature, float humidity, int heartRate) {
  // Create JSON document for sensor data
  DynamicJsonDocument doc(1024);
  doc["timestamp"] = millis();
  doc["temperature"] = temperature;
  doc["humidity"] = humidity;
  doc["heartRate"] = heartRate;
  doc["wifiStatus"] = isConnected ? "connected" : "disconnected";
  
  // Convert to JSON string
  String jsonString;
  serializeJson(doc, jsonString);
  
  // Always store data locally (Edge Computing principle)
  if (currentSampleCount < MAX_STORED_SAMPLES) {
    // Append to file
    File file = SPIFFS.open(DATA_FILE, "a");
    if (file) {
      file.println(jsonString);
      file.close();
      currentSampleCount++;
      saveSampleCount();
      Serial.println("💾 Data stored locally (Edge Computing)");
    } else {
      Serial.println("❌ Failed to open file for writing");
    }
  } else {
    // Storage limit reached - implement circular buffer strategy
    Serial.println("⚠️ Storage limit reached - implementing circular buffer");
    implementCircularBuffer(jsonString);
  }
  
  // If connected, try to send data immediately
  if (isConnected) {
    sendDataToCloud(jsonString);
  }
}

void implementCircularBuffer(String newData) {
  // Read all existing data
  String allData = "";
  File file = SPIFFS.open(DATA_FILE, "r");
  if (file) {
    allData = file.readString();
    file.close();
  }
  
  // Remove oldest entry (first line) and add new one
  int firstNewline = allData.indexOf('\n');
  if (firstNewline != -1) {
    allData = allData.substring(firstNewline + 1);
  }
  allData += newData + "\n";
  
  // Write back to file
  file = SPIFFS.open(DATA_FILE, "w");
  if (file) {
    file.print(allData);
    file.close();
    Serial.println("🔄 Circular buffer updated");
  }
}

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
      sendDataToCloud(line);
      syncedCount++;
      delay(100); // Small delay between sends
    }
  }
  file.close();
  
  // Clear the file after successful sync
  if (syncedCount > 0) {
    SPIFFS.remove(DATA_FILE);
    currentSampleCount = 0;
    saveSampleCount();
    Serial.print("✅ Synced ");
    Serial.print(syncedCount);
    Serial.println(" data points to cloud");
  }
}

void sendDataToCloud(String jsonData) {
  // Simulate sending to cloud via Serial.println
  Serial.println("☁️ Sending to cloud: " + jsonData);
  
  // In a real implementation, this would be:
  // - MQTT publish
  // - HTTP POST to REST API
  // - WebSocket connection
}

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

void saveSampleCount() {
  File file = SPIFFS.open("/sample_count.txt", "w");
  if (file) {
    file.println(currentSampleCount);
    file.close();
  }
}
