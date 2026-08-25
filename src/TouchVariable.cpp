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
  this->balancedValue = outLo;
};

TouchVariable::~TouchVariable(){};


void TouchVariable::setInputRange(){
  adjustInHi = true; // Auto adjust inHi if there is a higher reading.
  flickerTouchInit(); // Fire up ADC if needed on platforms where ADC affects touch
  
  // Warm up the touch sensor hardware and allow filters/clocks to settle
  flickerTouchRead(pin);
  delay(50);

  // Average multiple readings to establish a stable baseline
  long total = 0;
  for (uint8_t i = 0; i < 16; i++) {
    total += flickerTouchRead(pin);
    delay(2);
  }
  int qval = (int)(total / 16);
  
  inLo = qval;
  inHi = inLo + max(100, (int)(qval * 0.10));
  balancedValue = mapped ? outLo : inLo;
  buffer = 0;
};

void TouchVariable::setInputRange(int inLo, int inHi){
  adjustInHi = false; // Don't increase inHi when getting a higher reading
  this->inHi = inHi; // Values can still go higher unless using setOutputRange()
  this->inLo = inLo; // Values can still go lower unless using setOutputRange()
  balancedValue = mapped ? outLo : inLo;
  buffer = 0;
};


int TouchVariable::read(){
  // Auto-calibrate if setInputRange was not explicitly called in setup
  if (inLo == 0 && inHi == 0){
    setInputRange();
  }

  int rawValue = flickerTouchRead(pin);

  if (adjustInHi && rawValue > inHi){
    inHi = rawValue;
  }

  // Ensure valid range to avoid divide-by-zero
  if (inHi <= inLo){
    inHi = inLo + 1;
  }

  int targetValue;
  if (mapped){
    int loBound = min(outLo, outHi);
    int hiBound = max(outLo, outHi);
    targetValue = map(rawValue, inLo, inHi, outLo, outHi);
    targetValue = constrain(targetValue, loBound, hiBound);
  } else {
    targetValue = rawValue;
  }

  // If noise reduction is disabled (0), return immediate value
  if (NR <= 0.0f){
    balancedValue = targetValue;
    return balancedValue;
  }

  int difference = targetValue - balancedValue;
  if (difference == 0){
    return balancedValue;
  }

  int span = mapped ? abs(outHi - outLo) : (inHi - inLo);
  // Deadband threshold: small fluctuations within deadband (0.5% to 1.5% of span) are ignored
  threshold = max(2, (int)(span * (NR / 7500.0f)));

  // If change is within noise deadband, keep current balancedValue solid
  if (abs(difference) <= threshold){
    return balancedValue;
  }

  // Smoothly track towards targetValue
  float alpha = 1.0f - (constrain(NR, 0.0f, 100.0f) / 100.0f) * 0.70f;
  int step = (int)(difference * alpha);
  if (step == 0){
    step = (difference > 0) ? 1 : -1;
  }
  balancedValue += step;

  return balancedValue;
};


void TouchVariable::setOutputRange(int outLo, int outHi){
  this->outLo = outLo;
  this->outHi = outHi;
  this->mapped = true;
  this->balancedValue = outLo;
  this->buffer = 0;
};

void TouchVariable::setNR(int amount){
  this->NR = (float)constrain(amount, 0, 100);
};

