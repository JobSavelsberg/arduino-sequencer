#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

class Button
{
private:
    int pin;
    bool lastState;
    bool currentState;
    unsigned long lastDebounceTime;
    unsigned long debounceDelay;

public:
    Button(int buttonPin, bool usePullup = true);
    void update();
    bool isPressed();
    bool wasPressed();
    bool wasReleased();
};

#endif // BUTTON_H
