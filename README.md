# Flicker 2.1.0
### A library for making capacitive touch easy to use across multiple microcontroller boards.
###### by Josh Nishikawa <github.com/joshnishikawa/Flicker>
###### The "TouchSwitch" class was adapted from the Bounce library by: Thomas O Fredericks, Eric Lowry, Jim Schimpf and Tom Harkaway
Includes all the functions found in the Bounce library, smooths erratic values to make variable input more useful, and includes functions for measuring velocity.

---

### SUPPORTED BOARDS & ARCHITECTURES:
- **Teensy (3.0, 3.1, 3.2, 3.5, 3.6, LC)**: Uses native hardware Touch Sense Input (TSI) pins.
- **ESP32 Classic (WROOM, WROVER)**: Uses built-in touch pins (T0–T9 on GPIOs 4, 0, 2, 15, 13, 12, 14, 27, 33, 32) with automatic signal normalization.
- **ESP32-S2 & ESP32-S3**: Uses built-in hardware capacitive touch pins (GPIOs 1–14).
- **AVR (Arduino Uno, Nano, Mega, Leonardo, Pro Mini)**: Single-pin capacitive sensing on any analog pin (A0–A7) using internal ADC sample & hold capacitor charge redistribution (no extra resistors or capacitors needed).
- **SAMD21 / SAMD51**: Supported on touch-enabled pins / core environments.

---

### VERSION LOG:
2.1.0
- Added multi-board hardware abstraction layer (`FlickerTouch.h`).
- Added support for ESP32 and ESP32-S2/S3 with signal inversion/normalization for classic ESP32.
- Added single-pin ADC capacitive touch sensing for classic AVR boards (Uno, Nano, Mega, Leonardo).
- Updated `library.properties` architectures to `*`.
- Prevented potential 32-bit integer overflow in `TouchVariable` filter for high-resolution touch counters.
- Self-contained `rangeFinder.ino` without requiring external dependencies.

2.0.0
- Thresholds for TouchSwitch are now set more dynamically and updated when
  highest/lowest readings are updated. This better accommodates more setups 
  regardless of quiescent readings or ranges between highest/lowest readings.

- setThresholds() MUST be called during setup and requires two arguments: the lowest possible reading that detects your finger and the reading at which a the value should be returned (contact). Use rangeFinder.ino to find these values.
- responsiveRead() is deprecated. Use read() instead.
- Setting outLo and outHi in the constructor is deprecated. Just map the return
  value of read() in the sketch.


1.1.4 
- Added previousDuration() function that can be used on rising/falling edge to return the length of the input's previous state.
- Added rose() and fell(). risingEdge() and fallingEdge() still work.
- Now using bool data type where appropriate.

1.0.4 
   - created a library.properties file
   - made the version number semver compliant (1.0.4 is the first official release)
   - moved source files into src/

1.0.3
  - Fixed a bug that pushed the quiescent touchRead value above the
    offThreshold when using the ADC.
    
1.0.2
  - Fine tuned ON and OFF thresholds for TouchSwitch and TouchVariable

1.0.0
  - Changed "Flicker" to "TouchSwitch" ("Flicker" deprecated but still works)
  - Added an option to LATCH the state of a TouchSwitch.
  - Added interval() (still useful even if noise reduction isn't time-based)
  - Added the TouchVariable class for stable reading of ranges of input.
  - Added the TouchVelocity class for detecting velocity of changes in input.
  - Started keeping this version log.
___
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 MA 02110-1301, USA.
