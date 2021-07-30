#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include "Hotspot.h"
#include "EepromSettings.h"

//Variables
int i = 0;
int statusCode;
const char* ssid = "";
const char* passphrase = "";
String st;
String content;

ESP8266WebServer server(80);
Hotspot Hotspot(server);
EepromSettings EepromSettings;

void setup()
{
  Serial.begin(115200); //Initializing if(DEBUG)Serial Monitor
  Serial.println();
  Serial.println("Disconnecting previously connected WiFi");
  WiFi.disconnect();
  delay(10);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println();
  Serial.println();
  Serial.println("Startup");
  String esid = EepromSettings.getValue(EepromSettings.getSsidKey());
  Serial.println();
  Serial.print("SSID: ");
  Serial.println(esid);
  String epass = EepromSettings.getValue(EepromSettings.getSsidPasswordKey());
  Serial.print("PASS: ");
  Serial.println(epass);
  WiFi.begin(esid.c_str(), epass.c_str());
  if (isConnectedToWifi())
  {
    Serial.println("Succesfully Connected to wifi!");
    createClientControllers();
  }
  else
  {
    Serial.println("Can't connect to wifi. Turning the HotSpot On.");
    Hotspot.start();
  }
}
void loop() {
  //server.handleClient();
  if ((WiFi.status() == WL_CONNECTED))
  {
    // Add your program code here which the esp8266 has to perform when it connects to network

  }
  else
  {
  }

}


//Functions used for saving WiFi credentials and to connect to it which you do not need to change
bool isConnectedToWifi(void)
{
  int c = 0;
  Serial.println("Checking WiFi connection");
  while ( c < 20 ) {
    if (WiFi.status() == WL_CONNECTED)
    {
      return true;
    }
    delay(500);
    Serial.print("*");
    c++;
  }
  Serial.println("");
  Serial.println("Connection timed out!");
  return false;
}



void createClientControllers() {
  server.on("/current-ssid", []() {
      Serial.println("Opened current sid controller");
      content = "{\"ssid\":\"";
      content += getEepromValue(0, 32);
      content += "\"}";
      server.sendHeader("Access-Control-Allow-Origin", "*");
      server.send(200, "application/json; charset=utf-8", content);
  });
  server.begin();
}

String getEepromValue(int start, int end) {
  String value;
  for (int i = start; i < end; ++i)
  {
    if (EEPROM.read(i) != NULL) {
      value += char(EEPROM.read(i));
    }
  }
  return value;
} 
