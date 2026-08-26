#include "TouchSwitch.h"

TouchSwitch::TouchSwitch(){};

TouchSwitch::TouchSwitch(byte pin){
  this->pin = pin;
  latched = false;
  setThreshold(); // only works if creating objects during setup
};

TouchSwitch::TouchSwitch(byte pin, byte mode){
  this->pin = pin;
  this->latched = mode;
  setThreshold(); // only works if creating objects during setup
};

TouchSwitch::TouchSwitch(byte pin, byte mode, int onT){
  this->pin = pin;
  latched = mode;
  onThreshold = onT;
  setThreshold(onThreshold); // only works if creating objects during setup
};

TouchSwitch::~TouchSwitch(){};


void TouchSwitch::setThreshold(){
  flickerTouchInit(); // Fire up ADC if needed on platforms where ADC affects touch
  
  // Warm up touch hardware and allow initial clock/charge to settle
  flickerTouchRead(pin);
  delay(50);

  long total = 0;
  for (uint8_t i = 0; i < 16; i++) {
    total += flickerTouchRead(pin);
    delay(2);
  }
  inLo = (int)(total / 16);
  userSetThreshold = false;

  int minDelta = max(20, (int)(inLo * 0.15));
  inHi = inLo + minDelta;
  onThreshold = inLo + minDelta;
  offThreshold = inLo + (int)(minDelta * 0.70);
}


void TouchSwitch::setThreshold(int threshold){
  userSetThreshold = true;
  onThreshold = threshold;
  flickerTouchInit(); // Fire up ADC if needed on platforms where ADC affects touch
  
  flickerTouchRead(pin);
  delay(50);

  long total = 0;
  for (uint8_t i = 0; i < 16; i++) {
    total += flickerTouchRead(pin);
    delay(2);
  }
  inLo = (int)(total / 16);
  inHi = max(inLo, onThreshold);

  int delta = max(10, onThreshold - inLo);
  offThreshold = inLo + (int)(delta * 0.75);
}


void TouchSwitch::updateThreshold(int low, int high){ // internal use only
  inLo = low;
  inHi = high;
  int range = inHi - inLo;

  if (!userSetThreshold){
    int minDelta = max(20, (int)(inLo * 0.15));
    int dynamicDelta = max(minDelta, (int)(range * 0.35));
    onThreshold = inLo + dynamicDelta;
    offThreshold = inLo + (int)(dynamicDelta * 0.70);
  }
  else{
    int delta = max(10, onThreshold - inLo);
    offThreshold = inLo + (int)(delta * 0.75);
  }
}


int TouchSwitch::update(){
	if (trigger()){
    retrigger(0);
    return stateChanged = 1;
  }
  // We need to retrigger, so simulate a state change
	if (retrigger_millis && (millis() - previous_millis >= retrigger_millis)){
    previous_millis = millis();
		retrigger(0);
		return stateChanged = 1;
	}
	return stateChanged = 0;
}


int TouchSwitch::read(){
	return (int)state;
}


byte TouchSwitch::rose() { return stateChanged && state; }

byte TouchSwitch::fell() { return stateChanged && !state; }


unsigned long TouchSwitch::duration(){
  return millis() - previous_millis;
}


void TouchSwitch::retrigger(unsigned long interval){
	 retrigger_millis = interval;
}


void TouchSwitch::interval(unsigned long interval_millis){
  this->interval_millis = interval_millis;
  retrigger_millis = 0;
}

unsigned long TouchSwitch::previousDuration()
{
  return durationOfPreviousState;
}

// Protected: triggers the pin
int TouchSwitch::trigger(){
  int newValue = flickerTouchRead(pin);
  int current_millis = millis();

  if (newValue > inHi){updateThreshold(inLo, newValue);}
  else if (newValue < inLo && (inLo - newValue) > 5){updateThreshold(newValue, inHi);}

  if(latched){ // LATCH behavior
    if (newValue >= onThreshold){
      if (waiting && held_millis >= interval_millis){
        state = !state;
        waiting = false;
        durationOfPreviousState = current_millis - previous_millis;
        previous_millis = current_millis;
        return 1;
      }
      else{return 0;}
    }
    else if (newValue <= offThreshold){
      waiting = true;
      held_millis = 0;
      return 0;
    }
    else{
      held_millis = 0;
      return 0;
    }
  }
  else{ // MOMENTARY behavior
    if (newValue >= onThreshold){
      if (waiting && held_millis >= interval_millis){
        if (!state){
          state = true;
          durationOfPreviousState = current_millis - previous_millis;
          previous_millis = current_millis;
          return 1;
        }
        else{
          waiting = false;
          return 0;
        }
      }
      else{
        if (!state){
          waiting = true;
          return 0;
        }
        else{return 0;}
      }
    }
    else if (newValue <= offThreshold){
      if(waiting && held_millis >= interval_millis){
        if (state){
          state = false;
          durationOfPreviousState = current_millis - previous_millis;
          previous_millis = current_millis;
          return 1;
        }
        else{
          waiting = false;
          return 0;
        }
      }
      else{
        if (state){
          waiting = true;
          return 0;
        }
        else{return 0;}
      }
    }
    else{
      held_millis = 0;
      return 0;}
  }
}
