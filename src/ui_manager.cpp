#include "ui_manager.h"
#include <stdio.h>

const float UIManager::BPM_DISPLAY_DURATION = 3.0f;
const float UIManager::SCALE_DISPLAY_DURATION = 3.0f;

// Forward declaration for utility functions
void midiNoteToString(int midiNote, char *noteStr);
void scaleTypeToString(int scaleType, char *scaleStr);

UIManager::UIManager(Display *display)
    : display(display), lastBpmChangeTime(0.0f), lastScaleChangeTime(0.0f), lastScaleType(-1), totalTime(0.0f)
{
}

void UIManager::update(float deltaTime)
{
    totalTime += deltaTime;
}

bool UIManager::isInitialized() const
{
    return display && display->isInitialized();
}

void UIManager::notifyBpmChanged()
{
    lastBpmChangeTime = totalTime;
}

void UIManager::notifyScaleChanged(int scaleType)
{
    if (scaleType != lastScaleType)
    {
        lastScaleType = scaleType;
        lastScaleChangeTime = totalTime;
    }
}

void UIManager::draw(const Sequence &sequence, const SequencePlayer &player, int currentScaleType)
{
    if (!isInitialized())
        return;

    auto &u8g2 = display->getU8g2();
    u8g2.firstPage();
    do
    {
        drawBpmDisplay(player);
        drawScaleDisplay(currentScaleType);
        drawSequenceVisualization(sequence, player);
        drawStepIndicator(player, sequence);
        drawCurrentNote(sequence, player);
    } while (u8g2.nextPage());
}

void UIManager::drawBpmDisplay(const SequencePlayer &player)
{
    auto &u8g2 = display->getU8g2();

    if (totalTime - lastBpmChangeTime <= BPM_DISPLAY_DURATION)
    {
        char bpmStr[16];
        char bpmValue[8];
        dtostrf(player.getBpm(), 0, 1, bpmValue);
        sprintf(bpmStr, "BPM: %s", bpmValue);
        u8g2.drawStr(1, 8, bpmStr);
    }
}

void UIManager::drawScaleDisplay(int currentScaleType)
{
    auto &u8g2 = display->getU8g2();

    notifyScaleChanged(currentScaleType);

    // Don't show scale when BPM is shown to avoid overlap
    if (totalTime - lastScaleChangeTime <= SCALE_DISPLAY_DURATION &&
        totalTime - lastBpmChangeTime > BPM_DISPLAY_DURATION)
    {
        char scaleStr[16];
        char scaleDisplayStr[24];
        scaleTypeToString(currentScaleType, scaleStr);
        sprintf(scaleDisplayStr, "Scale: %s", scaleStr);
        u8g2.drawStr(1, 8, scaleDisplayStr);
    }
}

void UIManager::drawSequenceVisualization(const Sequence &sequence, const SequencePlayer &player)
{
    auto &u8g2 = display->getU8g2();

    const int SEQ_START_X = 0;
    const int SEQ_START_Y = 10;
    const int SEQ_HEIGHT = 64 - 20;
    const int STEP_WIDTH = 128 / sequence.getLength();

    // Find note range
    int lowestNote = 127;
    int highestNote = 0;

    for (int i = 0; i < sequence.getLength(); i++)
    {
        int note = sequence.getNote(i);
        if (note < lowestNote || lowestNote == 127)
            lowestNote = note;
        if (note > highestNote || highestNote == 0)
            highestNote = note;
    }

    // Draw sequence steps
    for (int i = 0; i < sequence.getLength(); i++)
    {
        int x = SEQ_START_X + (i * STEP_WIDTH);
        int note = sequence.getNote(i);
        float gateDuration = sequence.getGateDuration(i);

        // Map note to height
        int noteHeight = map(note, lowestNote, highestNote, 3, SEQ_HEIGHT);
        int y = SEQ_START_Y + SEQ_HEIGHT - noteHeight;

        // Calculate gate width
        int gateWidth = (int)(STEP_WIDTH * gateDuration);
        if (gateWidth < 1)
            gateWidth = 1;

        // Draw rectangle - filled if current step, outline if not
        if (i == player.getCurrentStep())
        {
            u8g2.drawBox(x, y, gateWidth, noteHeight);
        }
        else
        {
            u8g2.drawFrame(x, y, gateWidth, noteHeight);
        }
    }
}

void UIManager::drawStepIndicator(const SequencePlayer &player, const Sequence &sequence)
{
    auto &u8g2 = display->getU8g2();

    char stepStr[16];
    sprintf(stepStr, "%d/%d", player.getCurrentStep() + 1, sequence.getLength());
    u8g2.drawStr(128 - 24, 64, stepStr);
}

void UIManager::drawCurrentNote(const Sequence &sequence, const SequencePlayer &player)
{
    auto &u8g2 = display->getU8g2();

    char noteStr[16];
    char noteName[8];
    midiNoteToString(sequence.getNote(player.getCurrentStep()), noteName);
    sprintf(noteStr, "%s", noteName);
    u8g2.drawStr(0, 64, noteStr);
}
