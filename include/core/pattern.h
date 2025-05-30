#ifndef PATTERN_H
#define PATTERN_H

#include <Arduino.h>

/**
 * Represents a musical pattern/sequence
 * Encapsulates note data and sequence logic
 */
class Pattern
{
private:
    int *notes;
    int length;
    int capacity;
    bool ownsMemory;

public:
    // Constructor with existing array (non-owning)
    Pattern(int *noteArray, int noteCount);

    // Constructor with capacity (owning)
    Pattern(int maxNotes);

    // Destructor
    ~Pattern();

    // Copy constructor and assignment (deep copy)
    Pattern(const Pattern &other);
    Pattern &operator=(const Pattern &other);

    // Pattern access
    int getNote(int index) const;
    int getLength() const;
    bool setNote(int index, int midiNote);
    bool addNote(int midiNote);
    void clear();

    // Pattern navigation
    int getNextIndex(int currentIndex) const;
    int getPreviousIndex(int currentIndex) const;

    // Validation
    bool isValidIndex(int index) const;
    bool isValidMidiNote(int midiNote) const;
};

#endif
