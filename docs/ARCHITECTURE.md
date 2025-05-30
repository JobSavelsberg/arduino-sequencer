# Arduino Sequencer - Refactored Architecture

## Overview

This document describes the refactored architecture of the Arduino sequencer application. The original monolithic design has been restructured into a layered, object-oriented architecture that addresses the previously identified architectural problems.

## New Architecture

### Layer Structure

```
┌─────────────────────────────────────────┐
│           Application Layer             │
│  ┌─────────────┐ ┌─────────────────────┐ │
│  │ Application │ │   Pattern/Sequence  │ │
│  │ Controller  │ │    Management       │ │
│  └─────────────┘ └─────────────────────┘ │
└─────────────────────────────────────────┘
┌─────────────────────────────────────────┐
│            Service Layer                │
│ ┌───────────┐ ┌──────────┐ ┌──────────┐ │
│ │ Sequencer │ │  Clock   │ │Parameter │ │
│ │  Engine   │ │ Manager  │ │ Manager  │ │
│ └───────────┘ └──────────┘ └──────────┘ │
│ ┌─────────────────────────────────────┐ │
│ │        LED Manager                  │ │
│ └─────────────────────────────────────┘ │
└─────────────────────────────────────────┘
┌─────────────────────────────────────────┐
│     Hardware Abstraction Layer         │
│ ┌──────────┐ ┌─────────┐ ┌────────────┐ │
│ │CV Output │ │Display  │ │ Hardware   │ │
│ │ System   │ │Manager  │ │ Controls   │ │
│ └──────────┘ └─────────┘ └────────────┘ │
└─────────────────────────────────────────┘
┌─────────────────────────────────────────┐
│           Platform Layer                │
│        Arduino Hardware APIs           │
└─────────────────────────────────────────┘
```

## Core Components

### 1. Pattern Class

**File**: `include/pattern.h`, `src/pattern.cpp`

-   **Purpose**: Encapsulates musical sequences/patterns
-   **Features**:
    -   Memory-safe note storage
    -   Pattern navigation (next/previous)
    -   MIDI note validation
    -   Copy semantics for pattern manipulation

### 2. Clock Class

**File**: `include/clock.h`, `src/clock.cpp`

-   **Purpose**: Handles timing and tempo control
-   **Features**:
    -   Precise BPM management
    -   Start/stop/reset functionality
    -   Tick generation for sequencer timing
    -   BPM validation and constraints

### 3. Sequencer Class

**File**: `include/sequencer.h`, `src/sequencer.cpp`

-   **Purpose**: Core sequencing logic and playback control
-   **Features**:
    -   Pattern playback management
    -   Note progression logic
    -   Integration with Clock for timing
    -   Step navigation (forward/backward/goto)

### 4. ParameterManager Class

**File**: `include/parameter_manager.h`, `src/parameter_manager.cpp`

-   **Purpose**: Handles user input with debouncing and smoothing
-   **Features**:
    -   Button debouncing
    -   Parameter smoothing for potentiometers
    -   Edge detection (button press/release events)
    -   Non-blocking input handling

### 5. LedManager Class

**File**: `include/led_manager.h`, `src/led_manager.cpp`

-   **Purpose**: Non-blocking LED control with timing
-   **Features**:
    -   Temporary LED activation with auto-off
    -   Beat and downbeat indication
    -   Multiple LED state management
    -   Time-based LED control

### 6. Application Class

**File**: `include/application.h`, `src/application.cpp`

-   **Purpose**: Main application controller and coordinator
-   **Features**:
    -   Component lifecycle management
    -   Subsystem coordination
    -   Main application loop logic
    -   Resource cleanup

## Key Improvements

### 1. Separation of Concerns

-   **Before**: All logic mixed in main loop
-   **After**: Clear separation between timing, sequencing, input handling, and output

### 2. State Management

-   **Before**: Global state structure
-   **After**: Encapsulated state within appropriate classes

### 3. Error Handling

-   **Before**: No validation or error handling
-   **After**: Input validation, bounds checking, and safe defaults

### 4. Modularity

-   **Before**: Tightly coupled components
-   **After**: Loosely coupled with clear interfaces

### 5. Testability

-   **Before**: Monolithic code hard to test
-   **After**: Individual components can be tested in isolation

### 6. Resource Management

-   **Before**: No memory management
-   **After**: RAII principles with proper constructors/destructors

## Usage

### Basic Operation

The refactored system maintains the same external behavior:

-   Potentiometer A controls BPM (60-200)
-   Potentiometer B controls modulation CV
-   Button A resets sequence to beginning
-   LEDs indicate beats and downbeats
-   Display shows current note and BPM

### Extension Points

The new architecture makes it easy to add:

-   Multiple patterns
-   Pattern editing
-   MIDI input/output
-   Different timing modes
-   Advanced display features
-   Save/load functionality

## Benefits

1. **Maintainability**: Clear code organization and separation of concerns
2. **Testability**: Individual components can be unit tested
3. **Extensibility**: Easy to add new features without affecting existing code
4. **Reliability**: Better error handling and input validation
5. **Performance**: More efficient resource usage and non-blocking operations
6. **Readability**: Self-documenting code with clear class responsibilities

## Migration Notes

-   The `SequencerState` struct is now only used for display compatibility
-   All sequencer logic has been moved to dedicated classes
-   The main.cpp file is now minimal and focused on application lifecycle
-   Hardware abstraction remains unchanged for compatibility

## Future Enhancements

With this architecture, the following features can be easily added:

-   Pattern banks and switching
-   Real-time pattern editing
-   MIDI synchronization
-   Swing/shuffle timing
-   Multiple CV outputs
-   Step recording
-   Pattern length variation
-   Tempo automation
