#include "utils/led_manager.h"
#include "hardware/hardware.h"

LedManager::LedManager()
{
    // Initialize LED states
    for (int i = 0; i < LED_COUNT; i++)
    {
        ledStates[i].isOn = false;
        ledStates[i].onTime = 0;
        ledStates[i].duration = 0;
        ledStates[i].isTemporary = false;
    }
}

int LedManager::getLedIndex(int ledPin) const
{
    if (ledPin == LED_A)
        return 0;
    if (ledPin == LED_B)
        return 1;
    return -1; // Invalid pin
}

void LedManager::setLed(int ledPin, bool state)
{
    int index = getLedIndex(ledPin);
    if (index < 0)
        return;

    ledStates[index].isOn = state;
    ledStates[index].isTemporary = false;
    setLED(ledPin, state);
}

void LedManager::setLedTemporary(int ledPin, unsigned long duration)
{
    int index = getLedIndex(ledPin);
    if (index < 0)
        return;

    ledStates[index].isOn = true;
    ledStates[index].onTime = millis();
    ledStates[index].duration = duration;
    ledStates[index].isTemporary = true;
    setLED(ledPin, true);
}

void LedManager::clearAllLeds()
{
    for (int i = 0; i < LED_COUNT; i++)
    {
        ledStates[i].isOn = false;
        ledStates[i].isTemporary = false;
    }
    clearAllLEDs(); // Call hardware function
}

void LedManager::indicateBeat(int ledPin, unsigned long duration)
{
    setLedTemporary(ledPin, duration);
}

void LedManager::indicateDownbeat(int ledPin, unsigned long duration)
{
    // Downbeat gets longer duration
    setLedTemporary(ledPin, duration * 2);
}

bool LedManager::isLedOn(int ledPin) const
{
    int index = getLedIndex(ledPin);
    if (index < 0)
        return false;
    return ledStates[index].isOn;
}

void LedManager::update(unsigned long currentTime)
{
    updateLed(LED_A, currentTime);
    updateLed(LED_B, currentTime);
}

void LedManager::updateLed(int ledPin, unsigned long currentTime)
{
    int index = getLedIndex(ledPin);
    if (index < 0)
        return;

    LedState &state = ledStates[index];

    if (state.isOn && state.isTemporary)
    {
        if (currentTime - state.onTime >= state.duration)
        {
            state.isOn = false;
            state.isTemporary = false;
            setLED(ledPin, false);
        }
    }
}
