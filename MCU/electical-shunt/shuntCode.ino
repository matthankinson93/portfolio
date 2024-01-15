#include <Preferences.h>
#include <BluetoothSerial.h>
#include <Arduino.h>
#include <Arduinojson.h>

const char *NVS_CURRENT_AMPS_KEY = "current_amps";
const char *NVS_MAX_AMPS_KEY = "max_amps";
const char *NVS_DEVICE_NAME_KEY = "device_name";
const char *NVS_CALIBRATION_FACTOR_KEY = "cal_factor";
const String ADV_KEY = "un4UB74aBJgy9Mu";
const int shuntPin = 23;
const int voltagePin = 24;
unsigned long LAST_MEASURE;
bool DEFINED;
float AMPS;
float MAX_AMPS;
float VOLTS;
float WATTS;
float PERCT;
String DEVICE = "New Shunt";
float CAL_FACTOR;

void check_keys()
{
    preferences.begin("my-app", false);
    if ((preferences.containsKey(NVS_MAX_AMPS_KEY)) && (preferences.containsKey(NVS_CURRENT_AMPS_KEY)) && (preferences.containsKey(NVS_DEVICE_NAME_KEY)) && (preferences.containsKey(NVS_CALIBRATION_FACTOR_KEY)))
    {
        DEFINED = true;
    }

    if (DEFINED)
    {
        MAX_AMPS = preferences.getFloat(NVS_MAX_AMPS_KEY);
        AMPS = preferences.getFloat(NVS_CURRENT_AMPS_KEY);
        DEVICE = preferences.getString(NVS_DEVICE_NAME_KEY);
        CAL_FACTOR = preferences.getFloat(NVS_CALIBRATION_FACTOR_KEY);
    }
    preferences.end();
}

void getUsage()
{
    AMPS += read_current();        // Returns current amp since last measurement. Then add the value to AMPS.
    VOLTS = readVolts();           // Returns live voltage number.
    PERCT = getPerct();            // Returns the % value of current against max.
    WATTS = getWatts(AMPS, VOLTS); // Returns the current usage in watts.

    // If value of AMPS has changed since last measurement:
    if (preferences.getFloat(NVS_CURRENT_AMPS_KEY) != AMPS)
    {
        saveAmps(AMPS);
    }
}

float read_current()
{
    // Only caught on first device boot.
    if (LAST_MEASURE == 0)
    {
        LAST_MEASURE = getTime();
    }

    float currentAmps = analogRead(shuntPin) * CAL_FACTOR;
    float timeDiff = (getTime() - LAST_MEASURE) / 3600000;
    float consumed;

    if (currentAmps >= 0)
    {
        consumed = currentAmps * timeDiff;
    }
    else
    {
        consumed = -currentAmps * timeDiff;
    }
    LAST_MEASURE = getTime();
    return consumed;
}

float readVolts()
{
    return static_cast<float>(analogRead(voltagePin));
}

float getPerct()
{
    float diff = MAX_AMPS - AMPS;
    float percentage = (diff / MAX_AMPS) * 100.0;
    return percentage;
}

float getTime()
{
    return millis();
}

float getWatts(float amps, float volts)
{
    return amps * volts;
}

void saveAmps(float amps)
{
    preferences.begin("my-app", false);
    preferences.putFloat(NVS_CURRENT_AMPS_KEY, amps);
    preferences.end();
}

void saveMaxAmp(float max)
{
    preferences.begin("my-app", false);
    preferences.putFloat(NVS_MAX_AMPS_KEY, max);
    preferences.end();
}

void saveName(String name)
{
    preferences.begin("my-app", false);
    preferences.putString(NVS_DEVICE_NAME_KEY, name);
    preferences.end();
}

void saveCalFactor(float cal)
{
    preferences.begin("my-app", false);
    preferences.putFloat(NVS_CALIBRATION_FACTOR_KEY, cal);
    preferences.end();
}

void checkBTCommand()
{
    if (SerialBT.available() > 0)
    {
        String jsonStr = SerialBT.readStringUntil('\n');
        jsonStr.trim();
        StaticJsonDocument<256> jsonBuffer;
        DeserializationError error = deserializeJson(jsonBuffer, jsonStr);

        if (error)
        {
            return;
        }

        if (jsonBuffer.containsKey("amps"))
        {
            saveAmps(jsonBuffer["amps"].as<float>());
        }

        if (jsonBuffer.containsKey("max"))
        {
            saveMaxAmp(jsonBuffer["max"].as<float>());
        }

        if (jsonBuffer.containsKey("name"))
        {
            saveName(jsonBuffer["name"]);
        }
    }
}

void postBTData(float amps, float volts, float perct, float watts, unsigned long lastMeasure, String name)
{
    StaticJsonDocument<256> jsonBuffer;
    jsonBuffer["amps"] = amps;
    jsonBuffer["volts"] = volts;
    jsonBuffer["perct"] = perct;
    jsonBuffer["watts"] = watts;
    jsonBuffer["lastMeasure"] = lastMeasure;
    jsonBuffer["name"] = name;
    String jsonData;
    serializeJson(jsonBuffer, jsonData);
    SerialBT.println(jsonData);
}

void setup()
{
    Preferences preferences;  // define nvs obj
    BluetoothSerial SerialBT; // define ble serial
    check_keys();             // check for valid keys
    SerialBT.begin(DEVICE.c_str());
}

void loop()
{
    checkBTCommand(); // Check incoming ble commands.
    if (DEFINED)      // Valid only if there are valid keys for calculations.
    {
        getUsage();                                                  // Read and calculate.
        postBTData(AMPS, VOLTS, PERCT, WATTS, LAST_MEASURE, DEVICE); // Post to ble.
    }
    else
    {
        check_keys(); // Re-check keys until keys are defined.
    }
    delay(3000); // 3-sec measurement intervals.
}