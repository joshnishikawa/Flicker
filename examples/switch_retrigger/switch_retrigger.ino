#include <Flicker.h>

// As long as the input is held, 
// it will be triggered repeatedly.

const uint8_t touchPin = 0; // TOUCH PIN e.g. 0 on Teensy, D0 on XIAO
bool ledState = false;
TouchSwitch myInput(touchPin); 

void setup(){
  pinMode(LED_BUILTIN, OUTPUT);

  // WARNING! if you setThreshold with no argument,
  // the threshold is calculated based on a call to
  // touchRead() so DON'T touch the input during setup()
  myInput.setThreshold(); 
}

void loop(){
  if (myInput.update()){
    if (myInput.read()){
      myInput.retrigger(500);
      ledState = !ledState;
    }
    else {ledState = LOW;
    }
  digitalWrite(LED_BUILTIN, ledState);
  }
}
