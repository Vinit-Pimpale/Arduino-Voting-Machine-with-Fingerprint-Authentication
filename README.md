# Arduino Voting Machine with Fingerprint Authentication

## Overview
This Arduino-based voting machine project integrates fingerprint authentication to ensure secure and tamper-proof voting processes. It utilizes Adafruit's fingerprint sensor module for biometric identification and an EEPROM for storing voter records. The system supports up to 25 voters and displays voting results on a 16x2 LCD screen in real-time.

## Features
Fingerprint Authentication: Voters must authenticate using their registered fingerprints before casting their votes.
LCD Display: Provides interactive prompts and displays voting instructions, voter status, and final voting results.
Secure Voting: Prevents unauthorized voting and ensures each voter can cast only one vote.
Multiple Candidate Support: Supports voting for up to three candidates with dynamic vote tallying and result computation.
Reset Functionality: Includes a system reset option to clear all voting records and start fresh.

## Components Used
- Arduino UNO: Main microcontroller managing the entire voting process.
- Adafruit Fingerprint Sensor: For secure voter authentication using biometric fingerprints.
- 16x2 LCD Display: Displays system status, voter prompts, and final voting results.
- EEPROM: Stores voter IDs and vote counts persistently.

## Setup and Usage
To use the voting machine:

1. Initialization: Connect all components as per the pin configuration specified in the code.
2. Enrollment: Admins can enroll voters using the fingerprint sensor and manage voter records via the Arduino IDE.
3. Voting Process: Voters authenticate themselves with their fingerprint, cast their votes using push buttons, and view immediate results on the LCD display.
4. Result Display: After voting ends, the system displays the winner or indicates a tie based on the vote counts.

## Future Improvements
- Integration with a real-time clock (RTC) for scheduling voting sessions.
- Incorporation of cryptographic techniques for enhanced security.
- Remote monitoring and reporting capabilities for election authorities.
