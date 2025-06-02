#include "sequence_player.h"

SequencePlayer::SequencePlayer(Sequence *seq, float initialBpm)
    : sequence(seq), currentStepIndex(0), time(0.0f), isPlaying(false), bpm(initialBpm), stepCallback(nullptr)
{
}

void SequencePlayer::start()
{
    isPlaying = true;
    time = 0.0f;
}

void SequencePlayer::stop()
{
    isPlaying = false;
}

void SequencePlayer::reset()
{
    currentStepIndex = 0;
    time = 0.0f;
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

void SequencePlayer::update(float dt)
{
    if (!isPlaying || !sequence || sequence->getLength() == 0)
    {
        return;
    }

    time += dt;

    if (time >= getNoteDurationSeconds())
    {
        // Advance to the next step
        currentStepIndex = (currentStepIndex + 1) % sequence->getLength();
        time = 0; // Reset accumulator for next note

        // Call the callback if it's set
        if (stepCallback)
        {
            stepCallback(currentStepIndex, getCurrentNote(), getNoteDurationSeconds());
        }
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

float SequencePlayer::getNoteDurationSeconds()
{
    return 60.0f / bpm; // Duration of a quarter note in seconds
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
