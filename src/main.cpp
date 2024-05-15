#include <Arduino.h>
#include <Wire.h>
#include "FanControls.h"
#include "TempHumidDisplay.h"
#include "HW-040.h"
#include "ControlsMenuDisplay.h"
#include "PWM.h"
#include "SSR.h"
#include "AHT21.h"
#include <AM2302-Sensor.h>
constexpr unsigned int SENSOR_PIN{45};

AM2302::AM2302_Sensor am2302{SENSOR_PIN};

ControlsMenuDisplay display3(Wire1);
TempHumidDisplay display2(Wire1, 0x3D);
FansControlDisplay display(Wire);

PwmFan heaterFan(7, 100, 255);
PwmFan coolerFan(6, 100, 255);
PwmFan ventFan(4, 100, 255);

SSR heater(15);
SSR cooler(5);

void setup()
{

    Serial.begin(115200);

    // Set up serial port for debugging
    Serial.println("Starting up...");
    // define wire pins
    Wire1.begin(35, 36);
    Wire.begin(38, 39);
    initAHT();
    am2302.begin();
    display.init();
    display2.init();
    display.setHeaterFanSpeed(0);
    display.setCoolerFanSpeed(0);
    display.setVentFanSpeed(0);
    display.update();
    display3.init();
    Knobinit();
}

void Knobloop()
{
    currentState = digitalRead(CLK);
    if (currentState != lastState)
    {
        if (digitalRead(DT) != currentState)
        {
            // counter++;
            // Serial.println("CW");
            display3.CW();
        }
        else
        {
            // counter--;
            // Serial.println("CCW");
            display3.CCW();
        }
        // Serial.print("Position: ");
        // Serial.println(counter);
    }
    lastState = currentState;
    bool currentSwitchState = digitalRead(SW);
    if (lastSwitchState == HIGH && currentSwitchState == LOW && millis() - lastDebounceTime > debounceDelay)
    {
        // Serial.println("Switch pressed!");
        lastDebounceTime = millis();
        lastSwitchState = currentSwitchState; // Update lastSwitchState with currentSwitchState
        display3.click();
    }
    else
    {
        lastSwitchState = currentSwitchState; // Update lastSwitchState with currentSwitchState
    }
}

unsigned long previousMillis = 0;
const unsigned long interval = 1000; // 1 second
bool isAutoMode = false;
float autoModeTime = 0;
int autoModeInterval = 60 * 1000;
unsigned long lastAutoMode = 0;
bool initAutoMode = false;

void loop()
{
    unsigned long currentMillis = millis();
    Knobloop();
    if (display3.getIsAutoMode())
    {
        display3.setIsAutoMode(false);
        isAutoMode = true;
        initAutoMode = true;
        autoModeTime = display3.getAutoModeTimer() * 3600000;
    }
    if (initAutoMode)
    {
        initAutoMode = false;
        display3.setHeaterFanSpeed(100);
        display3.setCoolerFanSpeed(100);
        display3.setVentFanSpeed(100);
        display3.setHeaterState(true);
        display3.setCoolerState(true);
        display3.update();
    }
    if (isAutoMode && currentMillis - lastAutoMode > autoModeInterval)
    {
        lastAutoMode = currentMillis;
        autoModeTime -= autoModeInterval;
        display3.setAutoModeTimer(autoModeTime / 3600000); // Cast the result of the division to a float
        display3.update();
    }
    if (isAutoMode && autoModeTime <= 0)
    {
        isAutoMode = false;
        display3.setHeaterFanSpeed(0);
        display3.setCoolerFanSpeed(0);
        display3.setVentFanSpeed(0);
        display3.setHeaterState(false);
        display3.setCoolerState(false);
        display3.update();
    }
    if (currentMillis - previousMillis >= interval)
    {
        previousMillis = currentMillis;
        sensors_event_t humidity, temp;
        heaterAht.getEvent(&humidity, &temp);
        display2.setTopTemp(temp.temperature);
        display2.setTopHumid(humidity.relative_humidity);
        Serial.print(temp.temperature);
        Serial.print(",");
        Serial.print(humidity.relative_humidity);
        Serial.print(",");
        coolerAht.getEvent(&humidity, &temp);
        display2.setMidTemp(temp.temperature);
        display2.setMidHumid(humidity.relative_humidity);
        Serial.print(temp.temperature);
        Serial.print(",");
        Serial.print(humidity.relative_humidity);
        Serial.print(",");
        // Get temperature event and print its value.
        auto status = am2302.read();
        display2.setBotTemp(am2302.get_Temperature());
        // Get humidity event and print its value.
        display2.setBotHumid(am2302.get_Humidity());
        Serial.print(am2302.get_Temperature());
        Serial.print(",");
        Serial.println(am2302.get_Humidity());
        display2.update();
    }
    if (display3.getNewState())
    {
        int heaterFanSpeed = display3.getHeaterFanSpeed();
        int coolerFanSpeed = display3.getCoolerFanSpeed();
        int ventFanSpeed = display3.getVentFanSpeed();
        bool heaterState = display3.getHeaterState();
        bool coolerState = display3.getCoolerState();
        heaterFan.setPercent(heaterFanSpeed);
        coolerFan.setPercent(coolerFanSpeed);
        ventFan.setPercent(ventFanSpeed);
        heater.setState(heaterState);
        cooler.setState(coolerState);

        display.setHeaterFanSpeed(heaterFanSpeed);
        display.setCoolerFanSpeed(coolerFanSpeed);
        display.setVentFanSpeed(ventFanSpeed);
        display.update();
        display3.clearNewState();
    }
    // ArduinoOTA.handle(); // Handles a code update request
}