# Arduino Sequencer

An Arduino-based control voltage sequencer for analog synthesizers. Features a clean, modular architecture with precise 1V per octave control voltages using PWM output + filtering.

## Features

-   **Musical Sequencing**: 16-step musical patterns with MIDI note support
-   **Real-time Controls**: BPM control (60-200 BPM) and modulation CV via potentiometers
-   **Visual Feedback**: LED beat indicators and OLED display
-   **Precise CV Output**: 1V per octave control voltages using 20 kHz PWM (inaudible)
-   **Modular Architecture**: Clean, object-oriented design for easy extension
-   **User Interface**: Reset button, tempo control, and real-time parameter adjustment

## Architecture

This project has been refactored from a monolithic design to a clean, layered architecture:

-   **Application Layer**: Main application controller and pattern management
-   **Service Layer**: Sequencer engine, clock manager, parameter handling, LED control
-   **Hardware Abstraction**: CV output, display, and hardware controls
-   **Platform Layer**: Arduino hardware APIs

For detailed architecture documentation, see [ARCHITECTURE.md](ARCHITECTURE.md).

## Hardware & Circuit

Hardware components:

-   Arduino Uno R3
-   128x64 OLED Display (I2C)
-   2 Potentiometers (BPM and Modulation)
-   1 Reset Button
-   2 LEDs (Beat and Downbeat indicators)

### CV Output Circuit

The PWM signal is converted to clean analog CV using:

1. Two cascaded RC low-pass filters (10kΩ + 100nF each) to smooth the PWM
2. Op-amp buffer for clean output and current drive capability

### Pin Assignments

-   **CV Outputs**: Pin 9 (Pitch), Pin 10 (Modulation)
-   **Potentiometers**: A2 (BPM), A3 (Modulation)
-   **Button**: Pin 2 (Reset)
-   **LEDs**: Pin 12 (Beat), Pin 13 (Downbeat)
-   **Display**: I2C (SDA/SCL)

## Getting Started with PlatformIO

This project uses PlatformIO for development and deployment.

1. [Install PlatformIO](https://platformio.org/install) (VS Code extension recommended)
2. Connect your Arduino Uno via USB
3. Use the PlatformIO interface to build and upload, or run `pio run --target upload` from the command line

For detailed instructions, see the [PlatformIO Quick Start Guide](https://docs.platformio.org/en/latest/integration/ide/vscode.html#quick-start).

## Usage

### Controls

-   **Potentiometer A**: Adjust BPM (60-200)
-   **Potentiometer B**: Control modulation CV level (0-5V)
-   **Button A**: Reset sequence to beginning
-   **LED A**: Downbeat indicator (first step of pattern)
-   **LED B**: Beat indicator (every step)

### Operation

1. Power on the Arduino
2. The sequencer automatically starts playing the default pattern
3. Adjust tempo with the BPM potentiometer
4. Control modulation depth with the modulation potentiometer
5. Press the reset button to restart the sequence from the beginning

## Code Structure

### Key Classes

-   **`Application`**: Main application controller
-   **`Sequencer`**: Core sequencing logic and playback
-   **`Pattern`**: Musical pattern/sequence management
-   **`Clock`**: Timing and tempo control
-   **`ParameterManager`**: User input handling with debouncing
-   **`LedManager`**: Non-blocking LED control

### Extension Points

The modular architecture makes it easy to add:

-   Multiple pattern banks
-   Pattern editing capabilities
-   MIDI input/output
-   Different timing modes
-   Advanced display features
-   Save/load functionality

## Future Ideas

-   **Pattern Banks** - Store and switch between multiple patterns
-   **MIDI Integration** - MIDI input/output for synchronization
-   **Pattern Editor** - Real-time sequence editing
-   **Swing/Shuffle** - Advanced timing options
-   **Multi-CV Outputs** - Additional control voltage outputs
-   **External Clock** - Sync to external clock sources
