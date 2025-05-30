#include "hardware/pwm_controller.h"

// PWM pin definitions
const int PWM_PIN_9 = 9;   // Timer1 output A (OC1A)
const int PWM_PIN_10 = 10; // Timer1 output B (OC1B)

void initPWM()
{
  pinMode(PWM_PIN_9, OUTPUT);  // Set Pin 9 (OC1A) as output
  pinMode(PWM_PIN_10, OUTPUT); // Set Pin 10 (OC1B) as output

  // Stop Timer1 and reset control registers
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;

  // Set Fast PWM mode with ICR1 as TOP
  TCCR1A = (1 << COM1A1) | (1 << COM1B1); // Non-inverting mode on both OC1A and OC1B
  TCCR1A |= (1 << WGM11);                 // Fast PWM part 1
  TCCR1B = (1 << WGM13) | (1 << WGM12);   // Fast PWM part 2
  TCCR1B |= (1 << CS10);                  // No prescaler (Timer clock = 16 MHz)
}

void setPWMFrequency(float freq_hz)
{
  // 16MHz / freq_hz = timer top value
  // Subtract 1 from result because counter goes from 0 to TOP
  unsigned int top = (16000000.0 / freq_hz) - 1;
  ICR1 = top;
}

void setPWMDutyCycle(int pin, float duty_cycle)
{
  // Calculate duty cycle value based on normalized duty cycle (0.0 - 1.0)
  unsigned int duty_value = (ICR1 * duty_cycle);

  // Set the appropriate output compare register based on pin
  if (pin == PWM_PIN_9)
  {
    OCR1A = duty_value; // Pin 9 uses OCR1A
  }
  else if (pin == PWM_PIN_10)
  {
    OCR1B = duty_value; // Pin 10 uses OCR1B
  }
}
