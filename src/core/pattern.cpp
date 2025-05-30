#include "core/pattern.h"
#include <string.h>

Pattern::Pattern(int *noteArray, int noteCount)
    : notes(noteArray), length(noteCount), capacity(noteCount), ownsMemory(false)
{
}

Pattern::Pattern(int maxNotes)
    : length(0), capacity(maxNotes), ownsMemory(true)
{
    notes = new int[capacity];
}

Pattern::~Pattern()
{
    if (ownsMemory && notes)
    {
        delete[] notes;
    }
}

Pattern::Pattern(const Pattern &other)
    : length(other.length), capacity(other.capacity), ownsMemory(true)
{
    notes = new int[capacity];
    memcpy(notes, other.notes, length * sizeof(int));
}

Pattern &Pattern::operator=(const Pattern &other)
{
    if (this != &other)
    {
        if (ownsMemory && notes)
        {
            delete[] notes;
        }

        length = other.length;
        capacity = other.capacity;
        ownsMemory = true;
        notes = new int[capacity];
        memcpy(notes, other.notes, length * sizeof(int));
    }
    return *this;
}

int Pattern::getNote(int index) const
{
    if (!isValidIndex(index))
    {
        return 0; // Return safe default
    }
    return notes[index];
}

int Pattern::getLength() const
{
    return length;
}

bool Pattern::setNote(int index, int midiNote)
{
    if (!isValidIndex(index) || !isValidMidiNote(midiNote))
    {
        return false;
    }
    notes[index] = midiNote;
    return true;
}

bool Pattern::addNote(int midiNote)
{
    if (!ownsMemory || length >= capacity || !isValidMidiNote(midiNote))
    {
        return false;
    }
    notes[length++] = midiNote;
    return true;
}

void Pattern::clear()
{
    if (ownsMemory)
    {
        length = 0;
    }
}

int Pattern::getNextIndex(int currentIndex) const
{
    if (length == 0)
        return 0;
    return (currentIndex + 1) % length;
}

int Pattern::getPreviousIndex(int currentIndex) const
{
    if (length == 0)
        return 0;
    return (currentIndex - 1 + length) % length;
}

bool Pattern::isValidIndex(int index) const
{
    return index >= 0 && index < length;
}

bool Pattern::isValidMidiNote(int midiNote) const
{
    return midiNote >= 0 && midiNote <= 127;
}
