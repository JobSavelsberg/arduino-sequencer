#ifndef CLOCK_H
#define CLOCK_H

#include <Arduino.h>

/**
 * Handles timing and tempo for the sequencer
 * Provides precise timing control and tempo management
 */
class Clock
{
private:
    float bpm;
    unsigned long lastTickTime;
    unsigned long tickInterval;
    bool running;

    void updateTickInterval();

public:
    Clock(float initialBpm = 120.0);

    // Timing control
    void start();
    void stop();
    void reset();
    bool isRunning() const;

    // Tempo management
    void setBpm(float newBpm);
    float getBpm() const;

    // Tick checking
    bool shouldTick(unsigned long currentTime);
    unsigned long getLastTickTime() const;
    unsigned long getTickInterval() const;

    // Utility functions
    float getNoteDurationMs() const;
    static bool isValidBpm(float bpm);
};

#endif
