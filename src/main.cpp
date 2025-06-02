#include <Arduino.h>
#include <stdio.h>
#include "hardware/pwm.h"
#include "hardware/led.h"
#include "hardware/button.h"
#include "hardware/pot.h"
#include "hardware/display.h"
#include "hardware/gate.h"
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

// CV Gate output
Gate cvGate(8);

// Create display object
Display oledDisplay;

// Sequence and player objects
Sequence mainSequence(16);                    // 16-step sequence
SequencePlayer player(&mainSequence, 120.0f); // Player with 120 BPM

// BPM display timing
static float lastBpmChangeTime = 0.0f;
static float totalTime = 0.0f;
const float BPM_DISPLAY_DURATION = 3.0f; // Show BPM for 3 seconds after change

// Scale display timing
static float lastScaleChangeTime = 0.0f;
static int lastScaleType = -1;             // Track last scale type to detect changes
const float SCALE_DISPLAY_DURATION = 3.0f; // Show scale for 3 seconds after change

// Transpose tracking
static int currentTranspose = 0; // Current transpose amount in semitones

/*
 * Available scales for randomization (selected by modulation pot):
 * 0 = Major scale (Ionian)
 * 1 = Natural minor (Aeolian)
 * 2 = Harmonic minor
 * 3 = Lydian
 * 4 = Mixolydian
 * 5 = Dorian
 * 6 = Phrygian
 * 7 = Pentatonic major
 * 8 = Pentatonic minor
 * 9 = Blues scale
 */

// Function declarations
void scaleTypeToString(int scaleType, char *scaleStr);

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

    // Draw current scale type only if it has changed in the last 3 seconds
    int currentScaleType = (int)modulationPot.getLinearValue(0, 9.99);

    // Check if scale type has changed
    if (currentScaleType != lastScaleType)
    {
      lastScaleType = currentScaleType;
      lastScaleChangeTime = totalTime;
    }

    // Also don't show scale when bpm is shown, otherwise they overlap
    if (totalTime - lastScaleChangeTime <= SCALE_DISPLAY_DURATION && totalTime - lastBpmChangeTime > BPM_DISPLAY_DURATION)
    {
      char scaleStr[16];
      char scaleDisplayStr[24];
      scaleTypeToString(currentScaleType, scaleStr);
      sprintf(scaleDisplayStr, "Scale: %s", scaleStr);
      u8g2.drawStr(1, 8, scaleDisplayStr); // Position below BPM display
    }

    // Draw sequence visualization
    const int SEQ_START_X = 0;
    const int SEQ_START_Y = 10;                             // Move down to make room for scale display
    const int SEQ_HEIGHT = 64 - 20;                        // Reduce height to accommodate scale text
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
      float gateDuration = mainSequence.getGateDuration(i);

      // Map note to height (higher notes = taller rectangles)
      int noteHeight = map(note, LOWEST_NOTE, HIGHEST_NOTE, 3, SEQ_HEIGHT); // Map MIDI range to pixel height
      int y = SEQ_START_Y + SEQ_HEIGHT - noteHeight;

      // Calculate gate width based on gate duration (0.0 to 1.0)
      int gateWidth = (int)(STEP_WIDTH * gateDuration);
      if (gateWidth < 1)
        gateWidth = 1; // Minimum 1 pixel width

      // Draw rectangle - filled if current step, outline if not
      if (i == player.getCurrentStep())
      {
        u8g2.drawBox(x, y, gateWidth, noteHeight); // Filled rectangle for current step
      }
      else
      {
        u8g2.drawFrame(x, y, gateWidth, noteHeight); // Outline rectangle for other steps
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

  // Trigger CV gate output using the gate duration from the sequence
  float gateDuration = mainSequence.getGateDuration(currentStep);
  cvGate.trigger(noteDurationSeconds * gateDuration);

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
  int sequence[] = {36, 38, 40, 41, 43, 45, 47, 48}; // C2 major scale
  int sequenceLength = sizeof(sequence) / sizeof(int);
  mainSequence.setNotes(sequence, sequenceLength);
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

  // Handle timing pot usage based on mode
  if (player.getIsPlaying())
  {
    // Playing mode: Use timing pot for BPM control
    float newBpm = timingPot.getLogValue(60.0, 500.0, 2.0);
    if (timingPot.hasChanged(10)) // Only update if significant change
    {
      player.setBpm(newBpm);
      lastBpmChangeTime = totalTime; // Record when BPM was changed
    }
  }

  // Update pitch from potentiometer
  if (!player.getIsPlaying())
  {
    // Edit mode: Use modulation pot to control pitch range (1-5 octaves)
    float pitchRange = modulationPot.getLinearValue(1.0, 5.0); // 1 to 5 octaves
    int octaveRange = (int)(pitchRange * 12);                  // Convert to semitones

    int newNote = (int)pitchPot.getLinearValue(BASE_0V_NOTE, BASE_0V_NOTE + octaveRange);
    if (pitchPot.hasChanged(5)) // Only update if significant change
    {
      mainSequence.setNote(player.getCurrentStep(), newNote);
      setCVNote(newNote); // Update CV output immediately
      drawUI();           // Refresh display immediately
    }

    // Edit mode: Use timing pot to control gate duration (10% to 100%)
    float newGateDuration = timingPot.getLinearValue(0.1, 1.0); // 10% to 100%
    if (timingPot.hasChanged(10))                               // Only update if significant change
    {
      mainSequence.setGateDuration(player.getCurrentStep(), newGateDuration);
      drawUI(); // Refresh display immediately
    }
  }
  else
  {
    // Playing mode: Use pitch pot to transpose entire sequence
    // Range of +/- 1 octave (12 semitones) for better control
    int newTranspose = (int)pitchPot.getLinearValue(-12, 12);
    if (pitchPot.hasChanged(5)) // Only update if significant change
    {
      int transposeChange = newTranspose - currentTranspose;
      if (transposeChange != 0)
      {
        mainSequence.transpose(transposeChange);
        currentTranspose = newTranspose;

        setCVNote(mainSequence.getNote(player.getCurrentStep())); // Update CV output to current note

        drawUI(); // Refresh display immediately
      }
    }
  }
  // Check if the play button was pressed
  if (playButton.wasPressed())
  {
    if (player.getIsPlaying())
    {
      player.stop(); // Pause if currently playing
      // Reset transpose when stopping
      if (currentTranspose != 0)
      {
        mainSequence.transpose(-currentTranspose); // Undo current transpose
        currentTranspose = 0;
        drawUI(); // Refresh display
      }
    }
    else
    {
      player.start(); // Resume/start if currently stopped
      // Reset transpose when starting
      currentTranspose = 0;
    }
  } // Button handling depends on play mode
  // Check for both buttons pressed simultaneously (randomize sequence) - highest priority
  static bool lastBothPressed = false;
  static bool justReleasedBoth = false;
  bool bothCurrentlyPressed = leftButton.isPressed() && rightButton.isPressed();
  if (bothCurrentlyPressed && !lastBothPressed)
  {
    // Both buttons just pressed - randomize sequence
    // Use modulation pot to select scale type (0-9 scales)
    int scaleType = (int)modulationPot.getLinearValue(0, 9.99); // 0-9 scale types
    mainSequence.randomize(BASE_0V_NOTE, 3, scaleType);         // Root=C2, 3 octaves, selected scale
    setCVNote(mainSequence.getNote(player.getCurrentStep()));

    // Update scale display timing to show the scale used for randomization
    lastScaleType = scaleType;
    lastScaleChangeTime = totalTime;

    drawUI();
  }

  // Track when both buttons were just released
  if (lastBothPressed && !bothCurrentlyPressed)
  {
    justReleasedBoth = true;
  }
  else if (!leftButton.isPressed() && !rightButton.isPressed())
  {
    // Reset the flag when both buttons are fully released
    justReleasedBoth = false;
  }

  lastBothPressed = bothCurrentlyPressed;
  // Only process individual button presses if both buttons are not currently pressed
  // and we didn't just release both buttons
  if (!bothCurrentlyPressed && !justReleasedBoth)
  {
    if (!player.getIsPlaying())
    {
      // Note edit mode: when paused, use left/right buttons to step through notes
      if (leftButton.wasReleased())
      {
        // Move to previous step
        int currentStep = player.getCurrentStep();
        int newStep = (currentStep - 1 + mainSequence.getLength()) % mainSequence.getLength();
        player.setCurrentStep(newStep);

        // Play the note and update CV output
        setCVNote(mainSequence.getNote(newStep));
        drawUI(); // Refresh display immediately
      }

      if (rightButton.wasReleased())
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
    else
    {
      // Play mode: use left/right buttons to adjust sequence length
      if (leftButton.wasReleased())
      {
        // Decrease sequence length (minimum 1 step)
        int currentLength = mainSequence.getLength();
        if (currentLength > 1)
        {
          mainSequence.setLength(currentLength - 1);

          // If current step is beyond new length, wrap to beginning
          if (player.getCurrentStep() >= mainSequence.getLength())
          {
            player.setCurrentStep(0);
            setCVNote(mainSequence.getNote(0));
          }

          drawUI(); // Refresh display immediately
        }
      }

      if (rightButton.wasReleased())
      {
        // Increase sequence length (up to maximum)
        int currentLength = mainSequence.getLength();
        if (currentLength < mainSequence.getMaxLength())
        {
          mainSequence.setLength(currentLength + 1);
          drawUI(); // Refresh display immediately
        }
      }
    }
  }

  // Update the player with the time delta
  player.update(dt);
  // Update outputs
  leftLED.update(dt);
  rightLED.update(dt);
  cvGate.update(dt);
}

/**
 * @brief Convert scale type number to scale name string
 * @param scaleType Scale type number (0-9)
 * @param scaleStr Output string buffer (must be at least 16 chars)
 */
void scaleTypeToString(int scaleType, char *scaleStr)
{
  const char *scaleNames[] = {
      "Major", "Minor", "Harm Min", "Lydian", "Mixolyd",
      "Dorian", "Phrygian", "Pent Maj", "Pent Min", "Blues"};

  if (scaleType >= 0 && scaleType <= 9)
  {
    strcpy(scaleStr, scaleNames[scaleType]);
  }
  else
  {
    strcpy(scaleStr, "Unknown");
  }
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