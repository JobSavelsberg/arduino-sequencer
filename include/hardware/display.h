#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>

class Display
{
private:
    U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2; // Use page buffer mode instead of full buffer
    bool initialized;

public:
    // Display constants
    static const int SCREEN_WIDTH = 128;
    static const int SCREEN_HEIGHT = 64;

    Display();
    void setup(unsigned long i2cSpeed = 400000);

    // Direct access to U8g2 instance for drawing operations
    U8G2_SSD1306_128X64_NONAME_1_HW_I2C &getU8g2() { return u8g2; }

    bool isInitialized() const { return initialized; }
};

#endif // DISPLAY_H
