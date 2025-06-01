#ifndef PWM_CONTROLLER_H
#define PWM_CONTROLLER_H

#include <Arduino.h>

namespace hardware
{
    // PWM channel definitions
    enum PWMChannel
    {
        PWM_CHANNEL_9 = 0,  // Pin 9 (Timer1A)
        PWM_CHANNEL_10 = 1, // Pin 10 (Timer1B)
        PWM_CHANNEL_11 = 2, // Pin 11 (Timer2A)
        PWM_CHANNEL_3 = 3   // Pin 3 (Timer2B)
    };

    /**
     * Initialize PWM channels
     * @param channels Array of PWM channels to initialize
     * @param numChannels Number of channels in the array
     */
    void initPWM(PWMChannel channels[], int numChannels);

    /**
     * Set PWM frequency in Hz
     * @param freq_hz Desired frequency in Hz
     */
    void setPWMFrequency(float freq_hz);

    /**
     * Set PWM duty cycle as a normalized value for a specific channel
     * @param channel PWM channel (PWM_CHANNEL_3, PWM_CHANNEL_9, PWM_CHANNEL_10, or PWM_CHANNEL_11)
     * @param duty_cycle Duty cycle (0.0 - 1.0)
     */
    void setPWMDutyCycle(PWMChannel channel, float duty_cycle);
#endif // PWM_CONTROLLER_H
}
