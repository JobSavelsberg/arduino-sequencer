#pragma once

#include "hardware/display.h"
#include "sequence.h"
#include "sequence_player.h"

/**
 * @brief Manages UI display logic and state
 */
class UIManager
{
private:
    Display *display;
    float lastBpmChangeTime;
    float lastScaleChangeTime;
    int lastScaleType;
    float totalTime;

    static const float BPM_DISPLAY_DURATION;
    static const float SCALE_DISPLAY_DURATION;

    void drawBpmDisplay(const SequencePlayer &player);
    void drawScaleDisplay(int currentScaleType);
    void drawSequenceVisualization(const Sequence &sequence, const SequencePlayer &player);
    void drawStepIndicator(const SequencePlayer &player, const Sequence &sequence);
    void drawCurrentNote(const Sequence &sequence, const SequencePlayer &player);

public:
    UIManager(Display *display);

    void update(float deltaTime);
    void draw(const Sequence &sequence, const SequencePlayer &player, int currentScaleType);
    void notifyBpmChanged();
    void notifyScaleChanged(int scaleType);
    bool isInitialized() const;
};
