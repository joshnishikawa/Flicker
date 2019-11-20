#include <Flicker.h>

// As long as the input is held, 
// it will be triggered repeatedly.

const uint8_t touchPin = 15; // CHOOSE A TOUCH PIN
const uint8_t ledPin = 13;
bool ledState = false;
TouchSwitch myInput(touchPin); 

void setup(){
  pinMode(ledPin,OUTPUT);

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
  digitalWrite(ledPin, ledState);
  }
}
