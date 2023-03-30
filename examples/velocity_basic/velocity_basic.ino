#include <Flicker.h>

/* This determines the interval between the time
   your finger is detected hovering over the input
   and the time it makes contact with the input.
   The LED lights when your finger goes below the
   'hover' threshold to act as a 'ready' light.*/

byte touchPin = 0; // CHOOSE A TOUCH PIN
byte LED = 13;
TouchVelocity myInput(touchPin);
 
void setup() {
  pinMode(LED, OUTPUT);
  
  // WARNING! You must pass two arguments to setThresholds()
  // Use the rangeFinder example to get a reading with your finger 1mm above the
  // input and another reading with your finger lightly touching the input.
  // You will not likely be able to use these values (900, 1250) as is.
  myInput.setThresholds(900, 1250);

  digitalWrite(LED, HIGH); 
}

void loop(){
  int velocity = myInput.read();
  if (myInput.fell()){
    digitalWrite(LED, HIGH);
  }
  if (myInput.rose()){
    digitalWrite(LED, LOW);
  }
  if (velocity >= 1){
    Serial.println(velocity);
  }
}
