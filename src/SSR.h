#include <Arduino.h>

class SSR
{
public:
    SSR(int pin)
    {
        this->pin = pin;
        pinMode(pin, OUTPUT);
    }
    void setState(bool state)
    {
        this->state = state;
        digitalWrite(pin, state);
    }
    bool getState()
    {
        return state;
    }

    void toggleState()
    {
        setState(!state);
    }

    void on()
    {
        setState(true);
    }
    void off()
    {
        setState(false);
    }

private:
    int pin;
    bool state;
};