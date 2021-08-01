#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "Hotspot.h"
#include "EepromSettings.h"
#include "WebServer.h"

//Variables
int i = 0;
int statusCode;
String st;
String content;

Hotspot hotspot;
EepromSettings eepromSettings = *(EepromSettings::instance());
WebServer webServer;

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
  String esid = eepromSettings.getValue(eepromSettings.getSsidKey());
  Serial.println();
  Serial.print("SSID: ");
  Serial.println(esid);
  String epass = eepromSettings.getValue(eepromSettings.getSsidPasswordKey());
  Serial.print("PASS: ");
  Serial.println(epass);
  WiFi.begin(esid.c_str(), epass.c_str());
  if (isConnectedToWifi())
  {
    Serial.println("Succesfully Connected to wifi!");
    webServer.launchApiServer();
  }
  else
  {
    Serial.println("Can't connect to wifi. Turning the HotSpot On.");
    hotspot.start();
    webServer.launchHotspotServer();
  }
}
void loop() {
    webServer.handleClient();
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
