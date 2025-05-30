#ifndef ANALOG_OUTPUT_H
#define ANALOG_OUTPUT_H

#include <Arduino.h>

/**
 * General purpose analog voltage output
 * For non-musical control voltages, envelopes, LFOs, etc.
 */

// Available analog output pins
extern const int ANALOG_OUT_PIN_9;  // Primary CV output (typically pitch)
extern const int ANALOG_OUT_PIN_10; // Secondary CV output (modulation)

/**
 * Initialize analog output system
 */
void initAnalogOutput();

/**
 * Set analog output to a specific voltage on a specific pin
 * @param pin Output pin (9 or 10)
 * @param voltage Output voltage (0.0 to maxVoltage)
 * @param maxVoltage Maximum output voltage (default: 5.0V)
 */
void setAnalogVoltage(int pin, float voltage, float maxVoltage = 5.0);

/**
 * Set analog output as percentage of max voltage on a specific pin
 * @param pin Output pin (9 or 10)
 * @param percentage 0.0 to 1.0 (0% to 100%)
 */
void setAnalogPercent(int pin, float percentage);

/**
 * Turn off specific analog output (set to 0V)
 * @param pin Output pin (9 or 10)
 */
void analogOff(int pin);

/**
 * Turn off all analog outputs
 */
void analogOffAll();

#endif
