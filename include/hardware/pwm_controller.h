#ifndef PWM_CONTROLLER_H
#define PWM_CONTROLLER_H

#include <Arduino.h>

// PWM output pins
extern const int PWM_PIN_9;  // Timer1 output A (OC1A)
extern const int PWM_PIN_10; // Timer1 output B (OC1B)

/**
 * Initialize PWM on Timer1 (Pins 9 and 10) with Fast PWM mode
 */
void initPWM();

/**
 * Set PWM frequency in Hz for Timer1 (affects both pins 9 and 10)
 * @param freq_hz Desired frequency in Hz
 */
void setPWMFrequency(float freq_hz);

/**
 * Set PWM duty cycle for a specific pin
 * @param pin PWM pin (9 or 10)
 * @param duty_cycle Duty cycle (0.0 - 1.0)
 */
void setPWMDutyCycle(int pin, float duty_cycle);

#endif // PWM_CONTROLLER_H
