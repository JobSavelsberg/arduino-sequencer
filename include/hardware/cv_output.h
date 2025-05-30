#ifndef CV_OUTPUT_H
#define CV_OUTPUT_H

#include <Arduino.h>

/**
 * Musical CV (Control Voltage) output
 * Handles 1V/octave standard for modular synthesizers
 * All functions work with musical concepts (notes, octaves)
 */

// CV output assignments
extern const int CV_PITCH_PIN;      // Primary pitch CV output (pin 9)
extern const int CV_MODULATION_PIN; // Modulation CV output (pin 10)

/**
 * Initialize the CV output system
 */
void initCVOutput();

/**
 * Set pitch CV output to a specific MIDI note using 1V/octave standard
 * @param midiNote MIDI note number (0-127)
 * @param baseNote MIDI note that corresponds to 0V (default: 36 = C2)
 * @param maxVoltage Maximum output voltage (default: 5.0V)
 */
void setCVNote(int midiNote, int baseNote = 36, float maxVoltage = 5.0);

/**
 * Set pitch CV output to a specific pitch offset from base note
 * @param semitones Number of semitones above base note (can be fractional for microtones)
 * @param baseNote MIDI note that corresponds to 0V (default: 36 = C2)
 */
void setCVPitch(float semitones, int baseNote = 36);

/**
 * Set modulation CV output (0-5V range, useful for filter cutoff, VCA, etc.)
 * @param modulationLevel Modulation amount (0.0 to 1.0)
 * @param maxVoltage Maximum modulation voltage (default: 5.0V)
 */
void setCVModulation(float modulationLevel, float maxVoltage = 5.0);

/**
 * Turn off pitch CV output (set to base note = 0V)
 */
void cvPitchOff();

/**
 * Turn off modulation CV output (set to 0V)
 */
void cvModulationOff();

/**
 * Turn off all CV outputs
 */
void cvAllOff();

#endif
