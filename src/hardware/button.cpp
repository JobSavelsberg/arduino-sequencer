#include "hardware/button.h"

Button::Button(int buttonPin, bool usePullup) : pin(buttonPin), lastState(HIGH), currentState(HIGH), lastDebounceTime(0), debounceDelay(50)
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

void Button::update()
{
    bool reading = digitalRead(pin);

    if (reading != lastState)
    {
        lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > debounceDelay)
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
