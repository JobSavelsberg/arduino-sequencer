#include <Arduino.h>
#include "hardware/pwm.h"
#include "hardware/led.h"
#include "hardware/button.h"
#include "hardware/pot.h"
#include "sequence.h"
#include "sequencePlayer.h"

const float MAX_VOLTAGE = 5.0; // Maximum output voltage for CV
// Note that corresponds to 0V output in MIDI terms
const int BASE_0V_NOTE = 36; // C2

// LEDs
LED leftLED(12);
LED rightLED(13);

// Buttons
Button playButton(2);

// Potentiometers
Pot timingPot(3);

// CV output
PWM cvOutPitch(9, MAX_VOLTAGE);

// Sequence and player objects
Sequence mainSequence(16);                   // 16-step sequence
SequencePlayer player(&mainSequence, 120.0); // Player with 120 BPM

/**
 * @brief Convert MIDI note number to CV output voltage (1V per octave)
 * @details Starting from MIDI note set in BASE_0V_NOTE, calculates the appropriate
 *          voltage and sets the PWM duty cycle accordingly
 * @param note MIDI note number to convert
 * @param channel PWM channel to output to (default: PWM_CHANNEL_9)
 */
void setCVNote(int note)
{
  float voltage = (note - BASE_0V_NOTE) / 12.0;
  float clampedVoltage = constrain(voltage, 0.0, MAX_VOLTAGE);
  float dutyCycle = clampedVoltage / MAX_VOLTAGE; // Normalize to 0.0 - 1.0 range
  cvOutPitch.setDutyCycle(dutyCycle);             // Set PWM duty cycle based on voltage
}

void setup()
{
  cvOutPitch.setup(20000); // Initialize PWM hardware with default 20kHz frequency

  // Initialize the sequence with a major scale manually
  int majorScale[] = {36, 38, 40, 41, 43, 45, 47, 48}; // C2 major scale
  int scaleLength = sizeof(majorScale) / sizeof(majorScale[0]);
  mainSequence.setNotes(majorScale, scaleLength);

  // Set up the callback and start the player
  player.onStepAdvance(onSequencerStep);
  player.start();
}

void updateInputs()
{
  timingPot.update();
  playButton.update();
}

void updateOutputs()
{
  // Update the CV output based on the current note in the sequence
  setCVNote(player.getCurrentNote());

  leftLED.update();
  rightLED.update();
}

void loop()
{
  // Update inputs
  updateInputs();

  // Update the player (handles timing and callbacks)
  player.update();

  // Update BPM from potentiometer
  player.setBpm(timingPot.getLogValue(60.0, 1000.0, 2.0));

  // Reset when the button is pressed
  if (playButton.wasPressed())
  {
    player.reset();                     // Reset to the first step
    setCVNote(player.getCurrentNote()); // Play the first note immediately
  }

  // Update outputs
  updateOutputs();
}

/**
 * @brief Callback function called when the sequencer advances to a new step
 * @param currentStep The current step index (0-based)
 * @param currentNote The MIDI note number for this step
 * @param noteDuration Duration of the note in milliseconds
 */
void onSequencerStep(int currentStep, int currentNote, unsigned long noteDuration)
{
  // Play the current note
  setCVNote(currentNote);

  // Turn on LED for beat indication
  rightLED.blink(noteDuration / 8);

  if (currentStep == 0)
  {
    leftLED.blink(noteDuration / 4); // Longer blink for first beat
  }
}