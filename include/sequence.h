#ifndef SEQUENCE_H
#define SEQUENCE_H

class Sequence
{
private:
    int *notes;           // Array of MIDI note numbers
    float *gateDurations; // Array of gate durations (0.0 to 1.0, as fraction of note duration)
    int maxNotes;         // Maximum number of notes the sequence can hold
    int currentNumNotes;  // Current number of notes in the sequence

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
    void randomize(int rootNote = 36, int octaves = 3, int scaleType = 0); // Randomize notes from a scale

    // Gate duration operations
    void setGateDuration(int stepIndex, float duration); // duration: 0.0 to 1.0
    float getGateDuration(int stepIndex);
    void setGateDurations(float *durations, int length);
};

#endif // SEQUENCE_H
