#include <WiFi.h>
#include <HTTPClient.h>

int x=0;
// Set our wifi name and password
const char* ssid = "AMBI";
const char* password = "nava8056";

String serverName = "GET https://api.thingspeak.com/update?api_key=AFWLHUSJ14FM3J08&field1=0";

unsigned long lastTime = 0;
unsigned long timerDelay = 60000;



void setup() {

  pinMode(34,INPUT);
  Serial.begin(9600); // Opens up the serial port with a baudrate of 9600 bits per second
  WiFi.begin(ssid, password); // Attempt to connect to wifi with our password
  Serial.println("Connecting"); // Print our status to the serial monitor
  // Wait for wifi to connect
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if ((millis() - lastTime) > timerDelay) { // Check if its been a minute
    if(WiFi.status()== WL_CONNECTED){ // Check to make sure wifi is still connected
      x= analogRead(34);
      sendData(x); // Call the sendData function defined below
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}

void sendData(int dta){
  HTTPClient http; // Initialize our HTTP client
  Serial.println(dta);
  String url = serverName + "&field1=" + dta; // Define our entire url
  http.begin(url.c_str()); // Initialize our HTTP request
  int httpResponseCode = http.GET(); // Send HTTP request

  if (httpResponseCode > 0){ // Check for good HTTP status code
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
  }else{
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  http.end();
}