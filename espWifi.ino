#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>

//Variables
int i = 0;
int statusCode;
const char* ssid = "";
const char* passphrase = "";
String st;
String content;

//--------Establishing Local server at port 80 whenever required
ESP8266WebServer server(80);

void setup()
{
  Serial.begin(115200); //Initializing if(DEBUG)Serial Monitor
  Serial.println();
  Serial.println("Disconnecting previously connected WiFi");
  WiFi.disconnect();
  EEPROM.begin(512); //Initializing EEPROM
  delay(10);
  // Clearing cache
  //for (int i = 0 ; i < EEPROM.length() ; i++) {
  //  EEPROM.write(i, 0);
  //}
  EEPROM.commit();
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println();
  Serial.println();
  Serial.println("Startup");
  String esid = getEepromValue(0, 32);
  Serial.println();
  Serial.print("SSID: ");
  Serial.println(esid);
  String epass = getEepromValue(32, 96);
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
    createHotspotControllers();
    startHotspot();// Setup accesspoint or HotSpot
  }
  server.begin();
}
void loop() {
  server.handleClient();
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
  Serial.println("Waiting for WiFi to connect");
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
  Serial.println("Connection timed out, opening AP or Hotspot");
  return false;
}

void startHotspot(void)
{
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  delay(100);
  int n = WiFi.scanNetworks();
  Serial.println("scan completed");
  if (n == 0) {
    Serial.println("No WiFi Networks found");
  } else {
    Serial.print(n);
    Serial.println("Networks found");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
      delay(10);
    }
  }
  Serial.println("");
  st = "<select name='ssid'>";
  for (int i = 0; i < n; ++i)
  {
    // Print SSID and RSSI for each network found
    st += "<option value='";
    st += WiFi.SSID(i);
    st += "'>";
    st += WiFi.SSID(i);
    st += " (";
    st += WiFi.RSSI(i);

    st += ")";
    st += (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*";
    st += "</option>";
  }
  st += "</select>";
  delay(100);
  WiFi.softAP("PetFeeder", "");
  Serial.println("Initializing_Wifi_accesspoint");
  Serial.println("over");
}

void createHotspotControllers()
{
  {
    server.on("/", []() {
      IPAddress ip = WiFi.softAPIP();
      String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
      content = "<!DOCTYPE html><html><head><meta name='viewport' content='width=device-width, initial-scale=1'><style>body{font-family: Arial, Helvetica, sans-serif;}form{border: 3px solid #f1f1f1;}input[type=text], input[type=password], select{width: 100%; padding: 12px 20px; margin: 8px 0; display: inline-block; border: 1px solid #ccc; box-sizing: border-box;}button{background-color: #4CAF50; color: white; padding: 14px 20px; margin: 8px 0; border: none; cursor: pointer; width: 100%;}button:hover{opacity: 0.8;}.cancelbtn{width: auto; padding: 10px 18px; background-color: #f44336;}.imgcontainer{text-align: center; margin: 24px 0 12px 0;}img.avatar{width: 40%; border-radius: 50%;}.container{padding: 16px;}span.psw{float: right; padding-top: 16px;}/* Change styles for span and cancel button on extra small screens */@media screen and (max-width: 300px){span.psw{display: block; float: none;}.cancelbtn{width: 100%;}}</style></head><body><h2>Настройка WiFi сети для PetFeeder</h2><form action='setting'> <div class='container'> <label for='uname'><b>Выберите WiFi Сеть из списка</b></label>";
      content += ipStr;
      content += st;
      content += " <label for='pass'><b>Пароль от WiFi сети</b></label> <input type='password' placeholder='Enter Password' name='pass' required> <button type='submit'>Сохранить</button> </div><div class='container' style='background-color:#f1f1f1'> <button type='button' class='cancelbtn'>Обновить список WiFi сетей</button> </div></form></body></html>";
      server.send(200, "text/html; charset=utf-8", content);
    });
    server.on("/scan", []() {
      //setupAP();
      IPAddress ip = WiFi.softAPIP();
      String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
      content = "<!DOCTYPE HTML>\r\n<html><a href='/'>go back</a>";
      server.send(200, "text/html", content);
    });

    server.on("/setting", []() {
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
        content = "{\"Success\":\"saved to eeprom... reset to boot into new wifi\"}";
        statusCode = 200;
        server.sendHeader("Access-Control-Allow-Origin", "*");
        server.send(statusCode, "application/json", content);
        ESP.reset();
      } else {
        content = "{\"Error\":\"404 not found\"}";
        statusCode = 404;
        Serial.println("Sending 404");
      }
      server.sendHeader("Access-Control-Allow-Origin", "*");
      server.send(statusCode, "application/json", content);
    });
  }
}



void createClientControllers(void) {
  server.on("/current-ssid", []() {
      Serial.println("Opened current sid controller");
      content = "{\"ssid\":\"";
      content += getEepromValue(0, 32);
      content += "\"}";
      server.sendHeader("Access-Control-Allow-Origin", "*");
      server.send(200, "application/json; charset=utf-8", content);
  });
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
