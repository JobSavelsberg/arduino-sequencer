#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

class Button
{
private:
    int pin;
    bool lastState;
    bool currentState;
    bool lastPressedState; // Track previous pressed state for wasPressed/wasReleased
    float debounceTimer;   // Time accumulated for debouncing in seconds
    float debounceDelay;   // Debounce delay in seconds

public:
    Button(int buttonPin, bool usePullup = true);
    void update(float dt); // dt is delta time in seconds
    bool isPressed();
    bool wasPressed();
    bool wasReleased();
};

#endif // BUTTON_H
