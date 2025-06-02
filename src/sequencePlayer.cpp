#include "sequencePlayer.h"
#include <Arduino.h>

SequencePlayer::SequencePlayer(Sequence *seq, float initialBpm)
    : sequence(seq), currentStepIndex(0), lastStepTime(0), isPlaying(false), bpm(initialBpm), stepCallback(nullptr)
{
}

void SequencePlayer::start()
{
    isPlaying = true;
    lastStepTime = millis();
}

void SequencePlayer::stop()
{
    isPlaying = false;
}

void SequencePlayer::reset()
{
    currentStepIndex = 0;
    lastStepTime = millis();
}

bool SequencePlayer::getIsPlaying()
{
    return isPlaying;
}

int SequencePlayer::getCurrentStep()
{
    return currentStepIndex;
}

void SequencePlayer::setCurrentStep(int step)
{
    if (sequence && step >= 0 && step < sequence->getLength())
    {
        currentStepIndex = step;
    }
}

void SequencePlayer::update()
{
    if (!isPlaying || !sequence || sequence->getLength() == 0)
    {
        return;
    }

    unsigned long currentTime = millis();
    if ((currentTime - lastStepTime) >= getNoteDuration())
    {
        // Call the callback if it's set
        if (stepCallback)
        {
            stepCallback(currentStepIndex, getCurrentNote(), getNoteDuration());
        }

        // Advance to the next step
        currentStepIndex = (currentStepIndex + 1) % sequence->getLength();
        lastStepTime = currentTime;
    }
}

void SequencePlayer::setBpm(float newBpm)
{
    if (newBpm > 0)
    {
        bpm = newBpm;
    }
}

float SequencePlayer::getBpm()
{
    return bpm;
}

unsigned long SequencePlayer::getNoteDuration()
{
    return (unsigned long)(60000.0 / bpm); // Duration of a quarter note in milliseconds
}

int SequencePlayer::getCurrentNote()
{
    if (sequence && currentStepIndex >= 0 && currentStepIndex < sequence->getLength())
    {
        return sequence->getNote(currentStepIndex);
    }
    return 0;
}

void SequencePlayer::onStepAdvance(StepCallback callback)
{
    stepCallback = callback;
}

void SequencePlayer::setSequence(Sequence *seq)
{
    sequence = seq;
    reset(); // Reset to beginning when setting new sequence
}

Sequence *SequencePlayer::getSequence()
{
    return sequence;
}
