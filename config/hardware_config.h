/**
 * Hardware Configuration for Arduino Sequencer
 *
 * This file contains all hardware-specific settings that can be
 * customized for different hardware setups.
 */

#ifndef HARDWARE_CONFIG_H
#define HARDWARE_CONFIG_H

// =============================================================================
// PIN ASSIGNMENTS
// =============================================================================

// Analog Inputs (Potentiometers)
#define POT_BPM_PIN A3        // BPM control potentiometer
#define POT_MODULATION_PIN A2 // Modulation CV control potentiometer
#define POT_SPARE_PIN A1      // Spare potentiometer for future use

// Digital Inputs (Buttons)
#define BUTTON_RESET_PIN 2 // Reset/Play button
#define BUTTON_LEFT_PIN 3  // Left navigation button (future use)
#define BUTTON_RIGHT_PIN 4 // Right navigation button (future use)

// Digital Outputs (LEDs)
#define LED_BEAT_PIN 12     // Beat indicator LED
#define LED_DOWNBEAT_PIN 13 // Downbeat indicator LED (also built-in LED)

// PWM Outputs (CV)
#define CV_PITCH_PIN 9       // Primary pitch CV output
#define CV_MODULATION_PIN 10 // Modulation CV output

// I2C Display (Fixed pins on Arduino Uno)
#define DISPLAY_SDA_PIN A4 // I2C Data line
#define DISPLAY_SCL_PIN A5 // I2C Clock line

// =============================================================================
// TIMING CONFIGURATION
// =============================================================================

#define DEFAULT_BPM 120.0f // Default tempo
#define MIN_BPM 30.0f      // Minimum allowed BPM
#define MAX_BPM 300.0f     // Maximum allowed BPM

#define PWM_FREQUENCY 20000.0f // PWM frequency in Hz (20kHz)
#define DISPLAY_UPDATE_MS 100  // Display update interval in milliseconds
#define PARAMETER_UPDATE_MS 50 // Parameter reading interval in milliseconds

// =============================================================================
// CV OUTPUT CONFIGURATION
// =============================================================================

#define CV_MAX_VOLTAGE 5.0f     // Maximum CV output voltage
#define BASE_NOTE_MIDI 36       // MIDI note for 0V (C2)
#define CV_VOLT_PER_OCTAVE 1.0f // Standard 1V/octave

// =============================================================================
// BUTTON DEBOUNCING
// =============================================================================

#define BUTTON_DEBOUNCE_MS 50 // Button debounce time in milliseconds

// =============================================================================
// LED TIMING
// =============================================================================

#define LED_BEAT_DURATION_DIV 8     // Beat LED duration = note_duration / 8
#define LED_DOWNBEAT_DURATION_DIV 4 // Downbeat LED duration = note_duration / 4

// =============================================================================
// DISPLAY CONFIGURATION
// =============================================================================

#define DISPLAY_WIDTH 128        // Display width in pixels
#define DISPLAY_HEIGHT 64        // Display height in pixels
#define DISPLAY_I2C_SPEED 400000 // I2C clock speed (400kHz)

// =============================================================================
// PARAMETER SMOOTHING
// =============================================================================

#define BPM_SMOOTHING_FACTOR 0.2f        // BPM parameter smoothing (0.0-1.0)
#define MODULATION_SMOOTHING_FACTOR 0.1f // Modulation smoothing (0.0-1.0)

// =============================================================================
// VALIDATION RANGES
// =============================================================================

#define MIDI_NOTE_MIN 0   // Minimum valid MIDI note
#define MIDI_NOTE_MAX 127 // Maximum valid MIDI note

#endif
