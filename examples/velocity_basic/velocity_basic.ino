#include <Flicker.h>

/* This determines the interval between the time
   your finger is detected hovering over the input
   and the time it makes contact with the input.
   The LED lights when your finger goes below the
   'hover' threshold to act as a 'ready' light.*/

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

TouchVelocity myInput(touchPin);
 
void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 2500); // Wait for Serial on USB CDC boards
  pinMode(LED_BUILTIN, OUTPUT);
  
  // Calling setThresholds() with no arguments automatically calculates
  // the hover and touch thresholds based on your quiescent touch reading
  // (DON'T touch the sensor during setup):
  myInput.setThresholds();

  // Or use the rangeFinder example to find specific numbers for your setup
  // and pass them as arguments:
  // myInput.setThresholds(hoverThreshold, touchThreshold);

  digitalWrite(LED_BUILTIN, HIGH); 
}

void loop(){
  int velocity = myInput.read();
  if (myInput.fell()){
    digitalWrite(LED_BUILTIN, HIGH);
  }
  if (myInput.rose()){
    digitalWrite(LED_BUILTIN, LOW);
  }
  if (velocity >= 1){
    Serial.println(velocity);
  }
}
