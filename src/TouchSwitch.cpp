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
  analogRead(A0); // The ADC can affect touch values so fire it up first.
  inLo = touchRead(pin);
  onThreshold = inLo * 1.1;  // initial threshold 10% above quiescent
  updateThreshold(inLo, onThreshold); // updated on higher reading
}


void TouchSwitch::setThreshold(int threshold){
  userSetThreshold = true;
  onThreshold = threshold;
  analogRead(A0); // The ADC can affect touch values so fire it up first.
  inLo = touchRead(pin);

  // breaks if user-set threshold is lower than the quiescent reading
  updateThreshold(inLo, onThreshold);
}


void TouchSwitch::updateThreshold(int low, int high){ // internal use only
  inLo = low;
  inHi = high;
  int range = inHi - inLo;

  if (!userSetThreshold){
    // Assumes that halfway between lowest & highest readings is a good place
    // for a threshold. Still a magic number but at least it's somewhat dynamic.
    onThreshold = inLo + range * 0.5;
    offThreshold = inLo + range * 0.3;
  }
  else{
    // The user can forego assumptions and set the threshold themselves.
    // We need to know where the threshold is in relation to the range of values
    float sens = float(onThreshold - inLo) / float(range);
    // Then we set the offThreshold to be 10% below the onThreshold
    offThreshold = inLo + range * (sens - 0.1);
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
  int newValue = touchRead(pin);
  int current_millis = millis();

  if (newValue > inHi){updateThreshold(inLo, newValue);}
  else if (newValue < inLo){updateThreshold(newValue, inHi);}

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
