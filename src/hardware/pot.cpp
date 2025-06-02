#include "hardware/pot.h"

Pot::Pot(int analogPin, double intervalSeconds, int smoothingSamples)
    : pin(analogPin), lastRawValue(0), lastMappedValue(0.0), time(0),
      readInterval(intervalSeconds), smoothingWindow(smoothingSamples), readIndex(0),
      total(0), useSmoothing(smoothingSamples > 1)
{
    if (useSmoothing)
    {
        readings = new int[smoothingWindow];
        for (int i = 0; i < smoothingWindow; i++)
        {
            readings[i] = 0;
        }
    }
    else
    {
        readings = nullptr;
    }
}

Pot::~Pot()
{
    if (readings != nullptr)
    {
        delete[] readings;
    }
}

void Pot::update(double dt)
{
    time += dt;

    if (time >= readInterval)
    {
        int rawReading = analogRead(pin);

        if (useSmoothing)
        {
            // Subtract the last reading
            total = total - readings[readIndex];
            // Read from the sensor
            readings[readIndex] = rawReading;
            // Add the reading to the total
            total = total + readings[readIndex];
            // Advance to the next position in the array
            readIndex = (readIndex + 1) % smoothingWindow;
            // Calculate the average
            lastRawValue = total / smoothingWindow;
        }
        else
        {
            lastRawValue = rawReading;
        }

        time = 0; // Reset the accumulator
    }
}

int Pot::getRawValue()
{
    return lastRawValue;
}

float Pot::getLinearValue(float minValue, float maxValue)
{
    float normalizedValue = (float)lastRawValue / 1023.0;
    return minValue + normalizedValue * (maxValue - minValue);
}

float Pot::getLogValue(float minValue, float maxValue, float curve)
{
    float normalizedValue = (float)lastRawValue / 1023.0;
    float logValue = pow(normalizedValue, curve);
    return minValue + logValue * (maxValue - minValue);
}

bool Pot::hasChanged(int threshold)
{
    static int lastCheckedValue = lastRawValue;
    bool changed = abs(lastRawValue - lastCheckedValue) >= threshold;
    if (changed)
    {
        lastCheckedValue = lastRawValue;
    }
    return changed;
}

void Pot::setReadInterval(double intervalSeconds)
{
    readInterval = intervalSeconds;
}
