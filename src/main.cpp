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
Button leftButton(7);
Button rightButton(4);

// Potentiometers - reduce smoothing to save memory
Pot timingPot(3, 0.01f, 1);     // 10ms read interval, no smoothing to save memory
Pot pitchPot(2, 0.01f, 1);      // 10ms read interval, no smoothing to save memory
Pot modulationPot(1, 0.01f, 1); // 10ms read interval, no smoothing to save memory

// CV output
PWM cvOutPitch(9, MAX_VOLTAGE);

// Create display object
Display oledDisplay;

// Sequence and player objects
Sequence mainSequence(16);                    // 16-step sequence
SequencePlayer player(&mainSequence, 120.0f); // Player with 120 BPM

// BPM display timing
static float lastBpmChangeTime = 0.0f;
static float totalTime = 0.0f;
const float BPM_DISPLAY_DURATION = 3.0f; // Show BPM for 3 seconds after change

/**
 * @brief Convert MIDI note number to note name string (e.g., "C#3")
 * @param midiNote MIDI note number (0-127)
 * @param noteStr Output string buffer (must be at least 4 chars)
 */
void midiNoteToString(int midiNote, char *noteStr)
{
  const char *noteNames[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};

  int note = midiNote % 12;         // Get note within octave (0-11)
  int octave = (midiNote / 12) - 1; // Calculate octave (-1 to 9 for MIDI range 0-127)

  sprintf(noteStr, "%s%d", noteNames[note], octave);
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

void drawUI()
{
  // Early exit if display is not initialized
  if (!oledDisplay.isInitialized())
    return;

  auto &u8g2 = oledDisplay.getU8g2();

  u8g2.firstPage(); // Start page mode rendering
  do
  {
    // Draw BPM display only if it has changed in the last 3 seconds
    if (totalTime - lastBpmChangeTime <= BPM_DISPLAY_DURATION)
    {
      char bpmStr[16];
      char bpmValue[8];
      dtostrf(player.getBpm(), 0, 1, bpmValue);
      sprintf(bpmStr, "BPM: %s", bpmValue);
      u8g2.drawStr(1, 8, bpmStr);
    }

    // Draw sequence visualization
    const int SEQ_START_X = 0;
    const int SEQ_START_Y = 0;
    const int SEQ_HEIGHT = 64 - 10;
    const int STEP_WIDTH = 128 / mainSequence.getLength(); // Width of each step rectangle

    int LOWEST_NOTE = 127;
    int HIGHEST_NOTE = 0; // MIDI note range

    // Set lowest and highest note to what is in the sequence
    for (int i = 0; i < mainSequence.getLength(); i++)
    {
      int note = mainSequence.getNote(i);
      if (note < LOWEST_NOTE || LOWEST_NOTE == 127)
        LOWEST_NOTE = note;
      if (note > HIGHEST_NOTE || HIGHEST_NOTE == 0)
        HIGHEST_NOTE = note;
    }

    for (int i = 0; i < mainSequence.getLength(); i++)
    {
      int x = SEQ_START_X + (i * STEP_WIDTH);
      int note = mainSequence.getNote(i);

      // Map note to height (higher notes = taller rectangles)
      int noteHeight = map(note, LOWEST_NOTE, HIGHEST_NOTE, 3, SEQ_HEIGHT); // Map MIDI range to pixel height
      int y = SEQ_START_Y + SEQ_HEIGHT - noteHeight;

      // Draw rectangle - filled if current step, outline if not
      if (i == player.getCurrentStep())
      {
        u8g2.drawBox(x, y, STEP_WIDTH, noteHeight); // Filled rectangle for current step
      }
      else
      {
        u8g2.drawFrame(x, y, STEP_WIDTH, noteHeight); // Outline rectangle for other steps
      }
    }
    // Draw current step indicator
    char stepStr[16];
    sprintf(stepStr, "%d/%d", player.getCurrentStep() + 1, mainSequence.getLength());
    u8g2.drawStr(128 - 24, 64, stepStr);
    // Draw current note
    char noteStr[16];
    char noteName[8];
    midiNoteToString(mainSequence.getNote(player.getCurrentStep()), noteName);
    sprintf(noteStr, "%s", noteName);
    u8g2.drawStr(0, 64, noteStr);

  } while (u8g2.nextPage()); // End page mode rendering
}

/**
 * @brief Callback function called when the sequencer advances to a new step
 * @param currentStep The current step index (0-based)
 * @param currentNote The MIDI note number for this step
 * @param noteDurationSeconds Duration of the note in seconds
 */
void onSequencerStep(int currentStep, int currentNote, float noteDurationSeconds)
{
  // Play the current note
  setCVNote(currentNote);
  // Calculate blink durations
  float rightBlinkDuration = noteDurationSeconds / 2;
  float leftBlinkDuration = noteDurationSeconds;

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

void update(float dt)
{
  // Update total time
  totalTime += dt;
  // Update inputs
  timingPot.update(dt);
  pitchPot.update(dt);
  modulationPot.update(dt); // Now update the modulation pot

  playButton.update(dt);
  leftButton.update(dt);
  rightButton.update(dt);

  // Update BPM from potentiometer
  float newBpm = timingPot.getLogValue(60.0, 1000.0, 2.0);
  if (timingPot.hasChanged(10)) // Only update if significant change
  {
    player.setBpm(newBpm);
    lastBpmChangeTime = totalTime; // Record when BPM was changed
  }
  // Update pitch from potentiometer when not playing (edit mode)
  if (!player.getIsPlaying())
  {
    // Use modulation pot to control pitch range (1-5 octaves)
    float pitchRange = modulationPot.getLinearValue(1.0, 5.0); // 1 to 5 octaves
    int octaveRange = (int)(pitchRange * 12);                  // Convert to semitones

    int newNote = (int)pitchPot.getLinearValue(BASE_0V_NOTE, BASE_0V_NOTE + octaveRange);
    if (pitchPot.hasChanged(5)) // Only update if significant change
    {
      mainSequence.setNote(player.getCurrentStep(), newNote);
      setCVNote(newNote); // Update CV output immediately
      drawUI();           // Refresh display immediately
    }
  }

  // Check if the play button was pressed
  if (playButton.wasPressed())
  {
    if (player.getIsPlaying())
    {
      player.stop(); // Pause if currently playing
    }
    else
    {
      player.start(); // Resume/start if currently stopped
    }
  }

  // Note edit mode: when paused, use left/right buttons to step through notes
  if (!player.getIsPlaying())
  {
    if (leftButton.wasPressed())
    {
      // Move to previous step
      int currentStep = player.getCurrentStep();
      int newStep = (currentStep - 1 + mainSequence.getLength()) % mainSequence.getLength();
      player.setCurrentStep(newStep);

      // Play the note and update CV output
      setCVNote(mainSequence.getNote(newStep));
      drawUI(); // Refresh display immediately
    }

    if (rightButton.wasPressed())
    {
      // Move to next step
      int currentStep = player.getCurrentStep();
      int newStep = (currentStep + 1) % mainSequence.getLength();
      player.setCurrentStep(newStep);

      // Play the note and update CV output
      setCVNote(mainSequence.getNote(newStep));
      drawUI(); // Refresh display immediately
    }
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
  float dt = deltaTime / 1000000.0f;                     // Convert microseconds to seconds
  lastFrameTime = currentTime;

  // Always prioritize timing-critical updates
  update(dt);
}