#include "hardware/pwm.h"

PWM::PWM(int pwmPin, float maxVoltage) : pin(pwmPin), maxVoltage(maxVoltage), initialized(false)
{
    // Only support pin 9 for now
    if (pin != 9)
    {
        // Could add error handling here
        return;
    }

    // Just store the pin number, actual initialization happens in begin()
}

/**
 * @brief Setup PWM on the specified pin with the given frequency
 * @param freq_hz Frequency in Hz for the PWM signal (default: 20kHz)
 *
 * This function initializes the PWM hardware if not already done,
 * sets the pin mode, and configures Timer1 for Fast PWM mode.
 */
void PWM::setup(float freq_hz)
{
    if (pin != 9)
        return;

    // Initialize PWM hardware if not already done
    if (!initialized)
    {
        pinMode(pin, OUTPUT); // Set Pin 9 (OC1A) as output

        // Stop Timer1 and reset control registers
        TCCR1A = 0;
        TCCR1B = 0;
        TCNT1 = 0;

        // Set Fast PWM mode with ICR1 as TOP
        TCCR1A = (1 << COM1A1);               // Non-inverting mode on OC1A
        TCCR1A |= (1 << WGM11);               // Fast PWM part 1
        TCCR1B = (1 << WGM13) | (1 << WGM12); // Fast PWM part 2
        TCCR1B |= (1 << CS10);                // No prescaler (Timer clock = 16 MHz)

        initialized = true;
    }

    // 16MHz / freq_hz = timer top value
    // Subtract 1 from result because counter goes from 0 to TOP
    unsigned int top = (16000000.0 / freq_hz) - 1;
    ICR1 = top;
}

void PWM::setDutyCycle(float duty_cycle)
{
    if (!initialized || pin != 9)
        return;

    // Calculate OCR1A value based on normalized duty cycle (0.0 - 1.0)
    unsigned int duty_value = (ICR1 * duty_cycle);
    OCR1A = duty_value;
}

void PWM::setVoltage(float voltage)
{
    if (!initialized || pin != 9)
        return;

    // Clamp voltage to the range [0, maxVoltage]
    float clampedVoltage = constrain(voltage, 0.0, maxVoltage);

    // Calculate normalized duty cycle (0.0 - 1.0)
    float dutyCycle = clampedVoltage / maxVoltage;

    // Set the PWM duty cycle
    setDutyCycle(dutyCycle);
}
