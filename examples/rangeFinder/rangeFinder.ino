/*
  This is a utility to find a usable range and/or
  thresholds for your particular touch setup. Get the values
  printed in the serial monitor to use in other sketches.  
*/

#include <Flicker.h>

int touchPin = 0; // TOUCH PIN e.g. 0 on Teensy, D0 on XIAO (or A0-A7 on AVR)
int resetPin = 19; // Optional button pin to reset hi/lo baseline
TouchSwitch resetBtn(resetPin);
int hi;

void setReset(){
  hi = flickerTouchRead(touchPin);
  Serial.print("Baseline Touch: "); Serial.println(hi);
}

void setup(){
  Serial.begin(9600);
  pinMode(resetPin, INPUT_PULLUP);
  setReset();
}

void loop(){
  resetBtn.update();
  if(resetBtn.rose()){
    setReset();
  }

  int newVal = flickerTouchRead(touchPin);
  if (newVal > hi){
    hi = newVal;
    Serial.print("High Touch: "); Serial.println(hi);
  }
}
