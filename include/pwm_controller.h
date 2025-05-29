#ifndef PWM_CONTROLLER_H
#define PWM_CONTROLLER_H

#include <Arduino.h>

/**
 * Initialize PWM on Timer1 (Pin 9) with Fast PWM mode
 */
void initPWM();

/**
 * Set PWM frequency in Hz
 * @param freq_hz Desired frequency in Hz
 */
void setPWMFrequency(float freq_hz);

/**
 * Set PWM duty cycle as a normalized value
 * @param duty_cycle Duty cycle (0.0 - 1.0)
 */
void setPWMDutyCycle(float duty_cycle);

#endif // PWM_CONTROLLER_H
