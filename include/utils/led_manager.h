#ifndef LED_MANAGER_H
#define LED_MANAGER_H

#include <Arduino.h>

/**
 * Manages LED states and timing for visual feedback
 * Provides non-blocking LED control with timing
 */
class LedManager
{
private:
    struct LedState
    {
        bool isOn;
        unsigned long onTime;
        unsigned long duration;
        bool isTemporary; // If true, LED will auto-turn off after duration
    };

    LedState ledStates[2]; // Assuming 2 LEDs

    static const int LED_COUNT = 2;

public:
    LedManager();

    // LED control
    void setLed(int ledPin, bool state);
    void setLedTemporary(int ledPin, unsigned long duration);
    void clearAllLeds();

    // Beat indication
    void indicateBeat(int ledPin, unsigned long duration);
    void indicateDownbeat(int ledPin, unsigned long duration);

    // State queries
    bool isLedOn(int ledPin) const;

    // Update method (call in main loop)
    void update(unsigned long currentTime);

private:
    int getLedIndex(int ledPin) const;
    void updateLed(int ledPin, unsigned long currentTime);
};

#endif
