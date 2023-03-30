/*
  This is not an example for how to use this library.
  Rather, it's a utility to find a usable range and/or
  thresholds for your particular needs. Get the values
  printed in the serial monitor to use in other sketches.  
*/

#include "Bounce2.h"

int touchPin = 0; // Change to the TOUCH pin you want to use.
int resetPin = 19; // you can set a pin to reset hi/lo
Bounce reset = Bounce(resetPin, 50);
int hi;

void setReset(){
  hi = touchRead(touchPin);
  Serial.print("High Touch: "); Serial.println(hi);
}

void setup(){
  pinMode(resetPin, INPUT_PULLUP);
  setReset();
}

void loop(){
  reset.update();
  if(reset.rose()){
    setReset();
  }

  int newVal = touchRead(touchPin);
  if (newVal > hi){
    hi = newVal;
    Serial.print("High Touch: "); Serial.println(hi);
  }
}
