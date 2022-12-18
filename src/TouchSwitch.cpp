#include "TouchSwitch.h"

TouchSwitch::TouchSwitch(){};

TouchSwitch::TouchSwitch(byte pin){
  this->pin = pin;
  latched = false;
};

TouchSwitch::TouchSwitch(byte pin, byte mode){
  this->pin = pin;
  this->latched = mode;
};

TouchSwitch::TouchSwitch(byte pin, byte mode, int onThreshold){
  this->pin = pin;
  latched = mode;
  this->onThreshold = onThreshold;
  offThreshold = onThreshold / 1.2 * 1.1;
};

TouchSwitch::~TouchSwitch(){};


void TouchSwitch::setThreshold(){
  int qval = 0;
  analogRead(A0); // The ADC can affect touch values so fire it up first.

  for (int i = 0; i < 10; i++){ // store the highest quiescent reading of 10
    int newValue = touchRead(pin);
    if (newValue > qval) qval = newValue;
  }

  onThreshold =  qval * 1.2; // Threshold for rising edge
  offThreshold = qval * 1.1; // Slightly lower for falling edge
}


void TouchSwitch::setThreshold(int threshold){
  onThreshold = threshold; // Threshold for rising edge
  offThreshold = threshold / 1.2 * 1.1; // Slightly lower for falling edge
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


byte TouchSwitch::rose() {return stateChanged && state; }

byte TouchSwitch::fell() {return stateChanged && !state; }


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

