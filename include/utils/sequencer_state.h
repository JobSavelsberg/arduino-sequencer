#ifndef SEQUENCER_STATE_H
#define SEQUENCER_STATE_H

#include <Arduino.h>

/**
 * Structure to hold sequencer state information
 */
struct SequencerState
{
    int currentNoteIndex;           // Current note being played (0-based)
    int numNotes;                   // Total number of notes in sequence
    int *midiNotes;                 // Array of MIDI note values
    float bpm;                      // Current beats per minute
    int baseNote;                   // Base note for voltage calculation (typically 36 for C2)
    unsigned long previousNoteTime; // Timestamp of the last note played
    unsigned long ledOnTime;        // Timestamp when LED was turned on
    bool ledState;                  // Current LED state
    float modulationLevel;          // Current modulation level (0.0 to 1.0)
};

#endif
