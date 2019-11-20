#include <Flicker.h>

// The input latches ON/OFF if touched more than 1/2 second.
// While it's ON, the LED will toggle ON and OFF.
// Also, a variable reading is taken from the same input,
// mapped to a range of 0~1023 and printed to serial.

const uint8_t touchPin = 15; // CHOOSE A TOUCH PIN
const uint8_t ledPin = 13;
bool ledState = false;
int preVal = 0;
TouchSwitch mySwitch(touchPin, LATCH); 
TouchVariable myVariable(touchPin, 0, 1023); 

void setup(){
  Serial.begin(9600);
  pinMode(ledPin,OUTPUT);

  // WARNING! if you setThreshold or setInputRange
  // with no arguments, the threshold or input range
  // is calculated based on a call to touchRead()
  // so DON'T touch the input during setup()
  mySwitch.setThreshold();
  mySwitch.interval(500);
  myVariable.setInputRange();
}

void loop(){
  if (mySwitch.update()){
    bool value = mySwitch.read();
    if (value == HIGH){
      mySwitch.retrigger(500);
      ledState = !ledState;
    }
    else {ledState = LOW;
    }
  digitalWrite(ledPin, ledState);
  }

  int newVal = myVariable.read();
  if (newVal != preVal){
    Serial.println(newVal);
    preVal = newVal;
  }

}
