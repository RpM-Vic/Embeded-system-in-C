# Microcontrollers final test (2017 Archive)

## ⚠️ Important: A Time Capsule

**For my more recent work with modern technologies (Angular, PostgreSQL, Bun, GraphQL, etc.), please explore my other repositories.**

This repository contains original source code from my early engineering work in 2017. It is preserved as a historical record and was never intended for production use.

## Personal note

**These things may mean nothing for most people but I see many achievements here:**

There are differences between simulating the functionality in proteus vs actually making things work in real life. This code was 100% functional back in 2017. The comments may be innacurate but all the code was meant to do something, they are not just random words.

There was no AI to help with bugs, only perceverance.

I was studying the 5th semester of mechatronics enginerring a subject supposed to be for 7th semester. 

Only 2 out of 20 students approved, me and my teammate, the teachers were wild then. 

In the words of my teammate, he considered me a genius for managing to write functional code in complex applications with "very few lines".

As students, we faced budget limitations, mobility constraints, and in many cases, sleep deprivation. This code is proof that it's possible to build something meaningful despite those challenges.

## Getting Started (If You Really Want to Dig In)

*Note: As this is archived code from 2017, it may require older toolchains or specific hardware to run. It's provided for reference and historical interest.*

## 🔧 Technical Implementation Highlights


### Real-time Interrupt Structure
Three hardware timers configured for:
- Timer0: 128µs resolution (milisecond counting)
- Timer1: 32.7ms overflow (seconds counting)
- Timer2: 0.5µs interrupt (high-resolution timing)

### EEPROM Management
Persistent storage of:
- Level setpoints (16-bit values split across two addresses)
- Temperature parameters
- Pump cycle counters
- System state

### Control Logic
- Hysteresis-based temperature control with visual feedback
- Pump alternation algorithm for extended motor life
- Configurable consumption parameters
- Interlock conditions to prevent pump damage


### Hardware Requirements
- PIC18f4550 programmer (PICKIT or compatible)
- RS232 port or USB-to-Serial adapter
- Custom PCB or breadboard setup (schematics not included - 2017 organization)
- Power supply: 5V for logic
- Bluetooth module (optional, HC-05/HC-06 compatible)

### Software Requirements
- CCS C Compiler (version from ~2017)
- LabVIEW 2010 or later
- Windows with COM port access

### Configuration
1. Burn the firmware using CCS C
2. Configure hardware connections per pin definitions in code
3. Set up LabVIEW HMI (contact for .vi files)
4. Connect via RS232 at 9600 baud
5. Pray to the electronics gods (optional but recommended)

## 🧪 Testing & Validation

The system was **100% functional in real-world conditions**, controlling:
- Water level in main tank
- Cisterna level monitoring
- Temperature regulation via thyristor-fired resistive element
- Pump activation/deactivation based on setpoints

All parameters could be modified both locally (keypad) and remotely (LabVIEW), with changes persisted to EEPROM.



## 📚 Educational Value

This project demonstrates:
- Real-time embedded systems programming
- Hardware-software integration
- Custom protocol design
- Hysteresis control implementation
- EEPROM management in resource-constrained environments
- Practical interrupt handling
- Team collaboration (1 firmware, 1 HMI/hardware)

 Embedded C, control logic, interrupt handling, protocol implementation, LabVIEW interface, physical connections, sensor integration


## 📜 License

MIT License - Feel free to learn from, modify, or improve. If you burn components, that's on you.
