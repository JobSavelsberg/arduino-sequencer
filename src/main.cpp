#include <Arduino.h>
#include <stdio.h>
#include "hardware/pwm.h"
#include "hardware/led.h"
#include "hardware/button.h"
#include "hardware/pot.h"
#include "hardware/display.h"
#include "sequence.h"
#include "sequence_player.h"

const float MAX_VOLTAGE = 5.0; // Maximum output voltage for CV
// Note that corresponds to 0V output in MIDI terms
const int BASE_0V_NOTE = 36; // C2

// LEDs
LED leftLED(12);
LED rightLED(13);

// Buttons
Button playButton(2);

// Potentiometers
Pot timingPot(3, 0.01); // 10ms read interval in seconds

// CV output
PWM cvOutPitch(9, MAX_VOLTAGE);

// Create display object
Display oledDisplay;

// Sequence and player objects
Sequence mainSequence(16);                   // 16-step sequence
SequencePlayer player(&mainSequence, 120.0); // Player with 120 BPM

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

void drawUI()
{
  // Early exit if display is not initialized
  if (!oledDisplay.isInitialized())
    return;

  auto &u8g2 = oledDisplay.getU8g2();

  u8g2.clearBuffer(); // Start frame

  // Draw header
  u8g2.drawStr(32, 10, "SEQUENCER");
  // Draw BPM display
  char bpmStr[16];
  char bpmValue[8];
  dtostrf(player.getBpm(), 0, 1, bpmValue);
  sprintf(bpmStr, "BPM: %s", bpmValue);
  u8g2.drawStr(0, 25, bpmStr);

  // Draw current step indicator
  char stepStr[16];
  sprintf(stepStr, "Step: %d/%d", player.getCurrentStep() + 1, mainSequence.getLength());
  u8g2.drawStr(70, 25, stepStr);

  // Draw current note
  char noteStr[16];
  sprintf(noteStr, "Note: %d", mainSequence.getNote(player.getCurrentStep()));
  u8g2.drawStr(0, 64, noteStr);

  u8g2.sendBuffer(); // End frame
}

/**
 * @brief Callback function called when the sequencer advances to a new step
 * @param currentStep The current step index (0-based)
 * @param currentNote The MIDI note number for this step
 * @param noteDurationSeconds Duration of the note in seconds
 */
void onSequencerStep(int currentStep, int currentNote, double noteDurationSeconds)
{
  // Play the current note
  setCVNote(currentNote);

  // Calculate blink durations
  double rightBlinkDuration = noteDurationSeconds / 2;
  double leftBlinkDuration = noteDurationSeconds;

  // Turn on LED for beat indication
  rightLED.blink(rightBlinkDuration);

  if (currentStep == 0)
  {
    leftLED.blink(leftBlinkDuration);
  }
  drawUI(); // Draw the UI if needed
}

void setup()
{
  cvOutPitch.setup(20000); // Initialize PWM hardware with default 20kHz frequency

  // Initialize display with slower I2C for more predictable timing
  oledDisplay.setup(100000);                        // Use 100kHz instead of 400kHz for more consistent timing
  oledDisplay.getU8g2().setFont(u8g2_font_6x10_tf); // Set default font

  // Initialize the sequence with a major scale manually
  int majorScale[] = {36, 38, 40, 41, 43, 45, 47, 48}; // C2 major scale
  int scaleLength = sizeof(majorScale) / sizeof(int);
  mainSequence.setNotes(majorScale, scaleLength);
  // Set up the callback and start the player
  player.onStepAdvance(onSequencerStep);
  player.start();
}

void update(double dt)
{
  // Update inputs
  timingPot.update(dt);
  playButton.update(dt);

  // Update BPM from potentiometer
  float newBpm = timingPot.getLogValue(60.0, 1000.0, 2.0);
  if (abs(newBpm - player.getBpm()) > 0.1) // Only update if significant change
  {
    player.setBpm(newBpm);
  }

  // Check if the play button was pressed
  if (playButton.wasPressed())
  {
    player.reset(); // Reset to the first step
  }

  // Update the player with the time delta
  player.update(dt);

  // Update outputs
  leftLED.update(dt);
  rightLED.update(dt);
}

void loop()
{
  // Calculate delta time in seconds
  static unsigned long lastFrameTime = 0;
  unsigned long currentTime = micros();
  unsigned long deltaTime = currentTime - lastFrameTime; // This handles overflow automatically
  double dt = deltaTime / 1000000.0;                     // Convert microseconds to seconds
  lastFrameTime = currentTime;

  // Always prioritize timing-critical updates
  update(dt);
}