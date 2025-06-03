#pragma once

#include "hardware/pwm.h"
#include "hardware/gate.h"
#include "hardware/led.h"

/**
 * @brief Manages CV output and hardware control
 */
class CVManager
{
public:
    static const float MAX_VOLTAGE;
    static const int BASE_0V_NOTE;

private:
    PWM *cvOutPitch;
    Gate *cvGate;
    LED *leftLED;
    LED *rightLED;

public:
    CVManager(PWM *cvOutPitch, Gate *cvGate, LED *leftLED, LED *rightLED);

    void setCVNote(int midiNote);
    void triggerGate(float duration);
    void updateBeatIndicators(int currentStep, float noteDuration);
    void update(float deltaTime);
};
