#pragma once

/**
 * @brief Utility functions for note and scale conversions
 */

/**
 * @brief Convert MIDI note number to note name string (e.g., "C#3")
 * @param midiNote MIDI note number (0-127)
 * @param noteStr Output string buffer (must be at least 4 chars)
 */
void midiNoteToString(int midiNote, char *noteStr);

/**
 * @brief Convert scale type number to scale name string
 * @param scaleType Scale type number (0-9)
 * @param scaleStr Output string buffer (must be at least 16 chars)
 */
void scaleTypeToString(int scaleType, char *scaleStr);
