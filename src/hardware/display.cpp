#include "hardware/display.h"

Display::Display() : u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE), initialized(false)
{
}

void Display::setup(unsigned long i2cSpeed)
{
    // Set I2C to specified speed (default 400kHz for optimal performance)
    Wire.setClock(i2cSpeed);
    u8g2.begin();
    initialized = true;

    // Set default font
    u8g2.setFont(u8g2_font_6x10_tf);
}
