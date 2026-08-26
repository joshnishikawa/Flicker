#include <Flicker.h>

// The input latches ON/OFF if touched more than 1/2 second.
// While it's ON, the LED will toggle ON and OFF.
// Also, a variable reading is taken from the same input,
// mapped to a range of 0~1023 and printed to serial.

#if defined(ARDUINO_ARCH_ESP32)
  #if defined(D0)
    const uint8_t touchPin = D0; // XIAO ESP32-S3 (D0 is GPIO 1 / TOUCH1)
  #elif defined(CONFIG_IDF_TARGET_ESP32S3) || defined(CONFIG_IDF_TARGET_ESP32S2)
    const uint8_t touchPin = 1;  // ESP32-S2 / ESP32-S3 touch pin (GPIO 1-14)
  #else
    const uint8_t touchPin = 4;  // Classic ESP32 touch pin T0 (GPIO 4)
  #endif
#elif defined(D0)
  const uint8_t touchPin = D0;   // XIAO SAMD21 / RP2040
#elif defined(__AVR__)
  const uint8_t touchPin = A0;   // AVR (Uno, Nano, Mega, Leonardo) analog pin
#else
  const uint8_t touchPin = 0;    // Teensy 3.x/LC (Pin 0 is TSI touch)
#endif
bool ledState = false;
int preVal = 0;
TouchSwitch mySwitch(touchPin, LATCH); 
TouchVariable myVariable(touchPin, 0, 1023); 

void setup(){
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);

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
  digitalWrite(LED_BUILTIN, ledState);
  }

  int newVal = myVariable.read();
  if (newVal != preVal){
    Serial.println(newVal);
    preVal = newVal;
  }

}
