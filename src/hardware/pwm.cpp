#include "hardware/pwm.h"

PWM::PWM(int pwmPin, float maxVoltage) : pin(pwmPin), maxVoltage(maxVoltage), initialized(false)
{
    // Only support pin 9 for now
    if (pin != 9)
    {
        // Could add error handling here
        return;
    }
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
        // Set pin mode to OUTPUT
        pinMode(pin, OUTPUT);

        // Stop Timer1 and reset control registers
        TCCR1A = 0;
        TCCR1B = 0;
        TCNT1 = 0;

        // Set Fast PWM mode with ICR1 as TOP
        TCCR1A = (1 << COM1A1);               // Non-inverting mode on OC1A
        TCCR1A |= (1 << WGM11);               // Fast PWM part 1
        TCCR1B = (1 << WGM13) | (1 << WGM12); // Fast PWM part 2
        TCCR1B |= (1 << CS10);                // No prescaler, clock source is system clock

        initialized = true;
    }

    // 16MHz / freq_hz = timer top value
    // Subtract 1 from result because counter goes from 0 to TOP
    unsigned long calculated_top_long = (F_CPU / freq_hz) - 1;
    unsigned int top;
    if (calculated_top_long > 65535)
    {
        // Handle error: frequency too low for 16-bit timer at this F_CPU
        // Potentially clamp to 65535 or return error
        top = 65535;
    }
    else if (calculated_top_long < 1)
    { // Min TOP for some modes might be higher
        // Handle error: frequency too high, poor resolution
        // Potentially clamp to a minimum sensible value or return error
        top = 1; // Example, actual minimum depends on desired behavior
    }
    else
    {
        top = static_cast<unsigned int>(calculated_top_long);
    }
    cli(); // Disable interrupts
    ICR1 = top;
    sei(); // Enable interrupts
}

void PWM::setDutyCycle(float duty_cycle)
{
    if (!initialized || pin != 9)
        return;

    // Calculate OCR1A value based on normalized duty cycle (0.0 - 1.0)
    unsigned int ocr_val = static_cast<unsigned int>(ICR1 * duty_cycle);

    cli(); // Disable interrupts to safely set OCR1A
    OCR1A = ocr_val;
    sei(); // Enable interrupts
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
