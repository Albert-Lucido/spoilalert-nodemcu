#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiManager.h>
#include <EEPROM.h>

#define EEPROM_SIZE 100

// Pins and Sensors
#include <DHT.h>
#define DHTPIN 4
#define DHTTYPE DHT11
#define MQ_PIN A0

DHT dht(DHTPIN, DHTTYPE);

// Variables
String serverUrl;

// Function to read stored server URL from EEPROM
String readServerUrlFromEEPROM() {
  char data[EEPROM_SIZE];
  for (int i = 0; i < EEPROM_SIZE; i++) {
    data[i] = EEPROM.read(i);
  }
  return String(data);
}

// Function to write server URL to EEPROM
void writeServerUrlToEEPROM(const String& url) {
  for (int i = 0; i < EEPROM_SIZE; i++) {
    EEPROM.write(i, i < url.length() ? url[i] : 0);
  }
  EEPROM.commit();
}

void setup() {
  Serial.begin(115200);
  EEPROM.begin(EEPROM_SIZE);
  dht.begin();

  WiFiManager wifiManager;

  // Custom server URL field
  WiFiManagerParameter custom_server("server", "Server URL", readServerUrlFromEEPROM().c_str(), 100);
  wifiManager.addParameter(&custom_server);

  // AutoConnect will use saved Wi-Fi or start AP
  if (!wifiManager.autoConnect("NodeMCU-Setup")) {
    Serial.println("⚠️ Failed to connect. Restarting...");
    delay(3000);
    ESP.restart();
  }

  // Save server URL
  serverUrl = String(custom_server.getValue());
  writeServerUrlToEEPROM(serverUrl);

  Serial.println("✅ Connected to WiFi");
  Serial.print("🌐 Server URL: ");
  Serial.println(serverUrl);
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  int gasReading = analogRead(MQ_PIN);
  int co2 = map(gasReading, 0, 1023, 0, 1000);
  int ethylene = map(gasReading, 0, 1023, 0, 50);

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("❌ Sensor read failed");
    return;
  }

  String vegetable = determineVegetable(humidity, temperature, co2, ethylene);
  sendToServer(temperature, humidity, co2, ethylene, vegetable);
  delay(3000);
}

String determineVegetable(float h, float t, int co2, int eth) {
  if (h > 95 || t > 10 || co2 > 1000 || eth > 10) return "Carrot";
  if (h > 90 || t > 12 || co2 > 1000 || eth > 10) return "Okra";
  if (h > 95 || t > 10 || co2 > 1000 || eth > 5) return "Lettuce";
  return "None";
}

void sendToServer(float temperature, float humidity, int co2, int ethylene, String vegetable) {
  if (WiFi.status() != WL_CONNECTED) return;

  WiFiClient client;
  HTTPClient http;
  http.begin(client, serverUrl);

  http.addHeader("Content-Type", "application/json");

  String jsonData = "{\"temperature\":" + String(temperature, 2) +
                    ",\"humidity\":" + String(humidity, 2) +
                    ",\"co2\":" + String(co2) +
                    ",\"ethylene\":" + String(ethylene) +
                    ",\"vegetable\":\"" + vegetable + "\"}";

  int responseCode = http.POST(jsonData);

  if (responseCode > 0) {
    Serial.print("📤 Data sent. Code: ");
    Serial.println(responseCode);
  } else {
    Serial.print("❌ Failed. Code: ");
    Serial.println(responseCode);
  }

  http.end();
}
