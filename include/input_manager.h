#pragma once

#include "hardware/pot.h"
#include "hardware/button.h"
#include "sequence.h"
#include "sequence_player.h"

/**
 * @brief Manages user input handling and mode-specific logic
 */
class InputManager
{
private:
    Pot *timingPot;
    Pot *pitchPot;
    Pot *modulationPot;
    Button *playButton;
    Button *leftButton;
    Button *rightButton;

    int currentTranspose;
    bool lastBothPressed;
    bool justReleasedBoth;

    void handlePlayModeInput(Sequence &sequence, SequencePlayer &player, float deltaTime);
    void handleEditModeInput(Sequence &sequence, SequencePlayer &player, float deltaTime);
    void handleButtonCombinations(Sequence &sequence, SequencePlayer &player);
    void handlePlayButton(SequencePlayer &player, Sequence &sequence);

public:
    InputManager(Pot *timingPot, Pot *pitchPot, Pot *modulationPot,
                 Button *playButton, Button *leftButton, Button *rightButton);

    void update(float deltaTime, Sequence &sequence, SequencePlayer &player);
    int getCurrentScaleType() const;
    bool hasBpmChanged() const;
    bool hasScaleChanged() const;
};
