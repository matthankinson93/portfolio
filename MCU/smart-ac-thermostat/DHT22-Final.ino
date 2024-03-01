#define RECORD_GAP_MICROS 12000

#include <DHT.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <WiFiManager.h>
#include <IRremote.hpp>
#include "time.h"

#define DHT_PIN 25
#define IR_RECEIVE_PIN 26
#define IR_SEND_PIN 27
#define DHT_TYPE DHT22
#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME ""
#define BLYNK_AUTH_TOKEN ""
#define TOLERANCE 100

DHT dht(DHT_PIN, DHT_TYPE);
unsigned int loopCount = 0;
unsigned int tempSet;
unsigned int tempVar;
unsigned int autoMode;
unsigned int coolingMode;
float temperatureArray[96];
int arrayIndex = 0;
BlynkTimer timer;

const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 36000;
const int daylightOffset_sec = 0;

uint64_t IR_ON[] = {0x20004000E05FC3, 0x8705002000};
uint64_t IR_OFF[] = {0x20004000E05FC3, 0x6705000000};

// Init Wi-Fi, Blynk, Serial and DHT sensor
void setup()
{
  Serial.begin(9600);

  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
  IrSender.begin(IR_SEND_PIN, ENABLE_LED_FEEDBACK, 0);

  connectToWiFi();
  Blynk.config(BLYNK_AUTH_TOKEN);
  dht.begin();
  sensorDataSend();
  timer.setInterval(300000L, sensorDataSend);
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void loop()
{
  Blynk.run();
  timer.run();
  readIR();
}

void connectToWiFi()
{
  WiFiManager wifiManager;
  wifiManager.autoConnect("WiFiManagerAP");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void readIR()
{
  if (IrReceiver.decode())
  {
    Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);
    IrReceiver.printIRResultShort(&Serial);
    IrReceiver.printIRSendUsage(&Serial); // Print the IR string required to send this value.
    IrReceiver.resume();
  }
}

void sensorDataSend()
{
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  // Handle the data if the readings are valid via ESP32 -> Blynk
  if (!isnan(humidity) && !isnan(temperature) && humidity != 0 && temperature != 0)
  {
    Blynk.virtualWrite(V0, temperature);
    Blynk.virtualWrite(V1, humidity);

    loopCount++;
    if (loopCount % 12 == 0)
    {
      temperatureArray[arrayIndex] = temperature;
      arrayIndex++;
      loopCount = 0;
      if (arrayIndex >= 96)
      {
        arrayIndex = 0;
      }
    }

    float sum = 0.0;
    float average = 0.0;
    int count = 0;
    for (int i = 0; i < 96; i++)
    {
      if (temperatureArray[i] != 0.0)
      {
        sum += temperatureArray[i];
        count++;
      }
    }

    if (count > 0)
    {
      average = sum / count;
      Blynk.virtualWrite(V2, average);
    }
    else
    {
      Serial.println("No values in average struct!");
    }
    setAC(temperature);
  }
  else
  {
    Serial.println("DHT22 reading invalid!");
  }
  getDateString();
}

void getDateString()
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
  }
  else
  {
    char timeString[25];
    strftime(timeString, sizeof(timeString), "%H:%M:%S %d/%m/%Y", &timeinfo);
    Blynk.virtualWrite(V3, timeString);
  }
}

void onAC()
{
  Serial.flush();
  IrSender.sendPulseDistanceWidthFromArray(38, 9000, 4550, 550, 1700, 550, 550, &IR_ON[0], 104, PROTOCOL_IS_LSB_FIRST, 0, 1);
}

void offAC()
{
  Serial.flush();
  IrSender.sendPulseDistanceWidthFromArray(38, 9000, 4550, 550, 1700, 550, 550, &IR_OFF[0], 104, PROTOCOL_IS_LSB_FIRST, 0, 1);
}

// Blynk definitions
void BlynkOnConnected()
{
  Blynk.syncAll();
}

BLYNK_WRITE(V4)
{
  tempSet = param.asInt();
}

BLYNK_WRITE(V5)
{
  tempVar = param.asInt();
}

BLYNK_WRITE(V6)
{
  if (param.asInt() == 1)
  {
    autoMode = 1;
  }
  else
  {
    autoMode = 0;
  }
  onAC();
}

BLYNK_WRITE(V7)
{
  if (param.asInt() == 1)
  {
    coolingMode = 1;
  }
  else
  {
    coolingMode = 0;
  }
}

void setAC(float temperature)
{
  if (autoMode)
  {
    if (coolingMode)
    {
      if (temperature > (tempSet + tempVar))
      {
        onAC();
      }
      else if (temperature < (tempSet - tempVar))
      {
        offAC();
      }
      else
      {
        return;
      }
    }
    else
    {
      if (temperature < (tempSet - tempVar))
      {
        onAC();
      }
      else if (temperature > (tempSet + tempVar))
      {
        offAC();
      }
      else
      {
        return;
      }
    }
  }
}