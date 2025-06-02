#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

class Button
{
private:
    int pin;
    bool lastState;
    bool currentState;
    double debounceTimer; // Time accumulated for debouncing in seconds
    double debounceDelay; // Debounce delay in seconds

public:
    Button(int buttonPin, bool usePullup = true);
    void update(double dt); // dt is delta time in seconds
    bool isPressed();
    bool wasPressed();
    bool wasReleased();
};

#endif // BUTTON_H
