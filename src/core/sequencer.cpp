#include "core/sequencer.h"

Sequencer::Sequencer(Pattern *pattern, Clock *clockInstance, int baseNote)
    : currentPattern(pattern), clock(clockInstance), currentNoteIndex(0),
      baseNote(baseNote), ownsPattern(false), ownsClock(false)
{
}

Sequencer::~Sequencer()
{
    if (ownsPattern && currentPattern)
    {
        delete currentPattern;
    }
    if (ownsClock && clock)
    {
        delete clock;
    }
}

void Sequencer::start()
{
    if (clock)
    {
        clock->start();
    }
}

void Sequencer::stop()
{
    if (clock)
    {
        clock->stop();
    }
}

void Sequencer::reset()
{
    currentNoteIndex = 0;
    if (clock)
    {
        clock->reset();
    }
}

bool Sequencer::isPlaying() const
{
    return clock ? clock->isRunning() : false;
}

void Sequencer::setPattern(Pattern *newPattern)
{
    if (ownsPattern && currentPattern)
    {
        delete currentPattern;
        ownsPattern = false;
    }
    currentPattern = newPattern;
    currentNoteIndex = 0; // Reset to beginning when pattern changes
}

Pattern *Sequencer::getPattern() const
{
    return currentPattern;
}

void Sequencer::setClock(Clock *newClock)
{
    if (ownsClock && clock)
    {
        delete clock;
        ownsClock = false;
    }
    clock = newClock;
}

Clock *Sequencer::getClock() const
{
    return clock;
}

int Sequencer::getCurrentNoteIndex() const
{
    return currentNoteIndex;
}

int Sequencer::getCurrentNote() const
{
    if (!currentPattern || !currentPattern->isValidIndex(currentNoteIndex))
    {
        return baseNote; // Return base note as safe default
    }
    return currentPattern->getNote(currentNoteIndex);
}

int Sequencer::getBaseNote() const
{
    return baseNote;
}

void Sequencer::setBaseNote(int note)
{
    if (isValidBaseNote(note))
    {
        baseNote = note;
    }
}

bool Sequencer::update(unsigned long currentTime)
{
    if (!clock || !currentPattern || currentPattern->getLength() == 0)
    {
        return false;
    }

    if (clock->shouldTick(currentTime))
    {
        stepForward();
        return true; // Note changed
    }

    return false; // No change
}

void Sequencer::stepForward()
{
    if (currentPattern && currentPattern->getLength() > 0)
    {
        currentNoteIndex = currentPattern->getNextIndex(currentNoteIndex);
    }
}

void Sequencer::stepBackward()
{
    if (currentPattern && currentPattern->getLength() > 0)
    {
        currentNoteIndex = currentPattern->getPreviousIndex(currentNoteIndex);
    }
}

void Sequencer::goToStep(int step)
{
    if (currentPattern && currentPattern->isValidIndex(step))
    {
        currentNoteIndex = step;
    }
}

bool Sequencer::isValidBaseNote(int note) const
{
    return note >= 0 && note <= 127; // Valid MIDI note range
}
