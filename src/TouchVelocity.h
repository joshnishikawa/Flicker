#include "Arduino.h"

#ifndef TouchVelocity_h
#define TouchVelocity_h

class TouchVelocity{

  public:
    // Default constructor
    TouchVelocity();

    // Initialize a velocity touch input.
    TouchVelocity(byte pin);

    // Destructor
    ~TouchVelocity();

    // Use this any time for specific thresholds (touchRead() is not called)
    void setThresholds(int hoverThreshold, int touchThreshold);
    
    // On rising edge, this returns the amount of time (in micros) between
    // breaking the hoverOnThreshold and breaking the touchOnThreshold.
    // Returns 0 if not rising edge.
    int read();
    
    // rose() is true for 1 scan after input goes above touchOnThreshold
    byte rose();
    byte risingEdge(){ return rose(); }

    // fell() is true for 1 scan after input goes 
    //     below hoverOffThreshold when read() is used OR
    //     below touchOffThreshold when responsiveRead() is used
    byte fell();
    byte fallingEdge(){ return fell(); }


  protected:
    byte pin;
    int state, 
        touchOnThreshold, touchOffThreshold, 
        hoverOnThreshold, hoverOffThreshold;
    bool stateChanged;
    elapsedMicros timer;
};

#endif
