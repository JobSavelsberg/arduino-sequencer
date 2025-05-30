#include "hardware/analog_output.h"
#include "hardware/pwm_controller.h"

// Analog output constants
const float DEFAULT_PWM_FREQUENCY = 20000.0; // PWM frequency in Hz

// Analog output pin definitions
const int ANALOG_OUT_PIN_9 = PWM_PIN_9;   // Primary CV output (typically pitch)
const int ANALOG_OUT_PIN_10 = PWM_PIN_10; // Secondary CV output (modulation)

void initAnalogOutput()
{
    // Initialize PWM system
    initPWM();
    setPWMFrequency(DEFAULT_PWM_FREQUENCY);
    setPWMDutyCycle(ANALOG_OUT_PIN_9, 0);
    setPWMDutyCycle(ANALOG_OUT_PIN_10, 0);
}

void setAnalogVoltage(int pin, float voltage, float maxVoltage)
{
    float clampedVoltage = constrain(voltage, 0.0, maxVoltage);
    float dutyCycle = clampedVoltage / maxVoltage;
    setPWMDutyCycle(pin, dutyCycle);
}

void setAnalogPercent(int pin, float percentage)
{
    float clampedPercent = constrain(percentage, 0.0, 1.0);
    setPWMDutyCycle(pin, clampedPercent);
}

void analogOff(int pin)
{
    setPWMDutyCycle(pin, 0);
}

void analogOffAll()
{
    setPWMDutyCycle(ANALOG_OUT_PIN_9, 0);
    setPWMDutyCycle(ANALOG_OUT_PIN_10, 0);
}
