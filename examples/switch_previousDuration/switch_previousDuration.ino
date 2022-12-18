#include <Flicker.h>

// This will tell you how long the input stayed in its PREVIOUS state.
// It may be useful to call this within rose() or fell().

const uint8_t touchPin = 15; // CHOOSE A TOUCH PIN
const uint8_t ledPin = 13;
TouchSwitch myInput(touchPin);

void setup() {
  pinMode(ledPin, OUTPUT);

  // WARNING! if you setThreshold with no argument,
  // the threshold is calculated based on a call to
  // touchRead() so DON'T touch the input during setup()
  myInput.setThreshold(); 
}

void loop() {
  myInput.update();

  if (myInput.fell()){
    // After the input was released, print how long it was held.
    Serial.println( myInput.previousDuration() );
  }

  digitalWrite(ledPin, myInput.read());
}
