#include <Arduino.h>

class PwmFan
{
public:
    PwmFan(int pin, int minSpeed, int maxSpeed)
    {
        this->pin = pin;
        this->minSpeed = minSpeed;
        this->maxSpeed = maxSpeed;
        pinMode(pin, OUTPUT);
    }
    void setSpeed(int speed)
    {
        if (speed != 0)
        {
            if (speed < minSpeed)
            {
                speed = minSpeed;
            }
            if (speed > maxSpeed)
            {
                speed = maxSpeed;
            }
        }
        analogWrite(pin, speed);
    }

    void setPercent(int percent)
    {
        int speed = map(percent, 0, 100, minSpeed, maxSpeed);
        // if percent is 0, set speed to 0
        if (percent == 0)
        {
            speed = 0;
        }
        setSpeed(speed);
    }

private:
    int pin;
    int minSpeed;
    int maxSpeed;
};
