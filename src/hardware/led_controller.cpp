#include "hardware/led_controller.h"

LED::LED(int ledPin) : pin(ledPin), isBlinking(false)
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

void LED::blink(unsigned long duration)
{
    isBlinking = true;
    blinkStart = millis();
    blinkDuration = duration;
    digitalWrite(pin, HIGH);
}

void LED::update()
{
    if (isBlinking && (millis() - blinkStart >= blinkDuration))
    {
        digitalWrite(pin, LOW);
        isBlinking = false;
    }
}
