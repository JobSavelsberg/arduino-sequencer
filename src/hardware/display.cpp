#include "hardware/display.h"
#include <Wire.h>

// Initialize the display with hardware I2C
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

// Display constants
const int SCREEN_WIDTH = 128;
const int SCREEN_HEIGHT = 64;

void initDisplay()
{
    // Set I2C to maximum speed
    Wire.setClock(400000); // 400kHz
    u8g2.begin();
}

void updateDisplay(const SequencerState &state)
{
    u8g2.clearBuffer();

    // Simple moving bar - most efficient animation
    int pos = (float)state.currentNoteIndex / (float)state.numNotes * SCREEN_WIDTH; // Calculate position based on current note index
    int size = (float)SCREEN_WIDTH / state.numNotes;
    float normalizedNotePitch = (state.midiNotes[state.currentNoteIndex] - state.baseNote) / (5 * 12.0); // Size of each bar segment
    u8g2.drawBox(pos, 64 - size - (normalizedNotePitch * 32), size, size);

    // Print note information
    // Pick a nice clear minimal font
    u8g2.setFont(u8g2_font_spleen6x12_mf);
    u8g2.setCursor(0, 10);
    u8g2.print("Note: ");
    const char *noteNames[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
    int octave = (state.midiNotes[state.currentNoteIndex] / 12) - 1; // MIDI note 0 is C-1
    int noteIndex = state.midiNotes[state.currentNoteIndex] % 12;
    u8g2.print(noteNames[noteIndex]);
    u8g2.print(octave);
    u8g2.print(" ");
    u8g2.print(state.midiNotes[state.currentNoteIndex]);
    u8g2.print(" (");
    u8g2.print((state.midiNotes[state.currentNoteIndex] - state.baseNote) / 12.0, 1); // Print voltage in V
    u8g2.print("V)");
    u8g2.setCursor(0, 30);
    u8g2.print("BPM: ");
    u8g2.print(state.bpm, 1); // Print BPM with one decimal place

    u8g2.sendBuffer();
}
