#ifndef Hotspot_h
#define Hotspot_h

#include <ESP8266WiFi.h>

// settings
const String SSID = "PetFeeder";
const String SSID_PASS = "text";
String networks[20];
int networksAmount = 0;

class Hotspot {
public:
    Hotspot() {

    }
    
    void start() {
        WiFi.mode(WIFI_STA);
        WiFi.disconnect();
        delay(100);
        int n = WiFi.scanNetworks();
        Serial.println("scan completed");
        if (n == 0) {
            Serial.println("No WiFi Networks found");
        } else {
            scanNetworks();
            Serial.print(networksAmount);
            Serial.println("Networks found");
            for (int i = 0; i < networksAmount; ++i)
            {
                // Print SSID and RSSI for each network found
                Serial.print(i + 1);
                Serial.print(": ");
                Serial.print(networks[i]);
                delay(10);
            }
        }
        delay(100);
        WiFi.softAP(SSID, SSID_PASS);
        Serial.println("Initializing HotSpot");
    }

protected:
    void scanNetworks(bool forceRescan = false) {
        if (networksAmount < 1 || forceRescan) {
            int networksAmount = WiFi.scanNetworks();
            for (int i = 0; i < networksAmount; ++i) {
                networks[i] = WiFi.SSID(i);
            }
            for (int i = networksAmount; i < 20; ++i) {
                networks[i] = "";
            }
        }
    }
};

#endif
