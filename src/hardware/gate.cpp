#include "hardware/gate.h"

Gate::Gate(int gatePin) : pin(gatePin), isHigh(false), gateStart(0), gateDuration(0), isGating(false)
{
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}

void Gate::high()
{
    isGating = false;
    isHigh = true;
    digitalWrite(pin, HIGH);
}

void Gate::low()
{
    isGating = false;
    isHigh = false;
    digitalWrite(pin, LOW);
}

void Gate::trigger(float duration)
{
    isGating = true;
    gateStart = 0;           // Start from 0 and count up
    gateDuration = duration; // Duration in seconds
    isHigh = true;
    digitalWrite(pin, HIGH);
}

void Gate::update(float dt)
{
    if (isGating)
    {
        gateStart += dt; // Accumulate time
        if (gateStart >= gateDuration)
        {
            isHigh = false;
            digitalWrite(pin, LOW);
            isGating = false;
        }
    }
}
