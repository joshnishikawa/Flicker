#include <Flicker.h>

/*
  Flicker - velocity_MIDI
  Calculates touch strike velocity and outputs MIDI Note On / Note Off.
  
  - Teensy: Uses hardware usbMIDI (Tools > USB Type > "MIDI" or "Serial + MIDI").
  - ESP32-S2 / ESP32-S3: Uses built-in TinyUSB USBMIDI.
  - All boards: Logs MIDI events to Serial Monitor for testing.
*/

int slowest = 22000; // Strike time (~22ms) for minimum velocity (1)
int fastest = 2500;  // Strike time (~2.5ms) for maximum velocity (127)
float velocityCurve = 2.2; // 1.0 = linear, 2.0~2.5 = natural musical exponential curve
int noteNumber = 38; // MIDI Note (38 = Snare / D1)

#if defined(ARDUINO_ARCH_ESP32)
  #if defined(D0)
    const uint8_t touchPin = D0; // XIAO ESP32-S3 (D0 is GPIO 1 / TOUCH1)
  #elif defined(CONFIG_IDF_TARGET_ESP32S3) || defined(CONFIG_IDF_TARGET_ESP32S2)
    const uint8_t touchPin = 1;  // ESP32-S2 / ESP32-S3 touch pin (GPIO 1-14)
  #else
    const uint8_t touchPin = 4;  // Classic ESP32 touch pin T0 (GPIO 4)
  #endif
#elif defined(D0)
  const uint8_t touchPin = D0;   // XIAO SAMD21 / RP2040
#elif defined(__AVR__)
  const uint8_t touchPin = A0;   // AVR (Uno, Nano, Mega, Leonardo) analog pin
#else
  const uint8_t touchPin = 0;    // Teensy 3.x/LC (Pin 0 is TSI touch)
#endif

// =============================================================================
// MIDI Support
// =============================================================================
#if defined(CORE_TEENSY) || defined(TEENSYDUINO)
  void midiSetup() {}
  void midiNoteOn(uint8_t note, uint8_t vel) {
    #if defined(usbMIDI)
    usbMIDI.sendNoteOn(note, vel, 1);
    #endif
  }
  void midiNoteOff(uint8_t note) {
    #if defined(usbMIDI)
    usbMIDI.sendNoteOff(note, 0, 1);
    #endif
  }
#elif (defined(CONFIG_IDF_TARGET_ESP32S3) || defined(CONFIG_IDF_TARGET_ESP32S2)) && __has_include("USBMIDI.h") && __has_include("USB.h")
  #include "USB.h"
  #include "USBMIDI.h"
  USBMIDI usbMidi;
  void midiSetup() { usbMidi.begin(); USB.begin(); }
  void midiNoteOn(uint8_t note, uint8_t vel) { usbMidi.noteOn(note, vel, 1); }
  void midiNoteOff(uint8_t note) { usbMidi.noteOff(note, 0, 1); }
#else
  void midiSetup() {}
  void midiNoteOn(uint8_t note, uint8_t vel) {}
  void midiNoteOff(uint8_t note) {}
#endif

TouchVelocity myInput(touchPin);

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 2500); // Wait for Serial on USB boards

  midiSetup();
  myInput.setThresholds();

  Serial.println(F("--- Flicker velocity_MIDI ---"));
  Serial.print(F("Touch Pin: "));
  Serial.println(touchPin);
}

void loop(){
  int velocity = myInput.read();

  if (velocity >= 1){
    // Calculate normalized strike speed (0.0 = slowest, 1.0 = fastest)
    float norm = constrain(float(slowest - velocity) / float(slowest - fastest), 0.0f, 1.0f);

    // Apply response curve for musical velocity dynamics
    int midiVel = constrain(int(1 + 126.0f * pow(norm, velocityCurve)), 1, 127);

    midiNoteOn(noteNumber, midiVel);

    Serial.print(F("MIDI Note ON  | Note: "));
    Serial.print(noteNumber);
    Serial.print(F(" | Vel: "));
    Serial.print(midiVel);
    Serial.print(F(" | Time: "));
    Serial.print(velocity);
    Serial.println(F(" us"));
  }

  if (myInput.fell()){
    midiNoteOff(noteNumber);
    Serial.print(F("MIDI Note OFF | Note: "));
    Serial.println(noteNumber);
  }
}
