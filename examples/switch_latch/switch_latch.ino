#include <Flicker.h>

// The input goes on when touched
// and off when touched again.

byte touchPin = 0; // TOUCH PIN e.g. 0 on Teensy, D0 on XIAO

// MOMENTARY (the default) or LATCH can be specified
TouchSwitch myInput(touchPin, LATCH);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  // WARNING! if you setThreshold with no argument,
  // the threshold is calculated based on a call to
  // touchRead() so DON'T touch the input during setup()
  myInput.setThreshold(); 
}

void loop(){
  myInput.update();
  digitalWrite(LED_BUILTIN, myInput.read());
}
