#ifndef Flicker_h
#define Flicker_h

#include "Arduino.h"
#include "TouchSwitch.h"
#include "TouchVariable.h"
#include "TouchVelocity.h"

// TouchVariable and TouchVelocity are now included in the Flicker libary.
// So, for continuity, the "Flicker" class has been changed to "TouchSwitch".
// "Flicker" continues to be included here for backward compatibility.
class Flicker: public TouchSwitch{
  public: 
  Flicker() : TouchSwitch(){}
  Flicker(byte pin) : TouchSwitch(pin){}
  Flicker(byte pin, int threshold) : TouchSwitch(pin, 0, threshold){}
  Flicker(byte pin, byte mode, int threshold) : TouchSwitch(pin, mode, threshold){}
};

#endif

