#include "utils/parameter_manager.h"
#include "hardware/hardware.h"

ParameterManager::ParameterManager() : lastBpm(120.0), lastModulation(0.0), lastUpdateTime(0)
{
    // Initialize button states
    for (int i = 0; i < 3; i++)
    {
        buttons[i].lastState = false;
        buttons[i].currentState = false;
        buttons[i].lastDebounceTime = 0;
        buttons[i].debounceDelay = DEBOUNCE_DELAY;
    }
}

int ParameterManager::getButtonIndex(int buttonPin)
{
    // Map button pins to array indices
    if (buttonPin == BUT_A)
        return 0;
    if (buttonPin == BUT_LEFT)
        return 1;
    if (buttonPin == BUT_RIGHT)
        return 2;
    return -1; // Invalid pin
}

void ParameterManager::updateButton(int buttonPin, unsigned long currentTime)
{
    int index = getButtonIndex(buttonPin);
    if (index < 0)
        return;

    bool reading = readButton(buttonPin);

    if (reading != buttons[index].lastState)
    {
        buttons[index].lastDebounceTime = currentTime;
    }

    if ((currentTime - buttons[index].lastDebounceTime) > buttons[index].debounceDelay)
    {
        if (reading != buttons[index].currentState)
        {
            buttons[index].currentState = reading;
        }
    }

    buttons[index].lastState = reading;
}

bool ParameterManager::isButtonPressed(int buttonPin)
{
    int index = getButtonIndex(buttonPin);
    if (index < 0)
        return false;
    return buttons[index].currentState;
}

bool ParameterManager::isButtonJustPressed(int buttonPin)
{
    int index = getButtonIndex(buttonPin);
    if (index < 0)
        return false;

    static bool lastStates[3] = {false, false, false};
    bool current = buttons[index].currentState;
    bool result = current && !lastStates[index];
    lastStates[index] = current;
    return result;
}

bool ParameterManager::isButtonJustReleased(int buttonPin)
{
    int index = getButtonIndex(buttonPin);
    if (index < 0)
        return false;

    static bool lastStates[3] = {false, false, false};
    bool current = buttons[index].currentState;
    bool result = !current && lastStates[index];
    lastStates[index] = current;
    return result;
}

float ParameterManager::readBpm(int potPin, float minBpm, float maxBpm)
{
    float rawBpm = readLogPot(potPin, minBpm, maxBpm, 1.0);
    lastBpm = smoothParameter(rawBpm, lastBpm, 0.2); // More responsive for BPM
    return lastBpm;
}

float ParameterManager::readModulation(int potPin, float minLevel, float maxLevel)
{
    float rawModulation = readLogPot(potPin, minLevel, maxLevel, 0.1);
    lastModulation = smoothParameter(rawModulation, lastModulation, 0.1);
    return lastModulation;
}

void ParameterManager::update(unsigned long currentTime)
{
    if (currentTime - lastUpdateTime >= UPDATE_INTERVAL)
    {
        updateButton(BUT_A, currentTime);
        updateButton(BUT_LEFT, currentTime);
        updateButton(BUT_RIGHT, currentTime);
        lastUpdateTime = currentTime;
    }
}

float ParameterManager::smoothParameter(float newValue, float lastValue, float smoothingFactor)
{
    return lastValue + smoothingFactor * (newValue - lastValue);
}
