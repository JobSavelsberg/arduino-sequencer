/**
 * Example patterns for the Arduino Sequencer
 * Implementation of predefined musical patterns
 */

#include "example_patterns.h"

namespace ExamplePatterns
{

    // Major scale C2 to C3 (default pattern)
    int majorScale_C2_C3[] = {36, 40, 41, 43, 45, 38, 41, 48, 45, 50, 52, 59, 60, 53, 57, 55};
    const int majorScale_C2_C3_length = sizeof(majorScale_C2_C3) / sizeof(majorScale_C2_C3[0]);

    // Pentatonic scale starting from C2
    int pentatonicScale_C2[] = {36, 38, 40, 43, 45, 48, 50, 52};
    const int pentatonicScale_C2_length = sizeof(pentatonicScale_C2) / sizeof(pentatonicScale_C2[0]);

    // C Major arpeggio (C-E-G-C)
    int cMajorArpeggio[] = {36, 40, 43, 48, 52, 55, 60, 64};
    const int cMajorArpeggio_length = sizeof(cMajorArpeggio) / sizeof(cMajorArpeggio[0]);

    // A Minor arpeggio (A-C-E-A)
    int aMinorArpeggio[] = {33, 36, 40, 45, 48, 52, 57, 60};
    const int aMinorArpeggio_length = sizeof(aMinorArpeggio) / sizeof(aMinorArpeggio[0]);

    // Four-on-the-floor kick pattern (using C2 for emphasis)
    int fourOnFloor[] = {36, 36, 36, 36};
    const int fourOnFloor_length = sizeof(fourOnFloor) / sizeof(fourOnFloor[0]);

    // Acid bassline pattern
    int acidBassline[] = {36, 39, 41, 44, 43, 39, 36, 34, 36, 41, 43, 46, 44, 41, 39, 36};
    const int acidBassline_length = sizeof(acidBassline) / sizeof(acidBassline[0]);

    // Factory functions
    Pattern *createMajorScale()
    {
        return new Pattern(majorScale_C2_C3, majorScale_C2_C3_length);
    }

    Pattern *createPentatonicScale()
    {
        return new Pattern(pentatonicScale_C2, pentatonicScale_C2_length);
    }

    Pattern *createCMajorArpeggio()
    {
        return new Pattern(cMajorArpeggio, cMajorArpeggio_length);
    }

    Pattern *createAMinorArpeggio()
    {
        return new Pattern(aMinorArpeggio, aMinorArpeggio_length);
    }

    Pattern *createFourOnFloor()
    {
        return new Pattern(fourOnFloor, fourOnFloor_length);
    }

    Pattern *createAcidBassline()
    {
        return new Pattern(acidBassline, acidBassline_length);
    }

    // Pattern names for display
    static const char *patternNames[] = {
        "Major Scale",
        "Pentatonic",
        "C Major Arp",
        "A Minor Arp",
        "Four on Floor",
        "Acid Bass"};

    Pattern *getRandomPattern()
    {
        int index = random(getPatternCount());
        switch (index)
        {
        case 0:
            return createMajorScale();
        case 1:
            return createPentatonicScale();
        case 2:
            return createCMajorArpeggio();
        case 3:
            return createAMinorArpeggio();
        case 4:
            return createFourOnFloor();
        case 5:
            return createAcidBassline();
        default:
            return createMajorScale();
        }
    }

    const char *getPatternName(int index)
    {
        if (index >= 0 && index < getPatternCount())
        {
            return patternNames[index];
        }
        return "Unknown";
    }

    int getPatternCount()
    {
        return sizeof(patternNames) / sizeof(patternNames[0]);
    }
}
