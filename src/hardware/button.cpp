#include "hardware/button.h"

Button::Button(int buttonPin, bool usePullup)
    : pin(buttonPin), lastState(HIGH), currentState(HIGH), lastPressedState(false), debounceTimer(0), debounceDelay(0.05f) // 50ms as seconds
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

void Button::update(float dt)
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
    bool pressed = isPressed();
    bool result = pressed && !lastPressedState;
    lastPressedState = pressed;
    return result;
}

bool Button::wasReleased()
{
    bool pressed = isPressed();
    bool result = !pressed && lastPressedState;
    lastPressedState = pressed;
    return result;
}
