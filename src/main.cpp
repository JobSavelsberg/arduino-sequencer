#include <Arduino.h>
#include "pwm_controller.h"

// Define voltage constant
const float MAX_VOLTAGE = 5.0;  // 5V is the max voltage output

void setup() {
  // Initialize PWM system
  initPWM();
  setPWMFrequency(20000); // 20 kHz (should not be audible)
  setPWMDutyCycle(0);
}

void loop() {
  // Generate note voltages based on 1V per octave standard
  const int NUM_NOTES = 12 * 5; // Five octaves
    
  // Play each note in the octave
  for (int i = 0; i < NUM_NOTES; i++) {
    // Convert note voltage to duty cycle (voltage / MAX_VOLTAGE)
    float duty_cycle = i * (1.0 / 12.0) / MAX_VOLTAGE;
    setPWMDutyCycle(duty_cycle);
    delay(100);
  }
}
