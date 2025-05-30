#include "hardware/hardware.h"

// Pin definitions
const int POT_A = A3;
const int POT_B = A2;
const int POT_C = A1;

const int BUT_A = 2;
const int BUT_LEFT = 3;
const int BUT_RIGHT = 4;

const int LED_A = 13; // Also built-in LED on Arduino Uno
const int LED_B = 12;

// CV output pins (for reference - actual control is via cv_output.h)
const int CV_OUT_PIN_9 = 9;   // Primary CV output (pitch)
const int CV_OUT_PIN_10 = 10; // Secondary CV output (modulation)

void initHardware()
{
    // Initialize button pins
    pinMode(BUT_A, INPUT_PULLUP); // Use internal pull-up resistor
    pinMode(BUT_LEFT, INPUT_PULLUP);
    pinMode(BUT_RIGHT, INPUT_PULLUP);

    // Initialize LED pins
    pinMode(LED_A, OUTPUT);
    pinMode(LED_B, OUTPUT);
    digitalWrite(LED_A, LOW);
    digitalWrite(LED_B, LOW);
}

float readLogPot(int pin, float minValue, float maxValue, float curve)
{
    int analogValue = analogRead(pin);
    float normalizedValue = (float)analogValue / 1023.0;
    float logValue = pow(normalizedValue, curve);
    return minValue + logValue * (maxValue - minValue);
}

bool readButton(int pin)
{
    return digitalRead(pin) == LOW; // Button is pressed when pin reads LOW (due to pull-up)
}

void setLED(int pin, bool state)
{
    digitalWrite(pin, state ? HIGH : LOW);
}

void clearAllLEDs()
{
    digitalWrite(LED_A, LOW);
    digitalWrite(LED_B, LOW);
}
