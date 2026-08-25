#include <Flicker.h>

// If you hold your finger on the input,
// something will happen.

const uint8_t touchPin = 0; // TOUCH PIN e.g. 0 on Teensy, D0 on XIAO
TouchSwitch myInput(touchPin);

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);

  // WARNING! if you setThreshold with no argument,
  // the threshold is calculated based on a call to
  // touchRead() so DON'T touch the input during setup()
  myInput.setThreshold();

  // Use rangeFinder.ino to find a specific threshold
  // Then replace '1200' and use the following line instead.
  // myInput.setThreshold(1200);
}

void loop() {
  myInput.update();

  if (myInput.rose()){
    Serial.println("rose");
  }
  if (myInput.fell()){
    Serial.println("fell");
  }

  digitalWrite(LED_BUILTIN, myInput.read());
}
