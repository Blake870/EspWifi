#ifndef Hotspot_h
#define Hotspot_h
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// settings
#define SSID "PetFeeder"
#define SSID_PASS ""
String[] networks;
int networksAmount = 0;
ESP8266WebServer webServer;
class Hotspot {

public:
    Hotspot(ESP8266WebServer server) {
        webServer = server;
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
        createControllers();
    }

protected:
    void createControllers() {
        webServer.on("/", []() {
            IPAddress ip = WiFi.softAPIP();
            String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
            String st = "<select name='ssid'>";
            for (int i = 0; i < networksAmount; ++i)
            {
                // Print SSID for each network found
                st += "<option value='";
                st += networks[i];
                st += "'>";
                st += networks[i];
                st += "</option>";
            }
            st += "</select>";
            String content = "<!DOCTYPE html><html><head><meta name='viewport' content='width=device-width, initial-scale=1'><style>body{font-family: Arial, Helvetica, sans-serif;}form{border: 3px solid #f1f1f1;}input[type=text], input[type=password], select{width: 100%; padding: 12px 20px; margin: 8px 0; display: inline-block; border: 1px solid #ccc; box-sizing: border-box;}button{background-color: #4CAF50; color: white; padding: 14px 20px; margin: 8px 0; border: none; cursor: pointer; width: 100%;}button:hover{opacity: 0.8;}.cancelbtn{width: auto; padding: 10px 18px; background-color: #f44336;}.imgcontainer{text-align: center; margin: 24px 0 12px 0;}img.avatar{width: 40%; border-radius: 50%;}.container{padding: 16px;}span.psw{float: right; padding-top: 16px;}/* Change styles for span and cancel button on extra small screens */@media screen and (max-width: 300px){span.psw{display: block; float: none;}.cancelbtn{width: 100%;}}</style></head><body><h2>Настройка WiFi сети для PetFeeder</h2><form action='setting'> <div class='container'> <label for='uname'><b>Выберите WiFi Сеть из списка</b></label>";
            content += ipStr;
            content += st;
            content += " <label for='pass'><b>Пароль от WiFi сети</b></label> <input type='password' placeholder='Enter Password' name='pass' required> <button type='submit'>Сохранить</button> </div><div class='container' style='background-color:#f1f1f1'> <button type='button' class='cancelbtn'>Обновить список WiFi сетей</button> </div></form></body></html>";
            server.send(200, "text/html; charset=utf-8", content);
        });
        webServer.on("/scan", []() {
            //setupAP();
            IPAddress ip = WiFi.softAPIP();
            String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
            String content = "<!DOCTYPE HTML>\r\n<html><a href='/'>go back</a>";
            server.send(200, "text/html", content);
        });

        webServer.on("/setting", []() {
            String qsid = server.arg("ssid");
            String qpass = server.arg("pass");
            if (qsid.length() > 0 && qpass.length() > 0) {
                Serial.println("clearing eeprom");
                for (int i = 0; i < 96; ++i) {
                    EEPROM.write(i, 0);
                }
                Serial.println(qsid);
                Serial.println("");
                Serial.println(qpass);
                Serial.println("");

                Serial.println("writing eeprom ssid:");
                for (int i = 0; i < qsid.length(); ++i)
                {
                    EEPROM.write(i, qsid[i]);
                    Serial.print("Wrote: ");
                    Serial.println(qsid[i]);
                }
                Serial.println("writing eeprom pass:");
                for (int i = 0; i < qpass.length(); ++i)
                {
                    EEPROM.write(32 + i, qpass[i]);
                    Serial.print("Wrote: ");
                    Serial.println(qpass[i]);
                }
                EEPROM.commit();
                String content = "{\"Success\":\"saved to eeprom... reset to boot into new wifi\"}";
                int statusCode = 200;
                server.sendHeader("Access-Control-Allow-Origin", "*");
                server.send(statusCode, "application/json", content);
                ESP.reset();
            } else {
                String content = "{\"Error\":\"404 not found\"}";
                int statusCode = 404;
                Serial.println("Sending 404");
            }
            server.sendHeader("Access-Control-Allow-Origin", "*");
            server.send(statusCode, "application/json", content);
        });
        webServer.begin();
    }
    void scanNetworks(bool forceRescan = false) {
        if (networksAmount < 1 || forceRescan) {
            networks[] = [];
            int networksAmount = WiFi.scanNetworks();
            for (int i = 0; i < networksAmount; ++i) {
                networks[i] = WiFi.SSID(i);
            }
        }
    }
};

#endif