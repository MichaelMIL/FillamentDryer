#include <Wire.h>
#include <Adafruit_AHTX0.h>

Adafruit_AHTX0 heaterAht;
Adafruit_AHTX0 coolerAht;

void initAHT()
{
    if (!heaterAht.begin(&Wire1))
    {
        Serial.println("Could not find AHT 1? Check wiring");
    }
    if (!coolerAht.begin(&Wire))
    {
        Serial.println("Could not find AHT 2? Check wiring");
    }
}