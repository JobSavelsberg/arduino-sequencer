#ifndef PWM_H
#define PWM_H

#include <Arduino.h>

class PWM
{
private:
    int pin;
    float maxVoltage;
    bool initialized;

public:
    PWM(int pwmPin, float maxVoltage = 5.0);
    void setup(float freq_hz = 20000.0);
    void setDutyCycle(float duty_cycle);
    void setVoltage(float voltage);
};

#endif // PWM_H