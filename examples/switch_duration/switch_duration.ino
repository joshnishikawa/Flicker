#include <Flicker.h>

// If you hold your finger on the input
// for long enough, something will happen.

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
TouchSwitch myInput(touchPin);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  // WARNING! if you setThreshold with no argument,
  // the threshold is calculated based on a call to
  // touchRead() so DON'T touch the input during setup()
  myInput.setThreshold(); 

  // Use rangeFinder.ino to find a specific threshold
  // Then replace '1200' and use the following line instead.
  // myInput.setThreshold(1200);

  myInput.interval(300); // How long to hold before response?
}

void loop() {
  myInput.update();
  digitalWrite(LED_BUILTIN, myInput.read());
}
