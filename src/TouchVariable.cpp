#include "TouchVariable.h"

TouchVariable::TouchVariable(){};

TouchVariable::TouchVariable(byte pin){
  this->pin = pin;
  setInputRange(); // only works if creating objects during setup
};

TouchVariable::TouchVariable(byte pin, int outLo, int outHi){
  this->pin = pin;
  this->outLo = outLo;
  this->outHi = outHi;
  mapped = true;
  setInputRange(); // only works if creating objects during setup
};

TouchVariable::~TouchVariable(){};


void TouchVariable::setInputRange(){
  adjustInHi = true; // Auto adjust inHi if there is a higher reading.
  int qval = 0;
  analogRead(A0); // The ADC can affect touch values so fire it up first.

  for (int i = 0; i < 10; i++){ // store the highest quiescent reading of 10
    int newValue = touchRead(pin);
    if (newValue > qval) qval = newValue;
  }

  inLo = qval * 1.02; // Because touchRead() is inaccurate here. ??????
  inHi = qval * 1.7; // Higher values are still possible
  offThreshold = qval * 1.2; // Just in case the input is multi-tasking
  onThreshold = qval * 1.3;  // Just in case the input is multi-tasking
};

void TouchVariable::setInputRange(int inLo, int inHi){
  adjustInHi = false; // Don't increase inHi when getting a higher reading
  this->inHi = inHi; // Values can still go higher unless using setOutputRange()
  onThreshold = inLo * 1.3; // Just in case the input is multi-tasking
  offThreshold = inLo * 1.2; // Just in case the input is multi-tasking
  this->inLo = inLo; // Values can still go lower unless using setOutputRange()
};


int TouchVariable::read(){
  int newValue = touchRead(pin);

  // Determine what percent of touchRead() values the threshold should be.
  threshold = newValue * (NR/100);

  if (adjustInHi){
    // A conservative inHi of 1.7x is set when setInputRange() is called but,
    // the highest reading could be much higher than that. This line adjusts
    // inHi if newValue is 1.1x higher. If newValue is higher but less than
    // 1.1* higher, inHi is left alone allowing you to 'max out' the input.
    // Use setInputRange(int inLo, int inHi) to prevent this auto-adjustment.
    inHi = newValue > inHi * 1.1 ? newValue : inHi;
  }
  if (mapped){
    newValue = map(newValue, inLo, inHi, outLo, outHi);
    newValue = constrain(newValue, outLo, outHi);
  }
  int difference = newValue - balancedValue;
  buffer = newValue == balancedValue ? buffer/2 : buffer + difference;
  if (buffer*buffer > threshold*threshold){ // "abs(buffer) > NR" doesn't work
    balancedValue = newValue;

    buffer = 0;
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
