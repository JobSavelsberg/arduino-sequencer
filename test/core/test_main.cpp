#include <unity.h>
#include "core/pattern.h"
#include "core/clock.h"
#include "core/sequencer.h"

// Test data
int testNotes[] = {36, 40, 43, 48}; // C, E, G, C (one octave)
const int testNoteCount = 4;

void setUp(void)
{
    // This is run before each test
}

void tearDown(void)
{
    // This is run after each test
}

// Pattern Tests
void test_pattern_creation()
{
    Pattern pattern(testNotes, testNoteCount);

    TEST_ASSERT_EQUAL(testNoteCount, pattern.getLength());
    TEST_ASSERT_EQUAL(36, pattern.getNote(0));
    TEST_ASSERT_EQUAL(48, pattern.getNote(3));
}

void test_pattern_navigation()
{
    Pattern pattern(testNotes, testNoteCount);

    // Test forward navigation
    TEST_ASSERT_EQUAL(1, pattern.getNextIndex(0));
    TEST_ASSERT_EQUAL(2, pattern.getNextIndex(1));
    TEST_ASSERT_EQUAL(0, pattern.getNextIndex(3)); // Wrap around

    // Test backward navigation
    TEST_ASSERT_EQUAL(3, pattern.getPreviousIndex(0)); // Wrap around
    TEST_ASSERT_EQUAL(0, pattern.getPreviousIndex(1));
    TEST_ASSERT_EQUAL(2, pattern.getPreviousIndex(3));
}

void test_pattern_validation()
{
    Pattern pattern(testNotes, testNoteCount);

    TEST_ASSERT_TRUE(pattern.isValidIndex(0));
    TEST_ASSERT_TRUE(pattern.isValidIndex(3));
    TEST_ASSERT_FALSE(pattern.isValidIndex(4));
    TEST_ASSERT_FALSE(pattern.isValidIndex(-1));

    TEST_ASSERT_TRUE(pattern.isValidMidiNote(0));
    TEST_ASSERT_TRUE(pattern.isValidMidiNote(127));
    TEST_ASSERT_FALSE(pattern.isValidMidiNote(-1));
    TEST_ASSERT_FALSE(pattern.isValidMidiNote(128));
}

// Clock Tests
void test_clock_bpm()
{
    Clock clock(120.0);

    TEST_ASSERT_EQUAL_FLOAT(120.0, clock.getBpm());
    TEST_ASSERT_EQUAL(500, clock.getTickInterval()); // 120 BPM = 500ms per beat

    clock.setBpm(60.0);
    TEST_ASSERT_EQUAL_FLOAT(60.0, clock.getBpm());
    TEST_ASSERT_EQUAL(1000, clock.getTickInterval()); // 60 BPM = 1000ms per beat
}

void test_clock_validation()
{
    Clock clock(120.0);

    // Valid BPM range
    TEST_ASSERT_TRUE(Clock::isValidBpm(60.0));
    TEST_ASSERT_TRUE(Clock::isValidBpm(120.0));
    TEST_ASSERT_TRUE(Clock::isValidBpm(200.0));

    // Invalid BPM range
    TEST_ASSERT_FALSE(Clock::isValidBpm(29.0));
    TEST_ASSERT_FALSE(Clock::isValidBpm(301.0));
    TEST_ASSERT_FALSE(Clock::isValidBpm(0.0));
    TEST_ASSERT_FALSE(Clock::isValidBpm(-1.0));
}

void test_clock_state()
{
    Clock clock(120.0);

    TEST_ASSERT_FALSE(clock.isRunning());

    clock.start();
    TEST_ASSERT_TRUE(clock.isRunning());

    clock.stop();
    TEST_ASSERT_FALSE(clock.isRunning());
}

// Sequencer Tests
void test_sequencer_creation()
{
    Pattern pattern(testNotes, testNoteCount);
    Clock clock(120.0);
    Sequencer sequencer(&pattern, &clock, 36);

    TEST_ASSERT_EQUAL(0, sequencer.getCurrentNoteIndex());
    TEST_ASSERT_EQUAL(36, sequencer.getCurrentNote()); // First note
    TEST_ASSERT_EQUAL(36, sequencer.getBaseNote());
}

void test_sequencer_navigation()
{
    Pattern pattern(testNotes, testNoteCount);
    Clock clock(120.0);
    Sequencer sequencer(&pattern, &clock, 36);

    // Step forward
    sequencer.stepForward();
    TEST_ASSERT_EQUAL(1, sequencer.getCurrentNoteIndex());
    TEST_ASSERT_EQUAL(40, sequencer.getCurrentNote()); // Second note (E)

    // Step forward to end and wrap
    sequencer.goToStep(3);
    TEST_ASSERT_EQUAL(3, sequencer.getCurrentNoteIndex());
    TEST_ASSERT_EQUAL(48, sequencer.getCurrentNote()); // Last note (C)

    sequencer.stepForward();
    TEST_ASSERT_EQUAL(0, sequencer.getCurrentNoteIndex());
    TEST_ASSERT_EQUAL(36, sequencer.getCurrentNote()); // Wrapped to first note
}

void test_sequencer_validation()
{
    Pattern pattern(testNotes, testNoteCount);
    Clock clock(120.0);
    Sequencer sequencer(&pattern, &clock, 36);

    TEST_ASSERT_TRUE(sequencer.isValidBaseNote(0));
    TEST_ASSERT_TRUE(sequencer.isValidBaseNote(127));
    TEST_ASSERT_FALSE(sequencer.isValidBaseNote(-1));
    TEST_ASSERT_FALSE(sequencer.isValidBaseNote(128));
}

// Main test runner
int main()
{
    UNITY_BEGIN();

    // Pattern tests
    RUN_TEST(test_pattern_creation);
    RUN_TEST(test_pattern_navigation);
    RUN_TEST(test_pattern_validation);

    // Clock tests
    RUN_TEST(test_clock_bpm);
    RUN_TEST(test_clock_validation);
    RUN_TEST(test_clock_state);

    // Sequencer tests
    RUN_TEST(test_sequencer_creation);
    RUN_TEST(test_sequencer_navigation);
    RUN_TEST(test_sequencer_validation);

    return UNITY_END();
}
