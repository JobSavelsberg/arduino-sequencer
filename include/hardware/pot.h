#ifndef POT_H
#define POT_H

#include <Arduino.h>

class Pot
{
private:
    int pin;
    int lastRawValue;
    float lastMappedValue;
    unsigned long lastReadTime;
    unsigned long readInterval;
    int smoothingWindow;
    int *readings;
    int readIndex;
    int total;
    bool useSmoothing;

public:
    Pot(int analogPin, unsigned long interval = 50, int smoothingSamples = 10);
    ~Pot();
    void update();
    int getRawValue();
    float getLinearValue(float minValue = 0.0, float maxValue = 1.0);
    float getLogValue(float minValue = 0.0, float maxValue = 1.0, float curve = 2.0);
    bool hasChanged(int threshold = 5);
    void setReadInterval(unsigned long interval);
};

#endif // POT_H
