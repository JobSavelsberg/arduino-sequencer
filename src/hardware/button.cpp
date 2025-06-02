#include "hardware/button.h"

Button::Button(int buttonPin, bool usePullup)
    : pin(buttonPin), lastState(HIGH), currentState(HIGH), debounceTimer(0), debounceDelay(0.05) // 50ms as seconds
{
    if (usePullup)
    {
        pinMode(pin, INPUT_PULLUP);
    }
    else
    {
        pinMode(pin, INPUT);
    }
}

void Button::update(double dt)
{
    bool reading = digitalRead(pin);

    if (reading != lastState)
    {
        debounceTimer = 0; // Reset timer when state changes
    }
    else
    {
        debounceTimer += dt; // Accumulate time when state is stable
    }

    if (debounceTimer > debounceDelay)
    {
        if (reading != currentState)
        {
            currentState = reading;
        }
    }

    lastState = reading;
}

bool Button::isPressed()
{
    return currentState == LOW;
}

bool Button::wasPressed()
{
    static bool lastPressed = false;
    bool pressed = isPressed();
    bool result = pressed && !lastPressed;
    lastPressed = pressed;
    return result;
}

bool Button::wasReleased()
{
    static bool lastPressed = false;
    bool pressed = isPressed();
    bool result = !pressed && lastPressed;
    lastPressed = pressed;
    return result;
}
