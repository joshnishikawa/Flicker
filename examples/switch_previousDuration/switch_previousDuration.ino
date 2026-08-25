#include <Flicker.h>

// This will tell you how long the input stayed in its PREVIOUS state.
// It may be useful to call this within rose() or fell().

const uint8_t touchPin = 0; // TOUCH PIN e.g. 0 on Teensy, D0 on XIAO
TouchSwitch myInput(touchPin);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

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

  digitalWrite(LED_BUILTIN, myInput.read());
}
