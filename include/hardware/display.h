#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <U8g2lib.h>
#include "utils/sequencer_state.h"

// Display constants
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

/**
 * Initialize the display with I2C communication
 */
void initDisplay();

/**
 * Update the display with current sequencer state
 * @param state Current sequencer state
 */
void updateDisplay(const SequencerState &state);

#endif
