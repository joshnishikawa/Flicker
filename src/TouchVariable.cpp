#include "TouchVariable.h"

TouchVariable::TouchVariable(){};

TouchVariable::TouchVariable(byte pin){
  this->pin = pin;
  this->mapped = false;
};

TouchVariable::TouchVariable(byte pin, int outLo, int outHi){
  this->pin = pin;
  this->outLo = outLo;
  this->outHi = outHi;
  this->mapped = true;
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

  inLo = qval * 1.01; // prevent noise on the bottom end
  inHi = qval * 1.1; // Higher values are still possible
  balancedValue = inLo;
  buffer = 0;
};

void TouchVariable::setInputRange(int inLo, int inHi){
  adjustInHi = false; // Don't increase inHi when getting a higher reading
  this->inHi = inHi; // Values can still go higher unless using setOutputRange()
  this->inLo = inLo; // Values can still go lower unless using setOutputRange()
  balancedValue = inLo;
  buffer = 0;
};


int TouchVariable::read(){
  int rawValue = flickerTouchRead(pin);

  // Determine what percent of touch reading values the threshold should be.
  threshold = (int)(rawValue * (NR / 100.0f));

  if (adjustInHi){
    // A conservative inHi is set when setInputRange() is called but,
    // the highest reading could be much higher than that. This line adjusts
    // inHi if rawValue is 1.1x higher. If rawValue is higher but less than
    // 1.1* higher, inHi is left alone allowing you to 'max out' the input.
    // Use setInputRange(int inLo, int inHi) to prevent this auto-adjustment.
    inHi = rawValue > inHi * 1.1 ? rawValue : inHi;
  }

  int difference = rawValue - balancedValue;
  int deadband = max(2, (int)(threshold * 0.05f));

  if (abs(difference) <= deadband){
    buffer = buffer / 2;
  } else {
    buffer = buffer + difference;
  }

  if (abs(buffer) > abs(threshold)){
    balancedValue = rawValue;
    buffer = 0;
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

