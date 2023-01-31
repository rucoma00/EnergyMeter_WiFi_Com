#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

// UART2 pins
#define U2TX 17
#define U2RX 16

// Graph variables
String _Vrms;
String _Irms;
String _Pa;
String _Q;
String _Sa;
String _PF;
String _Energy;

// Replace with your network credentials
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);


// Function to process recieved string
int processString(String r)
{
  if (r.length() <= 0)
    return 1;
  else
  {
    int comma1=r.indexOf(",");
    int comma2=r.indexOf(",", comma1+1);
    int comma3=r.indexOf(",", comma2+1);
    int comma4=r.indexOf(",", comma3+1);
    int comma5=r.indexOf(",", comma4+1);
    int comma6=r.indexOf(",", comma5+1);

    _Vrms = r.substring(0,comma1);
    _Irms = r.substring(comma1+1,comma2);
    _Pa = r.substring(comma2+1,comma3);
    _Q = r.substring(comma3+1,comma4);
    _Sa = r.substring(comma4+1,comma5);
    _PF = r.substring(comma5+1,comma6);
    _Energy = r.substring(comma6+1);
    return 0;
  }
}
 
void setup() {
  Serial.begin(115200);
  Serial2.begin(921600, SERIAL_8N1,U2RX,U2TX);  // Serial2.begin(baud-rate, protocol, RX pin, TX pin); (default protocol) 130853
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  // Initialize SPIFFS
  if(!SPIFFS.begin(true)){
    for(int i=0; i<4;i++)
    {
      digitalWrite(LED_BUILTIN, LOW);
      delay(1000);
      digitalWrite(LED_BUILTIN, HIGH);
    }
    return;
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html");
  });
  server.on("/Energy", HTTP_GET, [](AsyncWebServerRequest *request){
  //request->send_P(200, "text/plain", readBME280Temperature().c_str());
  request->send_P(200, "text/plain", _Energy.c_str());
  });
  server.on("/Vrms", HTTP_GET, [](AsyncWebServerRequest *request){
    //request->send_P(200, "text/plain", readBME280Temperature().c_str());
    request->send_P(200, "text/plain", _Vrms.c_str());
  });
  server.on("/Irms", HTTP_GET, [](AsyncWebServerRequest *request){
    //request->send_P(200, "text/plain", readBME280Humidity().c_str());
    request->send_P(200, "text/plain", _Irms.c_str());
  });
  server.on("/P", HTTP_GET, [](AsyncWebServerRequest *request){
    //request->send_P(200, "text/plain", readBME280Pressure().c_str());
    request->send_P(200, "text/plain", _Pa.c_str());
  });
  server.on("/Q", HTTP_GET, [](AsyncWebServerRequest *request){
    //request->send_P(200, "text/plain", readBME280Pressure().c_str());
    request->send_P(200, "text/plain", _Q.c_str());
  });
  server.on("/S", HTTP_GET, [](AsyncWebServerRequest *request){
    //request->send_P(200, "text/plain", readBME280Pressure().c_str());
    request->send_P(200, "text/plain", _Sa.c_str());
  });
  server.on("/PF", HTTP_GET, [](AsyncWebServerRequest *request){
    //request->send_P(200, "text/plain", readBME280Pressure().c_str());
    request->send_P(200, "text/plain", _PF.c_str());
  });

  // Start server
  server.begin();
}
 
void loop() {
  String recieve;
  if(Serial2.available())
  {
    recieve = Serial2.readStringUntil('\n');
  }
  if(!processString(recieve))
  {
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(LED_BUILTIN, HIGH);
  } 
}