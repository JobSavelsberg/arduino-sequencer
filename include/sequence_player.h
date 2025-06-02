#ifndef SEQUENCE_PLAYER_H
#define SEQUENCE_PLAYER_H

#include "sequence.h"

// Callback function type for step events
typedef void (*StepCallback)(int currentStep, int currentNote, double noteDurationSeconds);

class SequencePlayer
{
private:
    Sequence *sequence;        // Pointer to the sequence being played
    int currentStepIndex;      // Current step in the sequence
    double time;               // Time accumulated since last step in seconds
    bool isPlaying;            // Whether the player is currently playing
    float bpm;                 // Current beats per minute
    StepCallback stepCallback; // Callback function for step events

public:
    // Constructor
    SequencePlayer(Sequence *seq, float initialBpm = 120.0);

    // Playback control
    void start();
    void stop();
    void reset();
    bool getIsPlaying();

    // Update function - call this regularly to handle timing
    void update(double dt); // dt is delta time in seconds

    // Step management
    int getCurrentStep();
    void setCurrentStep(int step);

    // Timing
    void setBpm(float newBpm);
    float getBpm();
    double getNoteDurationSeconds(); // Returns note duration in seconds

    // Get current note
    int getCurrentNote();

    // Callback management
    void onStepAdvance(StepCallback callback);

    // Sequence management
    void setSequence(Sequence *seq);
    Sequence *getSequence();
};

#endif // SEQUENCE_PLAYER_H
