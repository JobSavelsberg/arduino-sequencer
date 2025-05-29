#include <Arduino.h>
#include "pwm_controller.h"

#define BPM_POT A0 // Pin for BPM input (potentiometer)

const float MAX_VOLTAGE = 5.0; // Maximum output voltage for CV

// Note that corresponds to 0V output in MIDI terms 
const int BASE_0V_NOTE = 36; // C2
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
float readLogPot(int pin, float minValue, float maxValue, float curve = 2.0) {
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
 */
void setCVNote(int note) {
  float voltage = (note - BASE_0V_NOTE) / 12.0;
  float clampedVoltage = constrain(voltage, 0.0, MAX_VOLTAGE);
  float dutyCycle = clampedVoltage / MAX_VOLTAGE; 
  setPWMDutyCycle(dutyCycle);
}


void setup() {
  // Initialize PWM system
  initPWM();
  setPWMFrequency(PWM_FREQUENCY);
  setPWMDutyCycle(0);
}

// Major C2 to C3 scale in MIDI notes
int midiNotes[] = {36, 38, 40, 41, 43, 45, 47, 48}; // MIDI note numbers for C2 to C3 major scale
const int numNotes = sizeof(midiNotes) / sizeof(midiNotes[0]); // Number of notes in the scale

unsigned long previousNoteTime = 0; // Timestamp of the last note played
int currentNoteIndex = 0; // Index of the current note in the scale

void loop() {
  unsigned long currentTime = millis();
  float noteDuration = (60.0 / bpm) * 1000; // Duration of a quarter note in milliseconds
  
  // Check if it's time to play the next note
  if (currentTime - previousNoteTime >= noteDuration) {
    // Play the next note
    setCVNote(midiNotes[currentNoteIndex]);
    
    // Move to the next note
    currentNoteIndex = (currentNoteIndex + 1) % numNotes;
      // Update the time of the last note
    previousNoteTime = currentTime;
  }
  
  // Update BPM from potentiometer
  bpm = readLogPot(BPM_POT, 60.0, 1000.0, 2.0);
}
