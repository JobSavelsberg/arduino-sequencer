#ifndef APPLICATION_H
#define APPLICATION_H

#include <Arduino.h>
#include "core/sequencer.h"
#include "core/pattern.h"
#include "core/clock.h"
#include "utils/parameter_manager.h"
#include "utils/led_manager.h"
#include "hardware/cv_output.h"
#include "hardware/display.h"

/**
 * Main application controller
 * Coordinates all subsystems and handles the main application logic
 */
class Application
{
private:
    // Core components
    Sequencer *sequencer;
    Pattern *currentPattern;
    Clock *clock;
    ParameterManager *parameterManager;
    LedManager *ledManager;

    // Application state
    bool isInitialized;
    unsigned long lastUpdateTime;

    // Default pattern data
    static int defaultNotes[];
    static const int defaultNoteCount;
    static const int BASE_NOTE;

public:
    Application();
    ~Application();

    // Lifecycle
    bool initialize();
    void update();
    void shutdown();

    // Control interface
    void start();
    void stop();
    void reset();
    bool isPlaying() const;

    // Access to components (for advanced control if needed)
    Sequencer *getSequencer() const { return sequencer; }
    ParameterManager *getParameterManager() const { return parameterManager; }
    LedManager *getLedManager() const { return ledManager; }

private:
    void handleUserInput();
    void updateCVOutput();
    void updateLEDs();
    void updateDisplay();
    void cleanup();
};

#endif
