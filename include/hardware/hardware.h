#ifndef HARDWARE_H
#define HARDWARE_H

#include <Arduino.h>

// Pin definitions
extern const int POT_A;
extern const int POT_B;
extern const int POT_C;

extern const int BUT_A;
extern const int BUT_LEFT;
extern const int BUT_RIGHT;

extern const int LED_A;
extern const int LED_B;

// CV output pins (for reference - actual control is via cv_output.h)
extern const int CV_OUT_PIN_9;  // Primary CV output (pitch)
extern const int CV_OUT_PIN_10; // Secondary CV output (modulation)

/**
 * Initialize all hardware pins and settings
 */
void initHardware();

/**
 * @brief Read potentiometer with logarithmic curve mapping
 * @param pin Analog pin to read from (0-1023)
 * @param minValue Minimum output value
 * @param maxValue Maximum output value
 * @param curve Logarithmic curve factor (1.0 = linear, 2.0 = quadratic, etc.)
 * @return Mapped value between minValue and maxValue using logarithmic curve
 */
float readLogPot(int pin, float minValue, float maxValue, float curve = 2.0);

/**
 * Read the state of a button (returns true when pressed)
 * @param pin Digital pin to read from
 * @return true if button is pressed, false otherwise
 */
bool readButton(int pin);

/**
 * Set LED state
 * @param pin LED pin
 * @param state true for on, false for off
 */
void setLED(int pin, bool state);

/**
 * Turn off all LEDs
 */
void clearAllLEDs();

#endif
