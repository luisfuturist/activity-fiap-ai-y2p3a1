#include <DHT.h>

#define DHT_PIN 15
#define DHT_TYPE DHT22
#define BUTTON_PIN 32

DHT dht(DHT_PIN, DHT_TYPE);

unsigned long lastDhtRead = 0;
const unsigned long dhtInterval = 2000; // 2s between DHT reads

unsigned long lastBeatCalc = 0;
const unsigned long heartbeatInterval = 10000; // 10s between BPM calculations

int beatCount = 0;
int bpm = 0;

// Debounce variables
int lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50; // 50ms debounce

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  dht.begin();
}

void loop() {
  unsigned long now = millis();

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
    }
  }
}
