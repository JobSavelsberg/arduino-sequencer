#ifndef LED_H
#define LED_H

#include <Arduino.h>

class LED
{
private:
    int pin;
    unsigned long blinkStart;
    unsigned long blinkDuration;
    bool isBlinking;

public:
    LED(int ledPin);
    void on();
    void off();
    void blink(unsigned long duration);
    void update();
};

#endif // LED_H
