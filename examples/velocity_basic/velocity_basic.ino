#include <Flicker.h>

/* This determines the interval between the time
   your finger is detected hovering over the input
   and the time it makes contact with the input.
   The LED lights when your finger goes below the
   'hover' threshold to act as a 'ready' light.*/

byte touchPin = 15; // CHOOSE A TOUCH PIN
byte LED = 13;
TouchVelocity myInput(touchPin);
 
void setup() {
  pinMode(LED, OUTPUT);
  
  // WARNING! setThreshold() uses a call to touchRead()
  // so DON'T touch the input during setup()
  myInput.setThreshold();
  digitalWrite(LED, HIGH); 
}

void loop(){
  int velocity = myInput.read();
  if (myInput.fallingEdge()){
    digitalWrite(LED, HIGH);
  }
  if (myInput.risingEdge()){
    digitalWrite(LED, LOW);
  }
  if (velocity >= 1){
    Serial.println(velocity);
  }
}
