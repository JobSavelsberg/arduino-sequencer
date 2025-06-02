#ifndef POT_H
#define POT_H

#include <Arduino.h>

class Pot
{
private:
    int pin;
    int lastRawValue;
    float lastMappedValue;
    double timeAccumulator; // Time accumulated since last read in seconds
    double readInterval;    // Read interval in seconds
    int smoothingWindow;
    int *readings;
    int readIndex;
    int total;
    bool useSmoothing;

public:
    Pot(int analogPin, double intervalSeconds = 0.05, int smoothingSamples = 10);
    ~Pot();
    void update(double dt); // dt is delta time in seconds
    int getRawValue();
    float getLinearValue(float minValue = 0.0, float maxValue = 1.0);
    float getLogValue(float minValue = 0.0, float maxValue = 1.0, float curve = 2.0);
    bool hasChanged(int threshold = 5);
    void setReadInterval(double intervalSeconds);
};

#endif // POT_H
