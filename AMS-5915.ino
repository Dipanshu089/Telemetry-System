//import neccessary files
#include<"WiFi.h"
#include"ESPAsyncWebServer.h"

#include"AMS5915.h"
// an AMS5915 object, which is a
// static pressure sensure at I2C
// address of 0x12, on I2C bus 0,
// and is a AMS5915-1200-B
AMS5915 sPress(Wire,0x12,AMS5915::AMS5915_1200_B);
// Set your access point network credentials
const char* ssid = "ESP32-Access-Point";
const char* password = "123456789";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
buzzerpressure=false;
buzzertemperature=false;
String readPres() {
  float pressure;
  pressure=String(sPress.getPressure_Pa());
  return String(sPress.getPressure_Pa());
}
String readtemp() {
  float temperature;
  temperature=sPress.getTemperature_C();
  return String(sPress.getTemperature_C());
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
  
  server.on("/pressure", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readPres().c_str());
  });
   server.on("/pressure", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readtemp().c_str());
  });
  server.on("/pressurebuzzer", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", buzzerpressure);
  });
  server.on("/temperaturebuzzer", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", buzzertemperature);
  });
  
  bool status;
 while(!Serial){}

  // starting communication with the 
  // static pressure transducer
  if (sPress.begin() < 0) {
    Serial.println("Error communicating with sensor, check wiring and I2C address");
    while(1){}
  }
  // Start server
  server.begin();
}
 
void loop(){
   // read the sensor
  sPress.readSensor();

  // displaying the data
  Serial.print(sPress.getPressure_Pa(),6);
  if(sPress.getPressure_Pa()<t)
    buzzerpressure=true;
  Serial.print("\t");
  Serial.println(sPress.getTemperature_C(),6);
  if(sPress.getTemperature_C()<t)
    buzzertemperature=true;
}
