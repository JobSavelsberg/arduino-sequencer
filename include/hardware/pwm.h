#ifndef PWM_H
#define PWM_H

#include <Arduino.h>

class PWM
{
private:
    int pin;
    bool initialized;

public:
    PWM(int pwmPin);
    void setup(float freq_hz = 20000.0);
    void setDutyCycle(float duty_cycle);
};

#endif // PWM_H