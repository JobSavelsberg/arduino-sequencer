#ifndef POT_H
#define POT_H

#include <Arduino.h>

class Pot
{
private:
    int pin;
    int lastRawValue;
    int lastCheckedValue; // For hasChanged() tracking - instance variable, not static
    float lastMappedValue;
    float time;         // Time accumulated since last read in seconds
    float readInterval; // Read interval in seconds
    int smoothingWindow;
    int *readings;
    int readIndex;
    int total;
    bool useSmoothing;

public:
    Pot(int analogPin, float intervalSeconds = 0.01f, int smoothingSamples = 3);
    ~Pot();
    void update(float dt); // dt is delta time in seconds
    int getRawValue();
    float getLinearValue(float minValue = 0.0, float maxValue = 1.0);
    float getLogValue(float minValue = 0.0, float maxValue = 1.0, float curve = 2.0);
    bool hasChanged(int threshold = 5);
    void setReadInterval(float intervalSeconds);
};

#endif // POT_H
