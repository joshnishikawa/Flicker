#include "TouchVelocity.h"

TouchVelocity::TouchVelocity(){};

TouchVelocity::TouchVelocity(byte pin){
  this->pin = pin;
  setThreshold(); // only works if creating objects during setup
};

TouchVelocity::TouchVelocity(byte pin, int outLo, int outHi){
  this->pin = pin;
  setThreshold(); // only works if creating objects during setup
  this->outLo = outLo;
  this->outHi = outHi;
};

TouchVelocity::~TouchVelocity(){};


int TouchVelocity::read(){
  int newValue = touchRead(pin);
  if (state == 0){ // idle
    stateChanged = false;
    timer = 0;
    if (newValue >= hoverOnThreshold){
      state = 1;
    }
    newValue = 0;
  }
  else if (state == 1){
    if (newValue >= touchOnThreshold){ // return velocity
      newValue = timer;
      stateChanged = true;
      state = 2; // wait for release
    }
    else if (newValue <= hoverOffThreshold){ // return to idle
      newValue = 0;
      state = 0;
    }
    else{ // keep waiting for touch
      newValue = 0;
    }
  }
  else if (state == 2){ // waiting for release
    stateChanged = false;
    if (newValue <= hoverOffThreshold){
      stateChanged = true;
      state = 0; // idle
    }
    else{stateChanged = false;}
    newValue = 0;
  }
  else{newValue = 0;};
  return newValue;
};


int TouchVelocity::responsiveRead(){
  int newValue = touchRead(pin);
  inHi = newValue>inHi*1.1?newValue:inHi; // auto detect range for lo-vel hits
  if (state == 4){ // idling
    if (newValue >= hoverOnThreshold){
      state = 1; // look for a high velocity hit
      timer = 0;
    }
    newValue = 0;
  }
  else if (state == 1){ // looking for a high velocity hit
    if (newValue <= hoverOffThreshold){
      state = 4; // idle
      newValue = 0;
    }
    else if (newValue >= touchOnThreshold){ // RETURN HI-VELOCITY HIT
      // hi-velocity hit happens between about 1600~9000 micros
      newValue = map(int(timer), 9000, MAX_VELOCITY, outHi/2, outHi);
      newValue = constrain(newValue, outHi/2, outHi);
      stateChanged = true;
      state = 2; // wait for release
    }
    else if (timer >= 9001){ // not fast enough for hi-velocity
      state = 0; // look for a low-velocity hit
      newValue = 0;
    }
    else{ // keep waiting for touch
      newValue = 0;
    }
  }
  else if (state == 0){ // looking for a low velocity hit
    stateChanged = false;
    if (newValue <= hoverOffThreshold){
      state = 4; // idle
      newValue = 0;
    }
    else if (newValue >= touchOnThreshold){
      state = 3; // look for a peak
      timer = 0;
      peak = newValue;
      newValue = 0;
    }
    else{ // keep waiting for a touch
      newValue = 0;}
  }
  else if (state == 3){ // looking for peak
    if (newValue <= peak || timer >= 5000){
      newValue = map(peak, touchOnThreshold, inHi, outLo, outHi/2);
      newValue = constrain(newValue, outLo, outHi/2);
      peak = 0;
      stateChanged = true;
      state = 2; // wait for release
    }
    else { // keep track of the peak
      peak = newValue;
      newValue = 0;
    }
  }
  else if (state == 2){ // waiting for release
    stateChanged = false;
    if (newValue <= touchOffThreshold){
      stateChanged = true;
      state = 0; // look for a low velocity hit
    }
    else{stateChanged = false;
    }
    newValue = 0;
  }
  else{newValue = 0;}
  return newValue;
}

byte TouchVelocity::rose(){
  return stateChanged && state == 2;
  };

byte TouchVelocity::fell(){
  return stateChanged && state == 0;
};


void TouchVelocity::setThreshold(){
  int qval = 0;
  analogRead(A0); // The ADC can affect touch values so fire it up first.

  for (int i = 0; i < 10; i++){ // store the highest quiescent reading of 10
    int newValue = touchRead(pin);
    if (newValue > qval) qval = newValue;
  }

  hoverOnThreshold = qval * 1.02; // when to start looking for a touch
  hoverOffThreshold = qval * 1.01; // when to idle
  touchOnThreshold = qval * 1.2; // when touched
  touchOffThreshold = qval * 1.1; // when released
}

void TouchVelocity::setThreshold(int hoverThreshold, int touchThreshold){
  hoverOnThreshold = hoverThreshold;                // when to look for a touch
  hoverOffThreshold = hoverThreshold * 1.01 / 1.02; // when to idle
  touchOnThreshold = touchThreshold;                // when touched
  touchOffThreshold = touchThreshold * 1.1 / 1.2;   // when released
}

void TouchVelocity::setOutputRange(int outLo, int outHi){
  this->outLo = outLo;
  this->outHi = outHi;
};

