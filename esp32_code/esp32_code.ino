#include <WiFi.h>
#include <WiFiClient.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <BH1750.h>

// replace with your network credentials
const char* ssid = "AMBI";
const char* password = "nava8056";

// replace with your ThingSpeak channel details
const char* server = "api.thingspeak.com";
const String api_key = "1BCT096B40V75KA7";
const String field = "lux"; // change this to the field number you want to update

// initialize the BH1750 sensor
BH1750 bh1750;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  bh1750.begin();
  connectWiFi();
}

void loop() {
  float lux = bh1750.readLightLevel();

  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");

  if (WiFi.status() == WL_CONNECTED) {
    updateThingSpeak(lux);
  }

  delay(30000); // wait for 30 seconds before updating again
}

void connectWiFi() {
  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    delay(1000);    
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void updateThingSpeak(float value) {
  WiFiClient client;
  if (client.connect(server, 80)) {
    String postStr = api_key;
    postStr += "&";
    postStr += field;
    postStr += "=";
    postStr += String(value);

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + api_key + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);

    Serial.println("Updating ThingSpeak...");
    while (client.connected()) {
      String line = client.readStringUntil('\n');
      Serial.println(line);
    }
    Serial.println("ThingSpeak update completed");
  } else {
    Serial.println("Failed to connect to ThingSpeak");
  }
}
