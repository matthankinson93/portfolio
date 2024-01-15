#include <WiFiManager.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"

int level_1 = 80;
int level_2 = 100;
int level_3 = 50;
bool res;
String header;
AsyncWebServer server(80);

void initSPIFFS() {
  if (!SPIFFS.begin()) {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
}

void initWiFi(){
  WiFi.mode(WIFI_STA);
  WiFiManager wm;  
  res = wm.autoConnect("BrewList-Setup");
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  if (res)
  {
    String ssid = WiFi.SSID();
    String pass = WiFi.psk();
    IPAddress staticIP(192,168,20,50);
    IPAddress dns = WiFi.dnsIP();
    IPAddress gateway = WiFi.gatewayIP();
    IPAddress subnet = WiFi.subnetMask();
    WiFi.config(staticIP, gateway, subnet, dns, dns);
    Serial.println(WiFi.localIP());
  }
}

void setup()
{
  Serial.begin(115200);
  initWiFi();
  initSPIFFS();

  // Web Server Root URL
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });

  server.serveStatic("/", SPIFFS, "/");

  // Start server
  server.begin();
}

void loop()
{
}
