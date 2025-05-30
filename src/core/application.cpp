#include "core/application.h"
#include "hardware/hardware.h"
#include "hardware/display.h"
#include "hardware/cv_output.h"
#include "utils/sequencer_state.h"

// Static member definitions
int Application::defaultNotes[] = {36, 40, 41, 43, 45, 38, 41, 48, 45, 50, 52, 59, 60, 53, 57, 55};
const int Application::defaultNoteCount = sizeof(defaultNotes) / sizeof(defaultNotes[0]);
const int Application::BASE_NOTE = 36; // C2

Application::Application()
    : sequencer(nullptr), currentPattern(nullptr), clock(nullptr),
      parameterManager(nullptr), ledManager(nullptr), isInitialized(false), lastUpdateTime(0)
{
}

Application::~Application()
{
    cleanup();
}

bool Application::initialize()
{
    if (isInitialized)
    {
        return true;
    }

    // Initialize hardware first
    initHardware();
    initDisplay();
    initCVOutput();

    // Create core components - Arduino doesn't use exceptions, so check for null
    currentPattern = new Pattern(defaultNotes, defaultNoteCount);
    if (!currentPattern)
    {
        cleanup();
        return false;
    }

    clock = new Clock(120.0);
    if (!clock)
    {
        cleanup();
        return false;
    }

    sequencer = new Sequencer(currentPattern, clock, BASE_NOTE);
    if (!sequencer)
    {
        cleanup();
        return false;
    }

    parameterManager = new ParameterManager();
    if (!parameterManager)
    {
        cleanup();
        return false;
    }

    ledManager = new LedManager();
    if (!ledManager)
    {
        cleanup();
        return false;
    }

    // Start the sequencer
    sequencer->start();

    isInitialized = true;
    return true;
}

void Application::update()
{
    if (!isInitialized)
    {
        return;
    }

    unsigned long currentTime = millis();

    // Update all subsystems
    parameterManager->update(currentTime);
    ledManager->update(currentTime);

    // Handle user input
    handleUserInput();

    // Update sequencer and check if note changed
    bool noteChanged = sequencer->update(currentTime);

    if (noteChanged)
    {
        updateCVOutput();
        updateLEDs();
    }

    // Update display (less frequently to avoid flicker)
    static unsigned long lastDisplayUpdate = 0;
    if (currentTime - lastDisplayUpdate >= 100)
    { // Update every 100ms
        updateDisplay();
        lastDisplayUpdate = currentTime;
    }

    lastUpdateTime = currentTime;
}

void Application::handleUserInput()
{
    // Handle BPM control
    float newBpm = parameterManager->readBpm(POT_A, 60.0, 200.0);
    clock->setBpm(newBpm);

    // Handle modulation control
    float modLevel = parameterManager->readModulation(POT_B, 0.0, 1.0);
    setCVModulation(modLevel);

    // Handle reset button
    if (parameterManager->isButtonJustPressed(BUT_A))
    {
        reset();
    }

    // Handle other buttons for future features
    if (parameterManager->isButtonJustPressed(BUT_LEFT))
    {
        // Future: Previous pattern or step backward
    }

    if (parameterManager->isButtonJustPressed(BUT_RIGHT))
    {
        // Future: Next pattern or step forward
    }
}

void Application::updateCVOutput()
{
    int currentNote = sequencer->getCurrentNote();
    int baseNote = sequencer->getBaseNote();
    setCVNote(currentNote, baseNote);
}

void Application::updateLEDs()
{
    float noteDuration = clock->getNoteDurationMs();

    // Beat indication
    ledManager->indicateBeat(LED_B, noteDuration / 8);

    // Downbeat indication (first note of pattern)
    if (sequencer->getCurrentNoteIndex() == 0)
    {
        ledManager->indicateDownbeat(LED_A, noteDuration / 4);
    }
}

void Application::updateDisplay()
{
    // Create a simplified state structure for display
    // This maintains compatibility with existing display code
    SequencerState displayState = {
        .currentNoteIndex = sequencer->getCurrentNoteIndex(),
        .numNotes = sequencer->getPattern()->getLength(),
        .midiNotes = nullptr, // We'll handle this in the display update
        .bpm = clock->getBpm(),
        .baseNote = sequencer->getBaseNote(),
        .previousNoteTime = clock->getLastTickTime(),
        .ledOnTime = 0,
        .ledState = ledManager->isLedOn(LED_B),
        .modulationLevel = 0.0 // We'll read this from the parameter manager
    };

    // Set current note for display
    static int currentNote;
    currentNote = sequencer->getCurrentNote();
    displayState.midiNotes = &currentNote;
    displayState.numNotes = 1; // We're showing just the current note

    ::updateDisplay(displayState); // Call global function with :: prefix
}

void Application::start()
{
    if (sequencer)
    {
        sequencer->start();
    }
}

void Application::stop()
{
    if (sequencer)
    {
        sequencer->stop();
    }
}

void Application::reset()
{
    if (sequencer)
    {
        sequencer->reset();
        updateCVOutput(); // Immediately update CV to first note
    }
}

bool Application::isPlaying() const
{
    return sequencer ? sequencer->isPlaying() : false;
}

void Application::shutdown()
{
    cleanup();
    isInitialized = false;
}

void Application::cleanup()
{
    delete ledManager;
    delete parameterManager;
    delete sequencer;
    delete clock;
    delete currentPattern;

    ledManager = nullptr;
    parameterManager = nullptr;
    sequencer = nullptr;
    clock = nullptr;
    currentPattern = nullptr;
}
