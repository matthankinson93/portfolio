#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <WiFiManager.h>
#include "time.h"
#include <Preferences.h>

#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME ""
#define BLYNK_AUTH_TOKEN ""

BlynkTimer timer;
Preferences preferences;

int flowPin1 = 12;
int flowPin2 = 26;
volatile int count1;
volatile int count2;
double keg1;
double keg2;
double cal1;
double cal2;
double flowRate;
double FULL = 19.5000;
const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 36000;
const int daylightOffset_sec = 0;

void setup()
{
  delay(1000);
  Serial.begin(115200);
  Serial.println("Booting");
  connectToWiFi();
  Blynk.config(BLYNK_AUTH_TOKEN);
  pinMode(flowPin1, INPUT);
  pinMode(flowPin2, INPUT);
  attachInterrupt(digitalPinToInterrupt(flowPin1), Flow1, RISING);
  attachInterrupt(digitalPinToInterrupt(flowPin2), Flow2, RISING);
  preferences.begin("kegs", false);
  loadCountValues(); // load count values into RAM buffer
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  timer.setInterval(10000L, sendToBlynk);       // send data to Blynk every minute
  timer.setInterval(1800000L, saveCountValues); // save to NVS every 30 minute
}

void loop()
{
  Blynk.run();
  timer.run();
}

void sendToBlynk()
{
  keg1 = calculateRemaining(count1, cal1);
  keg2 = calculateRemaining(count2, cal2);
  Serial.print("Keg1: ");
  Serial.println(keg1);
  Serial.print("Keg2: ");
  Serial.println(keg2);
  Serial.print("count1: ");
  Serial.println(count1);
  Serial.print("Count2: ");
  Serial.println(count2);
  Blynk.virtualWrite(V0, keg1);
  Blynk.virtualWrite(V1, keg2);
  Blynk.virtualWrite(V7, calculatePercent(keg1));
  Blynk.virtualWrite(V8, calculatePercent(keg2));
  getDateString();
}

void loadCountValues()
{
  preferences.begin("kegs", false);
  count1 = preferences.getInt("count1", 0);
  count2 = preferences.getInt("count2", 0);
  preferences.end();
}

void saveCountValues()
{
  preferences.begin("kegs", false);
  preferences.putInt("count1", count1);
  preferences.putInt("count2", count2);
  preferences.end();
}

void Flow1()
{
  count1++;
}

void Flow2()
{
  count2++;
}

double calculateRemaining(int args, double calibration)
{
  double used = (args * 2.25) / 1000;
  used *= (1 - (calibration / 100)); // Subtract calibration percentage
  double amount = FULL - used;
  return amount;
}

double calculatePercent(double args)
{
  double percent = (args / 19.5) * 100.0;
  percent = max(min(percent, 100.0), 0.0);
  return percent;
}

BLYNK_WRITE(V3)
{
  cal1 = param.asFloat();
}

BLYNK_WRITE(V4)
{
  cal2 = param.asFloat();
}

BLYNK_WRITE(V5)
{
  if (param.asInt() == 1)
  {
    count1 = 0;
    saveCountValues();
  }
}

BLYNK_WRITE(V6)
{
  if (param.asInt() == 1)
  {
    count2 = 0;
    saveCountValues();
  }
}

void connectToWiFi()
{
  WiFiManager wifiManager;
  wifiManager.autoConnect("KegMonitorWi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
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
    char timeString[50]; // Buffer to store the formatted time string
    strftime(timeString, sizeof(timeString), "%H:%M:%S %d/%m/%Y", &timeinfo);
    Blynk.virtualWrite(V2, timeString);
  }
}
