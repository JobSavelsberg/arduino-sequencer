#include "utils.h"
#include <stdio.h>
#include <string.h>

void midiNoteToString(int midiNote, char *noteStr)
{
    const char *noteNames[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};

    int note = midiNote % 12;         // Get note within octave (0-11)
    int octave = (midiNote / 12) - 1; // Calculate octave (-1 to 9 for MIDI range 0-127)

    sprintf(noteStr, "%s%d", noteNames[note], octave);
}

void scaleTypeToString(int scaleType, char *scaleStr)
{
    const char *scaleNames[] = {
        "Major", "Minor", "Harm Min", "Lydian", "Mixolyd",
        "Dorian", "Phrygian", "Pent Maj", "Pent Min", "Blues"};

    if (scaleType >= 0 && scaleType <= 9)
    {
        strcpy(scaleStr, scaleNames[scaleType]);
    }
    else
    {
        strcpy(scaleStr, "Unknown");
    }
}
