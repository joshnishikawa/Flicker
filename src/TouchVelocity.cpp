#include "TouchVelocity.h"

TouchVelocity::TouchVelocity(){};

TouchVelocity::TouchVelocity(byte pin){
  this->pin = pin;
};

TouchVelocity::~TouchVelocity(){};


void TouchVelocity::setThresholds(int hoverThreshold, int touchThreshold){
  hoverOnThreshold = hoverThreshold;
  touchOnThreshold = touchThreshold;
  touchOffThreshold = touchOnThreshold * 0.9;
  hoverOffThreshold = hoverOnThreshold * 0.99;
}


int TouchVelocity::read(){
  int newValue = touchRead(pin);

  if (state == 0){ // idle
    stateChanged = false;
    timer = 0;
    if (newValue >= hoverOnThreshold){
      state = 1;
    }
    return 0;
  }
  else if (state == 1){
    if (newValue >= touchOnThreshold){ // return velocity
      stateChanged = true;
      state = 2; // wait for release
      return timer;
    }
    else if (newValue <= hoverOffThreshold){ // return to idle
      state = 0;
      return 0;
    }
    else{ // keep waiting for touch
      return 0;
    }
  }
  else if (state == 2){ // waiting for release
    stateChanged = false;
    if (newValue <= hoverOffThreshold){
      stateChanged = true;
      state = 0; // idle
    }
    else{stateChanged = false;}
    return 0;
  }
  else return 0;
};


byte TouchVelocity::rose(){
  return stateChanged && state == 2;
};

byte TouchVelocity::fell(){
  return stateChanged && state == 0;
};
