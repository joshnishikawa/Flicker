#include <Flicker.h>

// This reads a touch input and prints a new
// value only when the signal is biased enough.
// The larger the change, the more immediate the response.

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

int preVal;
TouchVariable myInput(touchPin);

void setup() {
  Serial.begin(9600);

  // WARNING! if you setInputRange with no argument,
  // the input range is calculated based on a call to
  // touchRead() so DON'T touch the input during setup()
  myInput.setInputRange();
}

void loop(){
  int newVal = myInput.read();
  if (newVal != preVal){
    Serial.print(myInput.inLo); Serial.print(" ");
    Serial.print(myInput.inHi); Serial.print(" ");
    Serial.println(newVal);
    preVal = newVal;
  }
}
