#ifndef GATE_H
#define GATE_H

#include <Arduino.h>

class Gate
{
private:
    int pin;
    bool isHigh;
    float gateStart;    // Time accumulated since gate started in seconds
    float gateDuration; // Duration of gate in seconds
    bool isGating;

public:
    Gate(int gatePin);
    void high();
    void low();
    void trigger(float duration);
    void update(float dt); // dt is delta time in seconds
    bool getState() const { return isHigh; }
};

#endif // GATE_H
