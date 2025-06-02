#ifndef SEQUENCE_PLAYER_H
#define SEQUENCE_PLAYER_H

#include "sequence.h"

// Callback function type for step events
typedef void (*StepCallback)(int currentStep, int currentNote, unsigned long noteDuration);

class SequencePlayer
{
private:
    Sequence *sequence;         // Pointer to the sequence being played
    int currentStepIndex;       // Current step in the sequence
    unsigned long lastStepTime; // Timestamp of the last step
    bool isPlaying;             // Whether the player is currently playing
    float bpm;                  // Current beats per minute
    StepCallback stepCallback;  // Callback function for step events

public:
    // Constructor
    SequencePlayer(Sequence *seq, float initialBpm = 120.0);

    // Playback control
    void start();
    void stop();
    void reset();
    bool getIsPlaying();

    // Update function - call this regularly to handle timing
    void update();

    // Step management
    int getCurrentStep();
    void setCurrentStep(int step);

    // Timing
    void setBpm(float newBpm);
    float getBpm();
    unsigned long getNoteDuration();

    // Get current note
    int getCurrentNote();

    // Callback management
    void onStepAdvance(StepCallback callback);

    // Sequence management
    void setSequence(Sequence *seq);
    Sequence *getSequence();
};

#endif // SEQUENCE_PLAYER_H
