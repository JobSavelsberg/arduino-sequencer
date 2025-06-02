#ifndef LED_H
#define LED_H

#include <Arduino.h>

class LED
{
private:
    int pin;
    double blinkStart;    // Time accumulated since blink started in seconds
    double blinkDuration; // Duration of blink in seconds
    bool isBlinking;

public:
    LED(int ledPin);
    void on();
    void off();
    void blink(double duration);
    void update(double dt); // dt is delta time in seconds
};

#endif // LED_H
