#include <Flicker.h>

// Map the variable touuch input to a specific range.
// Examples: 0~1023 to emulate analog; or 0~127 for MIDI.

byte touchPin = 15; // CHOOSE A TOUCH PIN
int preVal;
TouchVariable myInput(touchPin);

void setup() {
  Serial.begin(9600);
 
  // WARNING! if you setInputRange with no argument,
  // the input range is calculated based on a call to
  // touchRead() so DON'T touch the input during setup()
  myInput.setInputRange();
  myInput.setOutputRange(0, 1023); // 7-bit MIDI
}

void loop(){
  int newVal = myInput.read();
  if (newVal != preVal){
    Serial.print(0); Serial.print(" ");
    Serial.print(1023); Serial.print(" ");
    Serial.println(newVal);
    preVal = newVal;
  }
}
