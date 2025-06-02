#include "hardware/led.h"

LED::LED(int ledPin) : pin(ledPin), blinkStart(0), blinkDuration(0), isBlinking(false)
{
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}

void LED::on()
{
    isBlinking = false;
    digitalWrite(pin, HIGH);
}

void LED::off()
{
    isBlinking = false;
    digitalWrite(pin, LOW);
}

void LED::blink(double duration)
{
    isBlinking = true;
    blinkStart = 0;           // Start from 0 and count up
    blinkDuration = duration; // Duration in seconds
    digitalWrite(pin, HIGH);
}

void LED::update(double dt)
{
    if (isBlinking)
    {
        blinkStart += dt; // Accumulate time
        if (blinkStart >= blinkDuration)
        {
            digitalWrite(pin, LOW);
            isBlinking = false;
        }
    }
}
