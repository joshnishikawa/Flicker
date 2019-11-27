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

TouchSwitch::TouchSwitch(byte pin, byte mode, int onThreshold){
  this->pin = pin;
  latched = mode;
  this->onThreshold = onThreshold;
  offThreshold = onThreshold / 1.3 * 1.2;
};

TouchSwitch::~TouchSwitch(){};


void TouchSwitch::setThreshold(){
  int newValue = touchRead(pin);
  onThreshold =  newValue * 1.3; // Threshold for rising edge
  offThreshold = newValue * 1.2; // Slightly lower for falling edge
}


void TouchSwitch::setThreshold(int threshold){
  onThreshold = threshold; // Threshold for rising edge
  offThreshold = threshold / 1.3 * 1.2; // Slightly lower for falling edge
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


byte TouchSwitch::risingEdge() {return stateChanged && state; }

byte TouchSwitch::fallingEdge() {return stateChanged && !state; }


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


// Protected: triggers the pin
int TouchSwitch::trigger(){
  int newValue = touchRead(pin);

  if(latched){ // LATCH behavior
    if (newValue >= onThreshold){
      if (waiting && held_millis >= interval_millis){
        state = !state;
        waiting = false;
        previous_millis = millis();
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
          previous_millis = millis();
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
          previous_millis = millis();
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
        else{return 0;
        }
      }
    }
    else{
      held_millis = 0;
      return 0;}
  }
}

