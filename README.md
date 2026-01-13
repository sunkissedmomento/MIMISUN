# MIMISUN - UVB Timer System

UVB exposure timer with precise timing control and safety features for infant phototherapy using UVB 10.0 lamps.

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Hardware Requirements](#hardware-requirements)
- [Pin Configuration](#pin-configuration)
- [Installation](#installation)
- [Configuration](#configuration)
- [Usage](#usage)
- [Timer Presets](#timer-presets)
- [LCD Display Information](#lcd-display-information)
- [Serial Monitor](#serial-monitor)
- [Troubleshooting](#troubleshooting)
- [System Behavior](#system-behavior)
- [Safety Information](#safety-information)
- [License](#license)

## Overview

MIMISUN is a precision UVB exposure timer system designed for safe and controlled phototherapy sessions for infants. The system uses UVB 10.0 lamps with predefined time intervals (3, 5, and 10 minutes) to ensure consistent and safe exposure levels. Features include automatic shutoff, audible completion alerts, and a clear LCD display for real-time monitoring.

**Safety Notice:** This device is designed for controlled UVB exposure under proper supervision. Always follow medical guidelines for infant phototherapy.

## Features

- Preset Timer Options - 3, 5, and 10-minute sessions
- Automatic Shutoff - Lamp turns off automatically when time expires
- LCD Display - Real-time countdown and status information
- Audible Alerts - Triple beep notification on completion
- Manual Control - Start/stop functionality at any time
- Serial Debug - Test mode for development and verification
- Safety Relay - Active-low relay for fail-safe operation
- Simple Interface - Four buttons for easy operation

## Hardware Requirements

### Components

| Component | Specification | Quantity |
|-----------|--------------|----------|
| Microcontroller | Arduino Uno or compatible | 1 |
| LCD Display | 16x2 I2C LCD (0x27 address) | 1 |
| Relay Module | 5V single channel (active-low) | 1 |
| Buzzer | 5V active buzzer | 1 |
| Push Buttons | Momentary tactile switches | 4 |
| UVB Lamp | UVB 10.0 phototherapy lamp | 1 |
| Power Supply | Appropriate for lamp voltage | 1 |
| Jumper Wires | Male-to-male/female | As needed |

### Optional Components

- Enclosure for electronics
- Emergency stop switch
- Timer start indicator LED
- External power adapter

## Pin Configuration

```
Arduino Uno Pinout:
┌─────────────────────────────┐
│                             │
│  D2  ──────  3min Button    │  Preset: 3 minutes
│  D3  ──────  5min Button    │  Preset: 5 minutes
│  D4  ──────  10min Button   │  Preset: 10 minutes
│  D5  ──────  Start/Stop     │  Control button
│  D8  ──────  Relay          │  Lamp control (active-low)
│  D9  ──────  Buzzer         │  Audio alert
│  A4  ──────  SDA (I2C)      │  LCD data
│  A5  ──────  SCL (I2C)      │  LCD clock
│  GND ──────  Common Ground  │  All components
│  5V  ──────  Logic Power    │  Buttons, LCD, buzzer
│                             │
└─────────────────────────────┘
```

### Pin Details

| Pin | Function | Description |
|-----|----------|-------------|
| D2 | btn3s | 3-minute preset button (INPUT_PULLUP) |
| D3 | btn5s | 5-minute preset button (INPUT_PULLUP) |
| D4 | btn10s | 10-minute preset button (INPUT_PULLUP) |
| D5 | btnStartStop | Start/stop timer control (INPUT_PULLUP) |
| D8 | relayPin | Relay control - ACTIVE-LOW (LOW = ON) |
| D9 | buzzerPin | Buzzer output for completion alert |
| A4 | SDA | I2C data line for LCD |
| A5 | SCL | I2C clock line for LCD |

### Wiring Diagram

```
LCD Display (I2C):
  VCC  →  5V (Arduino)
  GND  →  GND (Arduino)
  SDA  →  A4 (Arduino)
  SCL  →  A5 (Arduino)

Relay Module (Active-Low):
  VCC  →  5V (Arduino)
  GND  →  GND (Arduino)
  IN   →  D8 (Arduino)
  COM  →  Lamp AC/DC power source
  NO   →  Lamp positive wire

Buzzer:
  +    →  D9 (Arduino)
  -    →  GND (Arduino)

Buttons (with internal pullup):
  3min Button    → D2 to GND
  5min Button    → D3 to GND
  10min Button   → D4 to GND
  Start/Stop     → D5 to GND

UVB Lamp Connection:
  Power Source → Relay COM
  Relay NO → Lamp positive
  Lamp negative → Power ground
```

## Installation

### 1. Arduino IDE Setup

1. Download and install [Arduino IDE](https://www.arduino.cc/en/software) (v1.8.19 or later)
2. Launch Arduino IDE
3. Verify installation

### 2. Install Required Libraries

Install via `Sketch → Include Library → Manage Libraries`:

- **LiquidCrystal I2C** by Frank de Brabander
- **Wire** (included by default)

**Installation steps:**
1. Open Library Manager: `Sketch → Include Library → Manage Libraries`
2. Search for "LiquidCrystal I2C"
3. Click "Install" on the library by Frank de Brabander
4. Close Library Manager

### 3. Hardware Assembly

1. **Connect LCD Display:**
   - Wire LCD I2C module to A4 (SDA) and A5 (SCL)
   - Connect VCC to 5V, GND to GND

2. **Connect Relay Module:**
   - Wire relay IN to D8
   - Connect relay power (VCC/GND)
   - **Important:** This is an ACTIVE-LOW relay
     - D8 LOW = Relay ON (lamp ON)
     - D8 HIGH = Relay OFF (lamp OFF)

3. **Connect Buttons:**
   - Wire all buttons to connect pin to GND
   - No external resistors needed (using INPUT_PULLUP)

4. **Connect Buzzer:**
   - Positive to D9
   - Negative to GND

5. **Wire UVB Lamp:**
   - Connect lamp power through relay NO contact
   - Ensure proper voltage rating
   - Use appropriate gauge wire for lamp current

### 4. Verify LCD Address

**Find I2C address:**

1. Upload I2C scanner sketch (available in examples)
2. Open Serial Monitor at 9600 baud
3. Note displayed address (usually 0x27 or 0x3F)
4. If different, update code:
   ```cpp
   LiquidCrystal_I2C lcd(0x27, 16, 2);  // Change 0x27 if needed
   ```

### 5. Upload Sketch

1. Connect Arduino to computer via USB
2. Select board: `Tools → Board → Arduino Uno`
3. Select port: `Tools → Port → [Your COM/Serial Port]`
4. Click **Upload** button (→)
5. Wait for "Done uploading" message

### 6. Verify Installation

1. Open Serial Monitor: `Tools → Serial Monitor`
2. Set baud rate to **9600**
3. Verify startup message:
   ```
   UVB TIMER - TEST MODE
   1=3s 2=5s 3=10s s=Start/Stop x=Stop
   ```
4. LCD should display:
   ```
   UVB TIMER
   Select Time
   ```

## Configuration

### Timer Presets

The code is configured for testing with short durations. **For production use, update to full minutes:**

```cpp
// CURRENT (TEST MODE):
if (cmd == '1') { setTime = 3000; showTime(3); }    // 3 seconds
if (cmd == '2') { setTime = 5000; showTime(5); }    // 5 seconds
if (cmd == '3') { setTime = 10000; showTime(10); }  // 10 seconds

// CHANGE TO (PRODUCTION):
if (cmd == '1') { setTime = 180000; showTime(3); }   // 3 minutes
if (cmd == '2') { setTime = 300000; showTime(5); }   // 5 minutes
if (cmd == '3') { setTime = 600000; showTime(10); }  // 10 minutes
```

**Also update button handlers:**

```cpp
// CURRENT (TEST MODE):
if (digitalRead(btn3s) == LOW) {
  setTime = 3000;     // Change to 180000 for 3 minutes
  showTime(3);
  lastPress = now;
}
if (digitalRead(btn5s) == LOW) {
  setTime = 5000;     // Change to 300000 for 5 minutes
  showTime(5);
  lastPress = now;
}
if (digitalRead(btn10s) == LOW) {
  setTime = 10000;    // Change to 600000 for 10 minutes
  showTime(10);
  lastPress = now;
}
```

### Time Conversion Reference

| Duration | Milliseconds | Code Value |
|----------|--------------|------------|
| 3 minutes | 180,000 ms | `180000` |
| 5 minutes | 300,000 ms | `300000` |
| 10 minutes | 600,000 ms | `600000` |
| 15 minutes | 900,000 ms | `900000` |
| 20 minutes | 1,200,000 ms | `1200000` |

### Debounce Adjustment

Modify if buttons are too sensitive or unresponsive:

```cpp
const unsigned long debounceDelay = 250;  // Default: 250ms
```

Increase value (e.g., 300-500ms) if experiencing multiple triggers.

### LCD Address

If LCD doesn't display, try alternative addresses:

```cpp
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Common address
// OR
LiquidCrystal_I2C lcd(0x3F, 16, 2);  // Alternative address
```

## Usage

### First Time Setup

1. **Power on the system**
   - Connect Arduino via USB or external power
   - LCD displays startup message
   - Relay is OFF (lamp is OFF)

2. **Initial display:**
   ```
   LCD Line 1: UVB TIMER
   LCD Line 2: Select Time
   ```

3. **Serial Monitor (optional):**
   ```
   UVB TIMER - TEST MODE
   1=3s 2=5s 3=10s s=Start/Stop x=Stop
   ```

### Normal Operation

**Step 1: Select Duration**
- Press one of three preset buttons:
  - **3min Button (D2):** Set 3-minute session
  - **5min Button (D3):** Set 5-minute session
  - **10min Button (D4):** Set 10-minute session

**Display shows:**
```
Set Time:
3 min
```

**Step 2: Start Timer**
- Press **Start/Stop Button (D5)**
- Relay activates (lamp turns ON)
- Countdown begins

**Display shows:**
```
RUNNING
Left: 180 sec
```

**Step 3: During Session**
- LCD updates countdown every second
- Lamp remains on
- Time decreases: 180 → 179 → 178...

**Step 4: Completion**
- Timer reaches zero
- Lamp automatically turns OFF
- Buzzer sounds 3 beeps
- Display returns to ready state

**Display shows:**
```
DONE
Select Time
```

### Manual Stop

**To stop timer early:**
- Press **Start/Stop Button (D5)** during session
- Lamp immediately turns OFF
- Timer resets
- System returns to ready state

### Test Mode (Serial Commands)

With Serial Monitor open at 9600 baud:

| Command | Action |
|---------|--------|
| `1` | Set 3-minute timer (or 3 seconds in test mode) |
| `2` | Set 5-minute timer (or 5 seconds in test mode) |
| `3` | Set 10-minute timer (or 10 seconds in test mode) |
| `s` | Start/Stop timer |
| `x` | Emergency stop (immediate shutoff) |

## Timer Presets

### 3-Minute Session

**Use Case:** Short exposure for sensitive skin or initial sessions

**Operation:**
1. Press 3min button
2. LCD shows "Set Time: 3 min"
3. Press Start/Stop
4. Countdown from 180 seconds
5. Auto-shutoff at completion

### 5-Minute Session

**Use Case:** Standard phototherapy session

**Operation:**
1. Press 5min button
2. LCD shows "Set Time: 5 min"
3. Press Start/Stop
4. Countdown from 300 seconds
5. Auto-shutoff at completion

### 10-Minute Session

**Use Case:** Extended therapy for specific conditions

**Operation:**
1. Press 10min button
2. LCD shows "Set Time: 10 min"
3. Press Start/Stop
4. Countdown from 600 seconds
5. Auto-shutoff at completion

## LCD Display Information

### Display States

**Idle State:**
```
┌────────────────┐
│UVB TIMER       │  Line 1: System name
│Select Time     │  Line 2: Status message
└────────────────┘
```

**Time Selected:**
```
┌────────────────┐
│Set Time:       │  Line 1: Prompt
│3 min           │  Line 2: Selected duration
└────────────────┘
```

**Running State:**
```
┌────────────────┐
│RUNNING         │  Line 1: Status
│Left: 180 sec   │  Line 2: Countdown timer
└────────────────┘
```

**Completion State:**
```
┌────────────────┐
│DONE            │  Line 1: Completion message
│Select Time     │  Line 2: Ready for next session
└────────────────┘
```

### Information Display

| LCD Element | Meaning |
|-------------|---------|
| "UVB TIMER" | System identifier |
| "Select Time" | Waiting for user input |
| "Set Time: X min" | Duration selected |
| "RUNNING" | Active session in progress |
| "Left: XXX sec" | Remaining time in seconds |
| "DONE" | Session completed |

## Serial Monitor

### Startup Output

```
UVB TIMER - TEST MODE
1=3s 2=5s 3=10s s=Start/Stop x=Stop
```

### Event Messages

**Time Selection:**
```
TIME SET: 3 sec
TIME SET: 5 sec
TIME SET: 10 sec
```

**Timer Control:**
```
TIMER STARTED
TIMER STOPPED
EMERGENCY STOP
```

### Debug Information

Serial Monitor provides real-time feedback useful for:
- Testing system functionality
- Verifying button presses
- Debugging relay operation
- Monitoring timer accuracy

## Troubleshooting

### LCD Display Issues

**Problem:** LCD shows nothing or garbled characters

**Solutions:**
- Verify I2C connections (A4=SDA, A5=SCL)
- Check LCD power (5V and GND)
- Adjust LCD contrast potentiometer on back of module
- Verify I2C address (try 0x27 or 0x3F)
- Test with I2C scanner sketch
- Check solder joints on I2C module

**Problem:** LCD backlight on but no text

**Solutions:**
- Adjust contrast potentiometer (turn with small screwdriver)
- Verify `lcd.init()` and `lcd.backlight()` in code
- Check library installation
- Try different LiquidCrystal_I2C library version

### Relay Issues

**Problem:** Relay clicks but lamp doesn't turn on

**Solutions:**
- Verify lamp power source is connected
- Check relay COM and NO connections
- Test lamp with direct power connection
- Ensure relay is rated for lamp current
- Measure voltage across relay contacts when active
- Replace relay if contacts burned

**Problem:** Relay doesn't activate (no click)

**Solutions:**
- Check D8 connection to relay IN
- Verify relay power (5V, GND)
- Test relay LED (should light when active)
- Measure D8 voltage (should be LOW/0V when timer running)
- Test relay with separate Arduino sketch
- Replace relay module

**Problem:** Lamp stays on continuously

**Solutions:**
- Check for stuck relay contacts
- Verify relay is active-low type
- Test with multimeter across relay contacts
- Replace faulty relay module
- Check for short circuit in wiring

### Button Problems

**Problem:** Buttons don't respond

**Solutions:**
- Verify button connections to D2-D5
- Check buttons connect pin to GND when pressed
- Test buttons with multimeter
- Ensure INPUT_PULLUP is set in code
- Clean button contacts
- Replace faulty buttons

**Problem:** Multiple triggers on single press

**Solutions:**
- Debounce already implemented (250ms)
- Increase debounce delay in code
- Add 0.1µF capacitor across button terminals
- Use higher quality tactile switches
- Check for loose wiring causing intermittent contact

### Timer Accuracy Issues

**Problem:** Timer doesn't count correctly

**Solutions:**
- Verify time conversion (milliseconds)
- Check for `delay()` calls interfering with millis()
- Test with Serial Monitor output
- Ensure no blocking code in loop()
- Verify Arduino crystal oscillator functioning

**Problem:** Countdown jumps or skips

**Solutions:**
- Reduce Serial.print() frequency
- Optimize LCD update frequency
- Check for memory issues
- Verify stable power supply

### Buzzer Issues

**Problem:** No sound on completion

**Solutions:**
- Check buzzer connections (D9, GND)
- Verify buzzer polarity (+ to D9)
- Test with simple tone sketch
- Measure D9 voltage during beep
- Replace buzzer if defective
- Check if passive buzzer (needs tone frequency)

**Problem:** Buzzer too quiet or loud

**Solutions:**
- Adjust beep duration in code (currently 200ms)
- Use different buzzer type (active vs passive)
- Add resistor to reduce volume
- Use external amplifier for louder sound

### Serial Monitor Issues

**Problem:** No output in Serial Monitor

**Solutions:**
- Set baud rate to 9600
- Select correct COM port
- Press Arduino reset button
- Try different USB cable
- Install CH340/CP2102 drivers if needed
- Verify `Serial.begin(9600)` in code

## System Behavior

### State Machine

```
┌──────────────┐
│   STARTUP    │
│  Initialize  │
└──────┬───────┘
       │
       ▼
┌──────────────┐
│   IDLE       │ ◄────────────────┐
│ Select Time  │                  │
└──────┬───────┘                  │
       │                          │
       │ Button press             │
       │ (3/5/10 min)             │
       ▼                          │
┌──────────────┐                  │
│ TIME SET     │                  │
│ Display time │                  │
└──────┬───────┘                  │
       │                          │
       │ Start/Stop pressed       │
       ▼                          │
┌──────────────┐                  │
│   RUNNING    │                  │
│ Lamp ON      │                  │
│ Countdown    │                  │
└──────┬───────┘                  │
       │                          │
       │ Timer expires            │
       │ or Stop pressed          │
       ▼                          │
┌──────────────┐                  │
│  COMPLETE    │                  │
│ Lamp OFF     │                  │
│ Buzzer beep  │                  │
└──────┬───────┘                  │
       │                          │
       └──────────────────────────┘
```

### Relay Logic

```
Relay Control (Active-Low):
  
  digitalWrite(relayPin, HIGH)  →  Relay OFF  →  Lamp OFF
  digitalWrite(relayPin, LOW)   →  Relay ON   →  Lamp ON

Timer States:
  NOT RUNNING  →  relayPin = HIGH  →  Lamp OFF
  RUNNING      →  relayPin = LOW   →  Lamp ON
  STOPPED      →  relayPin = HIGH  →  Lamp OFF
```

### Timing Diagram

```
Time:     0s    1s    2s    3s    ...   178s  179s  180s
          │     │     │     │            │     │     │
State:    IDLE  ├─────────────RUNNING────────────────┤ DONE
          │     │                                     │
Relay:    OFF   ON──────────────────────────────────OFF
          │     │                                     │
LCD:      Select│ Left:180s→179s→178s→...→2s→1s→0s  │Done
          │     │                                     │
Buzzer:   ───────────────────────────────────────────███
                                                      Beep
```

### Button Debounce

```
Physical Button Press:
  │
  ├──┐    ┌──────────────┐
  │  │    │  Stable LOW  │
  │  └────┘              └────
  │  ↑
  │  Bounce (ignored)
  │
  └─ Software reads after 250ms delay
```

## Safety Information

### UVB Exposure Guidelines

**Critical Safety Rules:**
- Never exceed recommended exposure times
- Always supervise infant during sessions
- Maintain proper distance from UVB source
- Protect infant's eyes (cover or remove from direct exposure)
- Monitor skin for signs of overexposure
- Follow medical professional's dosage recommendations

**Session Limits:**
- Initial sessions: Start with minimum time (3 minutes)
- Standard sessions: 5 minutes per medical guidance
- Maximum: Never exceed 10 minutes without medical approval
- Frequency: Follow prescribed schedule (typically daily)

### Electrical Safety

- Use properly rated power supply for UVB lamp
- Ensure relay is rated for lamp current
- Keep electronics away from water/moisture
- Use insulated enclosure for all components
- Ground lamp housing if metal
- Never modify wiring while powered on

### System Operation Safety

**Before Each Use:**
- Verify lamp is functioning correctly
- Check relay operation (should click on start)
- Confirm timer countdown is accurate
- Test emergency stop function
- Ensure buzzer is audible

**During Use:**
- Never leave infant unattended
- Watch for signs of discomfort
- Keep manual stop button accessible
- Monitor LCD display for remaining time
- Be ready to manually stop if needed

**After Use:**
- Verify lamp has turned off
- Allow lamp to cool before handling
- Clean lamp and surrounding area
- Log session duration and observations
- Report any equipment malfunctions

### Equipment Maintenance

**Daily:**
- Verify all buttons function correctly
- Check LCD display clarity
- Test buzzer operation
- Inspect lamp for damage

**Weekly:**
- Clean lamp surface (when cool)
- Check all wire connections
- Test emergency stop function
- Verify timer accuracy with stopwatch

**Monthly:**
- Inspect relay contacts for wear
- Check lamp UV output (if meter available)
- Clean electronic enclosure
- Verify all safety features

**Annually:**
- Replace UVB lamp (output degrades over time)
- Test relay under load
- Calibrate timer if needed
- Professional equipment inspection

### Emergency Procedures

**If System Malfunctions:**
1. Press Start/Stop button immediately
2. If lamp doesn't turn off, disconnect power
3. Do not use until repaired
4. Document malfunction details
5. Have qualified technician inspect

**If Infant Shows Distress:**
1. Stop session immediately (press Start/Stop)
2. Remove infant from UVB exposure
3. Check skin for redness or burns
4. Contact medical professional if needed
5. Document incident

**Power Failure:**
- System automatically turns off (fail-safe)
- Relay defaults to OFF state
- No automatic restart after power restored
- Must manually restart session

## Notes

- All timer values stored as unsigned long (milliseconds)
- Relay uses active-low logic for fail-safe operation
- Internal pull-up resistors eliminate need for external resistors
- LCD I2C reduces pin usage compared to parallel interface
- Debounce prevents accidental multiple button triggers
- Serial test mode allows verification without waiting full duration
- System uses non-blocking code (millis() instead of delay())

## Support

For issues not covered in troubleshooting:

1. Check Serial Monitor for debug output
2. Verify all connections match wiring diagram
3. Test individual components separately
4. Ensure proper library versions installed
5. Review medical guidelines for UVB exposure
6. Consult with qualified medical professional

## Medical Disclaimer

This device is provided as-is for controlled UVB exposure. Always consult with a qualified healthcare provider before using UVB therapy for infants. Follow all medical guidelines and recommendations for phototherapy. The manufacturer is not responsible for misuse or improper application of this device.

## License

This project is provided as-is for educational and medical purposes under proper supervision.

---

**Project:** MIMISUN  
**Application:** UVB Phototherapy Timer  
**Platform:** Arduino Uno  
**UVB Source:** UVB 10.0 Lamp  
**Timer Presets:** 3, 5, 10 minutes  
**Version:** 1.0  
**Last Updated:** January 2026
