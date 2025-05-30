/**
 * Example patterns for the Arduino Sequencer
 *
 * This file contains predefined musical patterns that can be loaded
 * into the sequencer for different musical styles and genres.
 */

#ifndef EXAMPLE_PATTERNS_H
#define EXAMPLE_PATTERNS_H

#include "core/pattern.h"

namespace ExamplePatterns
{

    // Major scale patterns
    extern int majorScale_C2_C3[];
    extern const int majorScale_C2_C3_length;

    extern int pentatonicScale_C2[];
    extern const int pentatonicScale_C2_length;

    // Arpeggios
    extern int cMajorArpeggio[];
    extern const int cMajorArpeggio_length;

    extern int aMinorArpeggio[];
    extern const int aMinorArpeggio_length;

    // Rhythmic patterns
    extern int fourOnFloor[];
    extern const int fourOnFloor_length;

    extern int acidBassline[];
    extern const int acidBassline_length;

    // Factory functions
    Pattern *createMajorScale();
    Pattern *createPentatonicScale();
    Pattern *createCMajorArpeggio();
    Pattern *createAMinorArpeggio();
    Pattern *createFourOnFloor();
    Pattern *createAcidBassline();

    // Utility functions
    Pattern *getRandomPattern();
    const char *getPatternName(int index);
    int getPatternCount();
}

#endif
