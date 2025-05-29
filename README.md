# Arduino Sequencer

An Arduino-based control voltage sequencer for analog synthesizers. Generates precise 1V per octave control voltages using PWM output + filtering.

## Features

-   Generates 1V per octave control voltages
-   20 kHz PWM frequency (inaudible)
-   Cycles through 5 octaves automatically
-   Built with PlatformIO for Arduino Uno

## Hardware & Circuit

I used an Arduino Uno R3 with the following connections:

-   PWM output on pin 9
-   `GND` connected to the circuit ground
-   `Vin` connected to the positive supply (12V)

The PWM signal is converted to clean analog CV using:

1. Two cascaded RC low-pass filters (10kΩ + 100nF each) to smooth the PWM
2. Op-amp buffer for clean output and current drive capability (positive supply at 12V, negative at GND)

## Getting Started with PlatformIO

This project uses PlatformIO for development and deployment.

1. [Install PlatformIO](https://platformio.org/install) (VS Code extension recommended)
2. Connect your Arduino Uno via USB
3. Use the PlatformIO interface to build and upload, or run `pio run --target upload` from the command line

For detailed instructions, see the [PlatformIO Quick Start Guide](https://docs.platformio.org/en/latest/integration/ide/vscode.html#quick-start).

## Usage

Currently runs an automatic sequence cycling through musical notes with appropriate control voltages based on the 1V per octave standard.

## Future Ideas

-   **MIDI file support** - Load and play back MIDI sequences
-   **Real-time MIDI input** - Live MIDI control and sequencing
-   **Standalone sequence editing** - Ability to create and modify sequences in standalone mode with some knobs/buttons and a display
