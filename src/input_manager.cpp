#include "input_manager.h"
#include "cv_manager.h"

InputManager::InputManager(Pot *timingPot, Pot *pitchPot, Pot *modulationPot,
                           Button *playButton, Button *leftButton, Button *rightButton)
    : timingPot(timingPot), pitchPot(pitchPot), modulationPot(modulationPot), playButton(playButton), leftButton(leftButton), rightButton(rightButton), currentTranspose(0), lastBothPressed(false), justReleasedBoth(false)
{
}

void InputManager::update(float deltaTime, Sequence &sequence, SequencePlayer &player)
{
    // Update all inputs
    timingPot->update(deltaTime);
    pitchPot->update(deltaTime);
    modulationPot->update(deltaTime);
    playButton->update(deltaTime);
    leftButton->update(deltaTime);
    rightButton->update(deltaTime);

    // Handle play button
    handlePlayButton(player, sequence);

    // Handle button combinations first (highest priority)
    handleButtonCombinations(sequence, player);

    // Handle mode-specific input
    if (player.getIsPlaying())
    {
        handlePlayModeInput(sequence, player, deltaTime);
    }
    else
    {
        handleEditModeInput(sequence, player, deltaTime);
    }
}

void InputManager::handlePlayButton(SequencePlayer &player, Sequence &sequence)
{
    if (playButton->wasPressed())
    {
        if (player.getIsPlaying())
        {
            player.stop();
            // Reset transpose when stopping
            if (currentTranspose != 0)
            {
                sequence.transpose(-currentTranspose);
                currentTranspose = 0;
            }
        }
        else
        {
            player.start();
            currentTranspose = 0;
        }
    }
}

void InputManager::handleButtonCombinations(Sequence &sequence, SequencePlayer &player)
{
    bool bothCurrentlyPressed = leftButton->isPressed() && rightButton->isPressed();

    if (bothCurrentlyPressed && !lastBothPressed)
    {
        // Both buttons just pressed - randomize sequence
        int scaleType = getCurrentScaleType();
        sequence.randomize(CVManager::BASE_0V_NOTE, 3, scaleType);
    }

    // Track when both buttons were just released
    if (lastBothPressed && !bothCurrentlyPressed)
    {
        justReleasedBoth = true;
    }
    else if (!leftButton->isPressed() && !rightButton->isPressed())
    {
        justReleasedBoth = false;
    }

    lastBothPressed = bothCurrentlyPressed;
}

void InputManager::handlePlayModeInput(Sequence &sequence, SequencePlayer &player, float deltaTime)
{
    // BPM control with timing pot
    float newBpm = timingPot->getLogValue(60.0, 500.0, 2.0);
    if (timingPot->hasChanged(10))
    {
        player.setBpm(newBpm);
    }

    // Transpose with pitch pot
    int newTranspose = (int)pitchPot->getLinearValue(-12, 12);
    if (pitchPot->hasChanged(5))
    {
        int transposeChange = newTranspose - currentTranspose;
        if (transposeChange != 0)
        {
            sequence.transpose(transposeChange);
            currentTranspose = newTranspose;
        }
    }

    // Sequence length control with left/right buttons
    if (!lastBothPressed && !justReleasedBoth)
    {
        if (leftButton->wasReleased())
        {
            // Decrease sequence length
            int currentLength = sequence.getLength();
            if (currentLength > 1)
            {
                sequence.setLength(currentLength - 1);
                if (player.getCurrentStep() >= sequence.getLength())
                {
                    player.setCurrentStep(0);
                }
            }
        }

        if (rightButton->wasReleased())
        {
            // Increase sequence length
            int currentLength = sequence.getLength();
            if (currentLength < sequence.getMaxLength())
            {
                sequence.setLength(currentLength + 1);
            }
        }
    }
}

void InputManager::handleEditModeInput(Sequence &sequence, SequencePlayer &player, float deltaTime)
{
    // Pitch range control with modulation pot
    float pitchRange = modulationPot->getLinearValue(1.0, 5.0);
    int octaveRange = (int)(pitchRange * 12);

    // Note editing with pitch pot
    int newNote = (int)pitchPot->getLinearValue(CVManager::BASE_0V_NOTE, CVManager::BASE_0V_NOTE + octaveRange);
    if (pitchPot->hasChanged(5))
    {
        sequence.setNote(player.getCurrentStep(), newNote);
    }

    // Gate duration control with timing pot
    float newGateDuration = timingPot->getLinearValue(0.1, 1.0);
    if (timingPot->hasChanged(10))
    {
        sequence.setGateDuration(player.getCurrentStep(), newGateDuration);
    }

    // Step navigation with left/right buttons
    if (!lastBothPressed && !justReleasedBoth)
    {
        if (leftButton->wasReleased())
        {
            int currentStep = player.getCurrentStep();
            int newStep = (currentStep - 1 + sequence.getLength()) % sequence.getLength();
            player.setCurrentStep(newStep);
        }

        if (rightButton->wasReleased())
        {
            int currentStep = player.getCurrentStep();
            int newStep = (currentStep + 1) % sequence.getLength();
            player.setCurrentStep(newStep);
        }
    }
}

int InputManager::getCurrentScaleType() const
{
    return (int)modulationPot->getLinearValue(0, 9.99);
}

bool InputManager::hasBpmChanged() const
{
    return timingPot->hasChanged(10);
}

bool InputManager::hasScaleChanged() const
{
    static int lastScaleType = -1;
    int currentScaleType = getCurrentScaleType();
    bool changed = (currentScaleType != lastScaleType);
    lastScaleType = currentScaleType;
    return changed;
}
