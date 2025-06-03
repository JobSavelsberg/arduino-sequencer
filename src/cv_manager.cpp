#include "cv_manager.h"

const float CVManager::MAX_VOLTAGE = 5.0f; // Arduino's typical maximum voltage output
const int CVManager::BASE_0V_NOTE = 36;    // C2

CVManager::CVManager(PWM *cvOutPitch, Gate *cvGate, LED *leftLED, LED *rightLED)
    : cvOutPitch(cvOutPitch), cvGate(cvGate), leftLED(leftLED), rightLED(rightLED)
{
}

void CVManager::setCVNote(int midiNote)
{
    float voltage = (midiNote - BASE_0V_NOTE) / 12.0f;
    float clampedVoltage = constrain(voltage, 0.0f, MAX_VOLTAGE);
    float dutyCycle = clampedVoltage / MAX_VOLTAGE;
    cvOutPitch->setDutyCycle(dutyCycle);
}

void CVManager::triggerGate(float duration)
{
    cvGate->trigger(duration);
}

void CVManager::updateBeatIndicators(int currentStep, float noteDuration)
{
    float rightBlinkDuration = noteDuration / 2;
    float leftBlinkDuration = noteDuration;

    rightLED->blink(rightBlinkDuration);

    if (currentStep == 0)
    {
        leftLED->blink(leftBlinkDuration);
    }
}

void CVManager::update(float deltaTime)
{
    leftLED->update(deltaTime);
    rightLED->update(deltaTime);
    cvGate->update(deltaTime);
}
