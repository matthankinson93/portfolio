#include <WiFiManager.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include <ArduinoJson.h>

bool res;
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len)
{
  AwsFrameInfo *info = (AwsFrameInfo *)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
  {
    data[len] = 0;
    ws.textAll(data);
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len)
{
  if (type == WS_EVT_CONNECT)
  {
    Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
  }
  else if (type == WS_EVT_DISCONNECT)
  {
    Serial.printf("WebSocket client #%u disconnected\n", client->id());
  }
  else if (type == WS_EVT_DATA)
  {
    handleWebSocketMessage(arg, data, len);
  }
}

void initWebSocket()
{
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

void initSPIFFS()
{
  if (!SPIFFS.begin())
  {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
}

void initWiFi()
{
  WiFi.mode(WIFI_STA);
  WiFiManager wm;
  res = wm.autoConnect("WebServer-Setup");
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    delay(1000);
  }
  if (res)
  {
    IPAddress staticIP = WiFi.localIP();
    staticIP[3] = 50;
    WiFi.config(staticIP, WiFi.gatewayIP(), WiFi.subnetMask(), WiFi.dnsIP());
  }
}

void setup()
{
  Serial.begin(115200);
  initWiFi();
  initSPIFFS();
  initWebSocket();

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/index.html", "text/html"); });

  server.serveStatic("/", SPIFFS, "/");

  server.on("/json", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    DynamicJsonDocument doc(1024);
    doc["heap"] = ESP.getFreeHeap();
    doc["ssid"] = WiFi.SSID();
    String response;
    serializeJson(doc, response);
    request->send(200, "application/json", response); });

  server.on("/login", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    if (!request->authenticate("admin", "admin"))
      return request->requestAuthentication();
    request->send(200, "text/plain", "Login Success!"); });

  server.begin();
}

void loop()
{
  ws.cleanupClients();
}
