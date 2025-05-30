#ifndef PARAMETER_MANAGER_H
#define PARAMETER_MANAGER_H

#include <Arduino.h>

/**
 * Manages user input parameters (potentiometers, buttons)
 * Provides debouncing and parameter smoothing
 */
class ParameterManager
{
private:
    // Button debouncing
    struct ButtonState
    {
        bool lastState;
        bool currentState;
        unsigned long lastDebounceTime;
        unsigned long debounceDelay;
    };

    ButtonState buttons[3]; // Assuming 3 buttons max

    // Parameter smoothing
    float lastBpm;
    float lastModulation;
    unsigned long lastUpdateTime;

    static const unsigned long UPDATE_INTERVAL = 50; // Update every 50ms
    static const unsigned long DEBOUNCE_DELAY = 50;  // 50ms debounce

public:
    ParameterManager();

    // Button handling
    bool isButtonPressed(int buttonPin);
    bool isButtonJustPressed(int buttonPin);  // Rising edge detection
    bool isButtonJustReleased(int buttonPin); // Falling edge detection

    // Parameter reading with smoothing
    float readBpm(int potPin, float minBpm = 60.0, float maxBpm = 200.0);
    float readModulation(int potPin, float minLevel = 0.0, float maxLevel = 1.0);

    // Update method (call in main loop)
    void update(unsigned long currentTime);

private:
    int getButtonIndex(int buttonPin);
    void updateButton(int buttonPin, unsigned long currentTime);
    float smoothParameter(float newValue, float lastValue, float smoothingFactor = 0.1);
};

#endif
