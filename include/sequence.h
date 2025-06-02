#ifndef SEQUENCE_H
#define SEQUENCE_H

class Sequence
{
private:
    int *notes;          // Array of MIDI note numbers
    int maxNotes;        // Maximum number of notes the sequence can hold
    int currentNumNotes; // Current number of notes in the sequence

public:
    // Constructor and destructor
    Sequence(int maxSequenceLength);
    ~Sequence();

    // Basic sequence operations
    void setNote(int stepIndex, int midiNote);
    void setNotes(int *midiNotes, int length);
    int getNote(int stepIndex);
    void setLength(int length);
    int getLength();
    int getMaxLength();
    void clear();
    void transpose(int semitones);
};

#endif // SEQUENCE_H
