#ifndef LED_H
#define LED_H

#include <Arduino.h>

class LED
{
private:
    int pin;
    float blinkStart;    // Time accumulated since blink started in seconds
    float blinkDuration; // Duration of blink in seconds
    bool isBlinking;

public:
    LED(int ledPin);
    void on();
    void off();
    void blink(float duration);
    void update(float dt); // dt is delta time in seconds
};

#endif // LED_H
