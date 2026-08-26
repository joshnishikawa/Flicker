#include "TouchVariable.h"

TouchVariable::TouchVariable(){
  this->mapped = false;
  this->buffer = 0;
};

TouchVariable::TouchVariable(byte pin){
  this->pin = pin;
  this->mapped = false;
  this->buffer = 0;
};

TouchVariable::TouchVariable(byte pin, int outLo, int outHi){
  this->pin = pin;
  this->outLo = outLo;
  this->outHi = outHi;
  this->mapped = true;
  this->buffer = 0;
};

TouchVariable::~TouchVariable(){};


void TouchVariable::setInputRange(){
  adjustInHi = true; // Auto adjust inHi if there is a higher reading.
  flickerTouchInit(); // Fire up ADC if needed on platforms where ADC affects touch
  
  // Discard initial dummy reading (hardware warmup / clear initial register state)
  flickerTouchRead(pin);
  delay(50);

  // Average 16 readings to establish a reliable baseline
  long total = 0;
  for (uint8_t i = 0; i < 16; i++) {
    total += flickerTouchRead(pin);
    delay(2);
  }
  int qval = (int)(total / 16);

  inLo = (int)(qval * 1.01); // prevent noise on the bottom end
  inHi = max(inLo + 10, (int)(qval * 1.10)); // Higher values are still possible
  balancedValue = inLo;
  buffer = (long)inLo << 8;
};

void TouchVariable::setInputRange(int inLo, int inHi){
  adjustInHi = false; // Don't increase inHi when getting a higher reading
  this->inLo = inLo;
  this->inHi = (inHi > inLo) ? inHi : (inLo + 1); // Guard against division by zero in map()
  balancedValue = inLo;
  buffer = (long)inLo << 8;
};


int TouchVariable::read(){
  int rawValue = flickerTouchRead(pin);

  if (balancedValue == 0 && buffer == 0){
    balancedValue = rawValue;
    buffer = (long)rawValue << 8;
  }

  if (NR <= 0.0f){
    balancedValue = rawValue;
    buffer = (long)rawValue << 8;
  } else {
    // Dynamic noise deadband scaled with NR to eliminate stationary jitter
    int deadband = max(1, (int)(rawValue * (NR / 10000.0f)));
    int diff = rawValue - balancedValue;

    if (abs(diff) > deadband){
      long target = (long)rawValue << 8;
      long delta = target - buffer;

      // Smooth fractional progression divisor
      int k = max(2, (int)(NR * 0.32f));
      if (abs(diff) > deadband * 6){
        k = max(2, k / 4); // Accelerate tracking for intentional large gestures
      }

      buffer += delta / k;
      balancedValue = (int)((buffer + 128) >> 8);
    }
  }

  if (adjustInHi && balancedValue > inHi){
    inHi = balancedValue;
  }

  if (mapped){
    int loBound = min(outLo, outHi);
    int hiBound = max(outLo, outHi);
    int mappedVal = map(balancedValue, inLo, inHi, outLo, outHi);
    return constrain(mappedVal, loBound, hiBound);
  }

  return balancedValue;
};


void TouchVariable::setOutputRange(int outLo, int outHi){
  this->outLo = outLo;
  this->outHi = outHi;
  mapped = true;
};

void TouchVariable::setNR(int amount){
  this->NR = float(amount);
};


