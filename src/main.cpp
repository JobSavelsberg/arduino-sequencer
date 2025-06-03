#include <Arduino.h>
#include "hardware/pwm.h"
#include "hardware/led.h"
#include "hardware/button.h"
#include "hardware/pot.h"
#include "hardware/display.h"
#include "hardware/gate.h"
#include "sequence.h"
#include "sequence_player.h"
#include "ui_manager.h"
#include "input_manager.h"
#include "cv_manager.h"
#include "utils.h"

// Hardware objects
LED leftLED(12);
LED rightLED(13);
Button playButton(2);
Button leftButton(7);
Button rightButton(4);
Pot timingPot(3, 0.01f, 1);     // 10ms read interval, no smoothing to save memory
Pot pitchPot(2, 0.01f, 1);      // 10ms read interval, no smoothing to save memory
Pot modulationPot(1, 0.01f, 1); // 10ms read interval, no smoothing to save memory
PWM cvOutPitch(9, 5.0f);        // Use literal value instead of MAX_VOLTAGE
Gate cvGate(8);
Display oledDisplay;

// Core objects
Sequence mainSequence(16);                    // 16-step sequence
SequencePlayer player(&mainSequence, 120.0f); // Player with 120 BPM

// Manager objects
UIManager uiManager(&oledDisplay);
InputManager inputManager(&timingPot, &pitchPot, &modulationPot, &playButton, &leftButton, &rightButton);
CVManager cvManager(&cvOutPitch, &cvGate, &leftLED, &rightLED);

/**
 * @brief Callback function called when the sequencer advances to a new step
 * @param currentStep The current step index (0-based)
 * @param currentNote The MIDI note number for this step
 * @param noteDurationSeconds Duration of the note in seconds
 */
void onSequencerStep(int currentStep, int currentNote, float noteDurationSeconds)
{
  // Play the current note
  cvManager.setCVNote(currentNote);

  // Trigger CV gate output using the gate duration from the sequence
  float gateDuration = mainSequence.getGateDuration(currentStep);
  cvManager.triggerGate(noteDurationSeconds * gateDuration);

  // Update beat indicators
  cvManager.updateBeatIndicators(currentStep, noteDurationSeconds);

  // Update UI
  uiManager.draw(mainSequence, player, inputManager.getCurrentScaleType());
}

void setup()
{
  cvOutPitch.setup(20000); // Initialize PWM hardware with default 20kHz frequency

  // Initialize display with slower I2C for more predictable timing
  oledDisplay.setup(100000);                        // Use 100kHz instead of 400kHz for more consistent timing
  oledDisplay.getU8g2().setFont(u8g2_font_6x10_tf); // Set default font

  // Initialize the sequence with a major scale manually
  int sequence[] = {36, 38, 40, 41, 43, 45, 47, 48}; // C2 major scale
  int sequenceLength = sizeof(sequence) / sizeof(int);
  mainSequence.setNotes(sequence, sequenceLength);

  // Set up the callback and start the player
  player.onStepAdvance(onSequencerStep);
  player.start();
}

void update(float dt)
{
  // Update managers
  uiManager.update(dt);
  inputManager.update(dt, mainSequence, player);
  cvManager.update(dt);

  // Handle UI notifications
  if (inputManager.hasBpmChanged())
  {
    uiManager.notifyBpmChanged();
  }

  // Update CV output if in edit mode
  if (!player.getIsPlaying())
  {
    cvManager.setCVNote(mainSequence.getNote(player.getCurrentStep()));
  }

  // Update the player with the time delta
  player.update(dt);

  // Refresh UI periodically or when needed
  static float lastUIUpdate = 0;
  lastUIUpdate += dt;
  if (lastUIUpdate > 0.1f)
  { // Update UI at 10Hz
    uiManager.draw(mainSequence, player, inputManager.getCurrentScaleType());
    lastUIUpdate = 0;
  }
}

void loop()
{
  // Calculate delta time in seconds
  static unsigned long lastFrameTime = 0;
  unsigned long currentTime = micros();
  unsigned long deltaTime = currentTime - lastFrameTime; // This handles overflow automatically
  float dt = deltaTime / 1000000.0f;                     // Convert microseconds to seconds
  lastFrameTime = currentTime;

  // Always prioritize timing-critical updates
  update(dt);
}