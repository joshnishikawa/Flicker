#include <Flicker.h>

/* WARNING! This example is useless if you haven't used the velocity_basic
   example to determine the fastest and slowest readings that you want to map
   to MIDI velocities. Once you have those numbers, enter them here.*/
int slowest = 2200;
int fastest = 550;

byte touchPin = 0; // CHOOSE A TOUCH PIN
int noteNumber = 38; // CHOOSE A NOTE TO PLAY

TouchVelocity myInput(touchPin);
 
void setup() {
  // WARNING! You must pass two arguments to setThresholds()
  // Use the rangeFinder example to get a reading with your finger 1mm above the
  // input and another reading with your finger lightly touching the input.
  // You will not likely be able to use these values (900, 1250) as is.
  myInput.setThresholds(900, 1250);
}

void loop(){
  int velocity = myInput.read();
  if (velocity >= 1){
    velocity = map(velocity, slowest, fastest, 1, 127);
    velocity = constrain(velocity, 1, 127);
    usbMIDI.sendNoteOn(noteNumber, velocity, 0);
  }
  if (myInput.fell()){
    usbMIDI.sendNoteOff(noteNumber, 0, 0);
  }
}
