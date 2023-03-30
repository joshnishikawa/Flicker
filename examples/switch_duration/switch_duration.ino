#include <Flicker.h>

// If you hold your finger on the input
// for long enough, something will happen.

const uint8_t touchPin = 0; // CHOOSE A TOUCH PIN
const uint8_t ledPin = 13;
TouchSwitch myInput(touchPin);

void setup() {
  pinMode(ledPin, OUTPUT);

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
  digitalWrite(ledPin, myInput.read());
}
