#include <Arduino.h>
#include "hardware/pwm_controller.h"

#define BPM_POT A0      // Pin for BPM input (potentiometer)
#define POT A1          // Pin for additional potentiometer
#define SELECTOR_POT A2 // Pin for selector potentiometer

#define PLAY_BUTTON 2 // Pin for play/stop button
#define YELLOW_LED 13 // Pin for external LED (if used)
#define RED_LED 12    // Built-in LED for playback indication

const float MAX_VOLTAGE = 5.0; // Maximum output voltage for CV

// Note that corresponds to 0V output in MIDI terms
const int BASE_0V_NOTE = 36;         // C2
const float PWM_FREQUENCY = 20000.0; // PWM frequency in Hz

float bpm = 120.0; // Current beats per minute

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
void setCVNote(int note, hardware::PWMChannel channel = hardware::PWM_CHANNEL_9)
{
  float voltage = (note - BASE_0V_NOTE) / 12.0;
  float clampedVoltage = constrain(voltage, 0.0, MAX_VOLTAGE);
  float dutyCycle = clampedVoltage / MAX_VOLTAGE;
  hardware::setPWMDutyCycle(channel, dutyCycle);
}

void setup()
{
  // Initialize only specific PWM channels you want to use - no need to track array size!
  hardware::PWMChannel channels[] = {hardware::PWM_CHANNEL_9, hardware::PWM_CHANNEL_10, hardware::PWM_CHANNEL_11, hardware::PWM_CHANNEL_3};
  hardware::initPWM(channels, sizeof(channels) / sizeof(hardware::PWMChannel));

  // Set frequency for both timers
  hardware::setPWMFrequency(PWM_FREQUENCY);

  pinMode(PLAY_BUTTON, INPUT_PULLUP); // Use internal pull-up resistor

  pinMode(RED_LED, OUTPUT);      // Set red LED pin as output
  pinMode(YELLOW_LED, OUTPUT);   // Set yellow LED pin as output
  digitalWrite(RED_LED, LOW);    // Start with red LED off
  digitalWrite(YELLOW_LED, LOW); // Start with yellow LED off
}

// Major C2 to C3 scale in MIDI notes
int midiNotes[] = {36, 38, 40, 41, 43, 45, 47, 48};            // MIDI note numbers for C2 to C3 major scale
const int numNotes = sizeof(midiNotes) / sizeof(midiNotes[0]); // Number of notes in the scale

unsigned long previousNoteTime = 0; // Timestamp of the last note played
int currentNoteIndex = 0;           // Index of the current note in the scale
unsigned long ledOnTime = 0;        // Timestamp when LED was turned on
bool ledState = false;              // Current LED state

void loop()
{
  unsigned long currentTime = millis();
  float noteDuration = (60.0 / bpm) * 1000; // Duration of a quarter note in milliseconds

  // Check if it's time to play the next note
  if (currentTime - previousNoteTime >= noteDuration)
  {
    // Play the current note on pin 9
    setCVNote(midiNotes[currentNoteIndex], hardware::PWM_CHANNEL_9);

    // Play a harmony note (3rd) on pin 10
    int harmonyNote = midiNotes[currentNoteIndex] + 4; // Major third
    setCVNote(harmonyNote, hardware::PWM_CHANNEL_10);

    // Play a fifth on pin 11
    int fifthNote = midiNotes[currentNoteIndex] + 7; // Perfect fifth
    setCVNote(fifthNote, hardware::PWM_CHANNEL_11);

    // Play an octave on pin 3
    int octaveNote = midiNotes[currentNoteIndex] + 12; // Octave higher
    setCVNote(octaveNote, hardware::PWM_CHANNEL_3);

    // Turn on LED for beat indication
    digitalWrite(YELLOW_LED, HIGH);

    if (currentNoteIndex == 0)
    {
      digitalWrite(RED_LED, HIGH);
    }

    ledOnTime = currentTime;
    ledState = true;

    // Move to the next note
    currentNoteIndex = (currentNoteIndex + 1) % numNotes;
    // Update the time of the last note
    previousNoteTime = currentTime;
  }

  // Handle LED timing - longer for first beat (note index 0), shorter for others
  if (ledState)
  {
    unsigned long ledDuration = (currentNoteIndex == 1) ? noteDuration / 4 : noteDuration / 8; // Longer for first beat
    if (currentTime - ledOnTime >= ledDuration)
    {
      digitalWrite(YELLOW_LED, LOW);
      digitalWrite(RED_LED, LOW);
      ledState = false;
    }
  }

  // Update BPM from potentiometer
  bpm = readLogPot(BPM_POT, 60.0, 1000.0, 2.0); // Reset when the button is pressed
  if (digitalRead(PLAY_BUTTON) == LOW)
  {
    currentNoteIndex = 0;                                            // Reset to the first note
    previousNoteTime = currentTime;                                  // Reset the timer
    setCVNote(midiNotes[currentNoteIndex], hardware::PWM_CHANNEL_9); // Play the first note immediately on pin 9

    // Also set initial harmony notes on other channels
    int harmonyNote = midiNotes[currentNoteIndex] + 4; // Major third
    setCVNote(harmonyNote, hardware::PWM_CHANNEL_10);

    int fifthNote = midiNotes[currentNoteIndex] + 7; // Perfect fifth
    setCVNote(fifthNote, hardware::PWM_CHANNEL_11);

    int octaveNote = midiNotes[currentNoteIndex] + 12; // Octave higher
    setCVNote(octaveNote, hardware::PWM_CHANNEL_3);
  }
}
