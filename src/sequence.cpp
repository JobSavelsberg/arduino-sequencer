#include <Arduino.h>
#include "sequence.h"

Sequence::Sequence(int maxSequenceLength)
    : maxNotes(maxSequenceLength), currentNumNotes(0)
{
    // Allocate memory for the notes array
    notes = new int[maxNotes];

    // Allocate memory for the gate durations array
    gateDurations = new float[maxNotes];

    // Initialize all notes to 0 and gate durations to 0.5 (50%)
    for (int i = 0; i < maxNotes; i++)
    {
        notes[i] = 0;
        gateDurations[i] = 0.5f; // Default 50% gate duration
    }
}

Sequence::~Sequence()
{
    // Clean up allocated memory
    if (notes != nullptr)
    {
        delete[] notes;
        notes = nullptr;
    }
    if (gateDurations != nullptr)
    {
        delete[] gateDurations;
        gateDurations = nullptr;
    }
}

void Sequence::setNote(int stepIndex, int midiNote)
{
    if (stepIndex >= 0 && stepIndex < maxNotes)
    {
        notes[stepIndex] = midiNote;

        // Update current length if we're setting a note beyond current length
        if (stepIndex >= currentNumNotes)
        {
            currentNumNotes = stepIndex + 1;
        }
    }
}

void Sequence::setNotes(int *midiNotes, int length)
{
    if (length > 0 && length <= maxNotes)
    {
        for (int i = 0; i < length; i++)
        {
            notes[i] = midiNotes[i];
        }
        currentNumNotes = length;
    }
}

int Sequence::getNote(int stepIndex)
{
    if (stepIndex >= 0 && stepIndex < currentNumNotes)
    {
        return notes[stepIndex];
    }
    return 0; // Return 0 for out-of-bounds
}

void Sequence::setLength(int length)
{
    if (length >= 0 && length <= maxNotes)
    {
        currentNumNotes = length;
    }
}

int Sequence::getLength()
{
    return currentNumNotes;
}

int Sequence::getMaxLength()
{
    return maxNotes;
}

void Sequence::clear()
{
    for (int i = 0; i < maxNotes; i++)
    {
        notes[i] = 0;
        gateDurations[i] = 0.5f; // Reset to default 50% gate duration
    }
    currentNumNotes = 0;
}

void Sequence::transpose(int semitones)
{
    // Find the current highest and lowest notes in the sequence
    int minNote = 127, maxNote = 0;
    bool hasNotes = false;
    for (int i = 0; i < currentNumNotes; i++)
    {
        if (notes[i] > 0) // Only consider non-zero notes
        {
            hasNotes = true;
            if (notes[i] < minNote)
                minNote = notes[i];
            if (notes[i] > maxNote)
                maxNote = notes[i];
        }
    }

    // If no notes, nothing to transpose
    if (!hasNotes)
        return;

    // CV output range constraints (BASE_0V_NOTE = 36, MAX = 96 for 5V range)
    const int CV_MIN_NOTE = 36; // C2 - corresponds to 0V
    const int CV_MAX_NOTE = 96; // C6 - corresponds to 5V (5 octaves)

    // Clamp semitones to prevent going out of usable CV range
    if (semitones > 0 && maxNote + semitones > CV_MAX_NOTE)
    {
        semitones = CV_MAX_NOTE - maxNote;
    }
    else if (semitones < 0 && minNote + semitones < CV_MIN_NOTE)
    {
        semitones = CV_MIN_NOTE - minNote;
    }

    // Apply the transposition
    for (int i = 0; i < currentNumNotes; i++)
    {
        if (notes[i] > 0) // Only transpose non-zero notes
        {
            notes[i] += semitones;
            // Clamp to CV output range
            if (notes[i] < CV_MIN_NOTE)
                notes[i] = CV_MIN_NOTE;
            else if (notes[i] > CV_MAX_NOTE)
                notes[i] = CV_MAX_NOTE;
        }
    }
}

void Sequence::setGateDuration(int stepIndex, float duration)
{
    if (stepIndex >= 0 && stepIndex < maxNotes)
    {
        // Clamp duration to valid range (0.0 to 1.0)
        gateDurations[stepIndex] = constrain(duration, 0.0f, 1.0f);
    }
}

float Sequence::getGateDuration(int stepIndex)
{
    if (stepIndex >= 0 && stepIndex < currentNumNotes)
    {
        return gateDurations[stepIndex];
    }
    return 0.5f; // Return default 50% for out-of-bounds
}

void Sequence::setGateDurations(float *durations, int length)
{
    if (length > 0 && length <= maxNotes)
    {
        for (int i = 0; i < length; i++)
        {
            gateDurations[i] = constrain(durations[i], 0.0f, 1.0f);
        }
    }
}
