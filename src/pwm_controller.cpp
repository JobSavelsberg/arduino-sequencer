#include "pwm_controller.h"

void initPWM() {
  pinMode(9, OUTPUT); // Set Pin 9 (OC1A) as output

  // Stop Timer1 and reset control registers
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;

  // Set Fast PWM mode with ICR1 as TOP
  TCCR1A = (1 << COM1A1);               // Non-inverting mode on OC1A
  TCCR1A |= (1 << WGM11);               // Fast PWM part 1
  TCCR1B = (1 << WGM13) | (1 << WGM12); // Fast PWM part 2
  TCCR1B |= (1 << CS10);                // No prescaler (Timer clock = 16 MHz)
}

void setPWMFrequency(float freq_hz) {
  // 16MHz / freq_hz = timer top value
  // Subtract 1 from result because counter goes from 0 to TOP
  unsigned int top = (16000000.0 / freq_hz) - 1;
  ICR1 = top;
}

void setPWMDutyCycle(float duty_cycle) {
  // Calculate OCR1A value based on normalized duty cycle (0.0 - 1.0)
  unsigned int duty_value = (ICR1 * duty_cycle);
  OCR1A = duty_value;
}
