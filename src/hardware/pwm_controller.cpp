#include "hardware/pwm_controller.h"

namespace hardware
{
  // Helper function to initialize PWM with an array of channels
  void initPWM(PWMChannel channels[], int numChannels)
  {
    bool useTimer1 = false;
    bool useTimer2 = false;
    bool enablePin9 = false;
    bool enablePin10 = false;
    bool enablePin3 = false;
    bool enablePin11 = false;

    // Single pass through channels to determine what we need
    for (int i = 0; i < numChannels; i++)
    {
      switch (channels[i])
      {
      case PWM_CHANNEL_9:
        pinMode(9, OUTPUT);
        useTimer1 = true;
        enablePin9 = true;
        break;
      case PWM_CHANNEL_10:
        pinMode(10, OUTPUT);
        useTimer1 = true;
        enablePin10 = true;
        break;
      case PWM_CHANNEL_11:
        pinMode(11, OUTPUT);
        useTimer2 = true;
        enablePin11 = true;
        break;
      case PWM_CHANNEL_3:
        pinMode(3, OUTPUT);
        useTimer2 = true;
        enablePin3 = true;
        break;
      }
    }

    // Initialize Timer1 if needed (pins 9 and 10)
    if (useTimer1)
    {
      // Stop Timer1 and reset control registers
      TCCR1A = 0;
      TCCR1B = 0;
      TCNT1 = 0;

      // Set Fast PWM mode with ICR1 as TOP
      if (enablePin9)
        TCCR1A |= (1 << COM1A1); // Non-inverting mode on OC1A
      if (enablePin10)
        TCCR1A |= (1 << COM1B1);            // Non-inverting mode on OC1B
      TCCR1A |= (1 << WGM11);               // Fast PWM part 1
      TCCR1B = (1 << WGM13) | (1 << WGM12); // Fast PWM part 2
      TCCR1B |= (1 << CS10);                // No prescaler (Timer clock = 16 MHz)
    }

    // Initialize Timer2 if needed (pins 3 and 11)
    if (useTimer2)
    {
      // Stop Timer2 and reset control registers
      TCCR2A = 0;
      TCCR2B = 0;
      TCNT2 = 0;

      // Set Fast PWM mode with top = 0xFF for Timer2
      if (enablePin11)
        TCCR2A |= (1 << COM2A1); // Non-inverting mode on OC2A
      if (enablePin3)
        TCCR2A |= (1 << COM2B1);             // Non-inverting mode on OC2B
      TCCR2A |= (1 << WGM21) | (1 << WGM20); // Fast PWM mode
      TCCR2B |= (1 << CS20);                 // No prescaler
    }
  }

  void setPWMFrequency(float freq_hz)
  {
    // Set frequency for Timer1 (pins 9 and 10)
    // 16MHz / freq_hz = timer top value for Timer1
    // Subtract 1 from result because counter goes from 0 to TOP
    unsigned int top = (16000000.0 / freq_hz) - 1;
    ICR1 = top;

    // Set frequency for Timer2 (pins 3 and 11)
    // Timer2 is 8-bit, so we need to use prescalers to achieve different frequencies
    // Base frequency: 16MHz / (prescaler * 256)

    // Clear prescaler bits and set new ones
    TCCR2B &= 0xF8; // Clear CS22, CS21, CS20 bits

    // Calculate required prescaler and set appropriate bits
    // Timer2 prescaler options: 1, 8, 32, 64, 128, 256, 1024
    if (freq_hz >= 31250) // 16MHz / (1 * 256) = 62.5kHz max
    {
      TCCR2B |= (1 << CS20); // Prescaler = 1
    }
    else if (freq_hz >= 3906) // 16MHz / (8 * 256) = 7.8kHz
    {
      TCCR2B |= (1 << CS21); // Prescaler = 8
    }
    else if (freq_hz >= 1953) // 16MHz / (32 * 256) = 1.95kHz
    {
      TCCR2B |= (1 << CS21) | (1 << CS20); // Prescaler = 32
    }
    else if (freq_hz >= 976) // 16MHz / (64 * 256) = 976Hz
    {
      TCCR2B |= (1 << CS22); // Prescaler = 64
    }
    else if (freq_hz >= 488) // 16MHz / (128 * 256) = 488Hz
    {
      TCCR2B |= (1 << CS22) | (1 << CS20); // Prescaler = 128
    }
    else if (freq_hz >= 244) // 16MHz / (256 * 256) = 244Hz
    {
      TCCR2B |= (1 << CS22) | (1 << CS21); // Prescaler = 256
    }
    else // 16MHz / (1024 * 256) = 61Hz minimum
    {
      TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20); // Prescaler = 1024
    }
  }

  void setPWMDutyCycle(PWMChannel channel, float duty_cycle)
  {
    // Calculate duty cycle value based on normalized duty cycle (0.0 - 1.0)
    switch (channel)
    {
    case PWM_CHANNEL_9:
    {
      unsigned int duty_value = (ICR1 * duty_cycle);
      OCR1A = duty_value;
    }
    break;
    case PWM_CHANNEL_10:
    {
      unsigned int duty_value = (ICR1 * duty_cycle);
      OCR1B = duty_value;
    }
    break;
    case PWM_CHANNEL_11:
    {
      unsigned int duty_value = (255 * duty_cycle); // Timer2 uses 8-bit (0-255)
      OCR2A = duty_value;
    }
    break;
    case PWM_CHANNEL_3:
    {
      unsigned int duty_value = (255 * duty_cycle); // Timer2 uses 8-bit (0-255)
      OCR2B = duty_value;
    }
    break;
    }
  }
}
