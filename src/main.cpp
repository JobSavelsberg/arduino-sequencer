#include <Arduino.h>
#include "hardware/pwm.h"
#include "hardware/led.h"
#include "hardware/button.h"
#include "hardware/pot.h"

const float MAX_VOLTAGE = 5.0; // Maximum output voltage for CV

// Note that corresponds to 0V output in MIDI terms
const int BASE_0V_NOTE = 36; // C2

float bpm = 120.0; // Current beats per minute

// LEDs
LED leftLED(12);
LED rightLED(13);

// Buttons
Button playButton(2);

// Potentiometers
Pot timingPot(3);

// CV output
PWM cvOutPitch(9, MAX_VOLTAGE);

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

// Major C2 to C3 scale in MIDI notes
int midiNotes[] = {36, 38, 40, 41, 43, 45, 47, 48};            // MIDI note numbers for C2 to C3 major scale
const int numNotes = sizeof(midiNotes) / sizeof(midiNotes[0]); // Number of notes in the scale

unsigned long previousNoteTime = 0; // Timestamp of the last note played
int currentNoteIndex = 0;           // Index of the current note in the scale

void setup()
{
  cvOutPitch.setup(20000); // Initialize PWM hardware with default 20kHz frequency
}

void updateInputs()
{
  timingPot.update();
  playButton.update();
}

void updateOutputs()
{
  // Update the CV output based on the current note
  setCVNote(midiNotes[currentNoteIndex]);

  leftLED.update();
  rightLED.update();
}

void loop()
{
  // Update inputs
  updateInputs();

  unsigned long currentTime = millis();
  unsigned long noteDuration = (60000.0 / bpm); // Duration of a quarter note in milliseconds (using integer)

  // Check if it's time to play the next note
  if (currentTime - previousNoteTime >= noteDuration)
  {
    // Play the current note on pin 9
    setCVNote(midiNotes[currentNoteIndex]);

    // Turn on LED for beat indication
    rightLED.blink(noteDuration / 8);

    if (currentNoteIndex == 0)
    {
      leftLED.blink(noteDuration / 4); // Longer blink for first beat
    }

    // Move to the next note
    currentNoteIndex = (currentNoteIndex + 1) % numNotes;
    // Update the time of the last note
    previousNoteTime = currentTime;
  }

  // Update BPM from potentiometer
  bpm = timingPot.getLogValue(60.0, 1000.0, 2.0);

  // Reset when the button is pressed
  if (playButton.wasPressed())
  {
    currentNoteIndex = 0;                   // Reset to the first note
    previousNoteTime = currentTime;         // Reset the timer
    setCVNote(midiNotes[currentNoteIndex]); // Play the first note immediately on pin 9
  }

  // Update outputs
  updateOutputs();
}
