#include "core/clock.h"

Clock::Clock(float initialBpm) : bpm(initialBpm), lastTickTime(0), running(false)
{
    updateTickInterval();
}

void Clock::updateTickInterval()
{
    if (bpm > 0)
    {
        tickInterval = (60.0 / bpm) * 1000.0; // Convert to milliseconds
    }
    else
    {
        tickInterval = 1000; // Default 1 second if invalid BPM
    }
}

void Clock::start()
{
    running = true;
    lastTickTime = millis(); // Reset timing when starting
}

void Clock::stop()
{
    running = false;
}

void Clock::reset()
{
    lastTickTime = millis();
}

bool Clock::isRunning() const
{
    return running;
}

void Clock::setBpm(float newBpm)
{
    if (isValidBpm(newBpm))
    {
        bpm = newBpm;
        updateTickInterval();
    }
}

float Clock::getBpm() const
{
    return bpm;
}

bool Clock::shouldTick(unsigned long currentTime)
{
    if (!running)
    {
        return false;
    }

    if (currentTime - lastTickTime >= tickInterval)
    {
        lastTickTime = currentTime;
        return true;
    }

    return false;
}

unsigned long Clock::getLastTickTime() const
{
    return lastTickTime;
}

unsigned long Clock::getTickInterval() const
{
    return tickInterval;
}

float Clock::getNoteDurationMs() const
{
    return tickInterval;
}

bool Clock::isValidBpm(float bpm)
{
    return bpm >= 30.0 && bpm <= 300.0; // Reasonable BPM range
}
