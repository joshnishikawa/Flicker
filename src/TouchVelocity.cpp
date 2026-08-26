#include "TouchVelocity.h"

TouchVelocity::TouchVelocity(){};

TouchVelocity::TouchVelocity(byte pin){
  this->pin = pin;
};

TouchVelocity::~TouchVelocity(){};


void TouchVelocity::setThresholds(){
  flickerTouchInit();
  
  // Discard initial dummy reading (hardware warmup)
  flickerTouchRead(pin);
  delay(50);

  // Average 16 readings to establish quiescent baseline
  long total = 0;
  for (uint8_t i = 0; i < 16; i++) {
    total += flickerTouchRead(pin);
    delay(2);
  }
  int baseline = (int)(total / 16);

  int hoverDelta = max(20, (int)(baseline * 0.04)); // ~4% above baseline, min 20
  int touchDelta = max(60, (int)(baseline * 0.18)); // ~18% above baseline, min 60
  int hoverT = baseline + hoverDelta;
  int touchT = baseline + touchDelta;
  setThresholds(hoverT, touchT);
}


void TouchVelocity::setThresholds(int hoverThreshold, int touchThreshold){
  flickerTouchInit();
  hoverOnThreshold = hoverThreshold;
  touchOnThreshold = touchThreshold;
  int span = max(10, touchOnThreshold - hoverOnThreshold);
  touchOffThreshold = touchOnThreshold - max(5, (int)(span * 0.25));
  hoverOffThreshold = hoverOnThreshold - max(5, (int)(span * 0.20));
}


int TouchVelocity::read(){
  // Auto-calibrate if thresholds were never set
  if (hoverOnThreshold == 0 && touchOnThreshold == 0){
    setThresholds();
  }

  int newValue = flickerTouchRead(pin);

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
      return (int)timer;
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
