#include "sequence.h"

Sequence::Sequence(int maxSequenceLength)
    : maxNotes(maxSequenceLength), currentNumNotes(0)
{
    // Allocate memory for the notes array
    notes = new int[maxNotes];

    // Initialize all notes to 0
    for (int i = 0; i < maxNotes; i++)
    {
        notes[i] = 0;
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
    }
    currentNumNotes = 0;
}
