#include "hardware/cv_output.h"
#include "hardware/analog_output.h"

// CV output pin assignments
const int CV_PITCH_PIN = ANALOG_OUT_PIN_9;       // Primary pitch CV output (pin 9)
const int CV_MODULATION_PIN = ANALOG_OUT_PIN_10; // Modulation CV output (pin 10)

void initCVOutput()
{
    // CV uses the analog output system
    initAnalogOutput();
}

void setCVNote(int midiNote, int baseNote, float maxVoltage)
{
    // Calculate voltage using 1V/octave standard
    float voltage = (midiNote - baseNote) / 12.0; // 1V per octave, 12 semitones per octave

    // Clamp to valid range
    if (voltage < 0)
        voltage = 0;
    if (voltage > maxVoltage)
        voltage = maxVoltage;

    setAnalogVoltage(CV_PITCH_PIN, voltage, maxVoltage);
}

void setCVPitch(float semitones, int baseNote)
{
    float voltage = semitones / 12.0; // 1V per octave
    if (voltage < 0)
        voltage = 0;
    setAnalogVoltage(CV_PITCH_PIN, voltage);
}

void setCVModulation(float modulationLevel, float maxVoltage)
{
    float clampedLevel = constrain(modulationLevel, 0.0, 1.0);
    float voltage = clampedLevel * maxVoltage;
    setAnalogVoltage(CV_MODULATION_PIN, voltage, maxVoltage);
}

void cvPitchOff()
{
    analogOff(CV_PITCH_PIN); // 0V = base note
}

void cvModulationOff()
{
    analogOff(CV_MODULATION_PIN); // 0V = no modulation
}

void cvAllOff()
{
    analogOff(CV_PITCH_PIN);
    analogOff(CV_MODULATION_PIN);
}
