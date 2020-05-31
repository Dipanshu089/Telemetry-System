//BMP180- Pressure,Temperature and Estimated altitude

// Import required libraries
#include "WiFi.h"
#include "ESPAsyncWebServer.h"

//for using BMP180 we need to include library containing its classes and functions
#include <Wire.h>
#include <Adafruit_BMP085.h>

// Set your access point network credentials
const char* ssid = "Client SSID";
const char* password = "applepie";

Adafruit_BMP085 bmp;//creating class object

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

String readTemp() {
  return String(bmp.readTemperature());
  //return String(1.8 * bmp.readTemperature() + 32);for temperature in 
}
String readPres() {
  return String(bmp.readPressure() / 100.0F);
}
String readSealevelPres() {
  return String(bmp.readSealevelPressure());
}
String readAlt() {
  return String(bmp.readAltitude());
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  Serial.println();
  
  // Setting the ESP as an access point
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readTemp().c_str());
  });
  server.on("/altitude", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readAlt().c_str());
  });
  server.on("/pressure", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readPres().c_str());
  });
    server.on("/sealevelpressure", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readSealevelPres().c_str());
  });
  
  bool status;
  status = bmp.begin(0x76);  
  if (!status) {
    Serial.println("Could not find a valid BMP180 sensor, check wiring!");
    while (1);
  }
  
  // Start server
  server.begin();
}
 
void loop(){
  
}
