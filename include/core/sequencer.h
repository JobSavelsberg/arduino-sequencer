#ifndef SEQUENCER_H
#define SEQUENCER_H

#include <Arduino.h>
#include "pattern.h"
#include "clock.h"

/**
 * Core sequencer logic
 * Manages pattern playback and sequencer state
 */
class Sequencer
{
private:
    Pattern *currentPattern;
    Clock *clock;
    int currentNoteIndex;
    int baseNote;
    bool ownsPattern;
    bool ownsClock;

public:
    // Constructor
    Sequencer(Pattern *pattern, Clock *clockInstance, int baseNote = 36);

    // Destructor
    ~Sequencer();

    // Playback control
    void start();
    void stop();
    void reset();
    bool isPlaying() const;

    // Pattern management
    void setPattern(Pattern *newPattern);
    Pattern *getPattern() const;

    // Clock management
    void setClock(Clock *newClock);
    Clock *getClock() const;

    // Sequencer state
    int getCurrentNoteIndex() const;
    int getCurrentNote() const;
    int getBaseNote() const;
    void setBaseNote(int note);

    // Sequencer update (call in main loop)
    bool update(unsigned long currentTime); // Returns true if note changed

    // Navigation
    void stepForward();
    void stepBackward();
    void goToStep(int step);

    // Validation
    bool isValidBaseNote(int note) const;
};

#endif
