#include <Arduino.h>
#include "hardware/pwm.h"
#include "hardware/led.h"
#include "hardware/button.h"

#define POT_A A3 // Pin for BPM input (potentiometer)
#define POT_B A2 // Pin for additional potentiometer
#define POT_C A1 // Pin for selector potentiometer

#define PLAY_BUTTON 2 // Pin for play/stop button

const float MAX_VOLTAGE = 5.0; // Maximum output voltage for CV

// Note that corresponds to 0V output in MIDI terms
const int BASE_0V_NOTE = 36; // C2

float bpm = 120.0; // Current beats per minute

// LED controllers
LED leftLED(12);
LED rightLED(13);

// Button controller
Button playButton(PLAY_BUTTON);

PWM cvOutPitch(9); // PWM output for CV pitch control

/**
 * @brief Read potentiometer with logarithmic curve mapping
 * @param pin Analog pin to read from (0-1023)
 * @param minValue Minimum output value
 * @param maxValue Maximum output value
 * @param curve Logarithmic curve factor (1.0 = linear, 2.0 = quadratic, etc.)
 * @return Mapped value between minValue and maxValue using logarithmic curve
 */
float readLogPot(int pin, float minValue, float maxValue, float curve = 2.0)
{
  int analogValue = analogRead(pin);
  float normalizedValue = (float)analogValue / 1023.0;
  float logValue = pow(normalizedValue, curve);
  return minValue + logValue * (maxValue - minValue);
}

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

unsigned long previousNoteTime = 0;         // Timestamp of the last note played
int currentNoteIndex = 0;                   // Index of the current note in the scale
unsigned long lastBPMRead = 0;              // Timestamp of last BPM reading
const unsigned long BPM_READ_INTERVAL = 50; // Read BPM every 50ms

void setup()
{
  cvOutPitch.setup(20000); // Initialize PWM hardware with default 20kHz frequency
}

void loop()
{
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

  // Update LED controllers (handles blink timing automatically)
  leftLED.update();
  rightLED.update();

  // Update button controller
  playButton.update();

  // Update BPM from potentiometer only occasionally to avoid timing jitter
  if (currentTime - lastBPMRead >= BPM_READ_INTERVAL)
  {
    bpm = readLogPot(POT_A, 60.0, 1000.0, 2.0);
    lastBPMRead = currentTime;
  }

  // Reset when the button is pressed
  if (playButton.wasPressed())
  {
    currentNoteIndex = 0;                   // Reset to the first note
    previousNoteTime = currentTime;         // Reset the timer
    setCVNote(midiNotes[currentNoteIndex]); // Play the first note immediately on pin 9
  }
}
