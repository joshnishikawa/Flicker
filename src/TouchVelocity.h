#include "Arduino.h"

#ifndef TouchVelocity_h
#define TouchVelocity_h

#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MKL26Z64__)
  #define MAX_VELOCITY 3000
#elif defined(__MK66FX1M0__) || defined(__MK64FX512__)
  #define MAX_VELOCITY 1600
#else
  #define MAX_VELOCITY 3000
#endif

class TouchVelocity{

  public:
    // Default constructor
    TouchVelocity();

    // Initialize a velocity touch input.
    TouchVelocity(byte pin);

    // Initialize a velocity touch input with a predetermined output range.
    // WARNING! Also call setInputRange() for this to work properly.
    TouchVelocity(byte pin, int outLo, int outHi);
    
    // Destructor
    ~TouchVelocity();

    // setThreshold() assumes the input is NOT being touched and automatically
    // calculates thresholds for 'hovered' and 'touched'. Use in setup().
    void setThreshold();

    // Use this any time for specific thresholds (touchRead() is not called)
    void setThreshold(int hoverThreshold, int touchThreshold);
    
    // Maps the values from responsiveRead() to a predetermined output range.
    void setOutputRange(int outLo, int outHi);

    // On rising edge, this returns the amount of time (in micros) between
    // breaking the hoverOnThreshold and breaking the touchOnThreshold.
    // Returns 0 if not rising edge.
    int read();
    
    // read() is a raw velocity reading and requires that the input go below the
    // hoverOffThreshold before a new value can be determined.
    // responsiveRead() works much better for things like finger drumming.
    // *See notes below
    int responsiveRead();

    // rose() is true for 1 scan after input goes above touchOnThreshold
    byte rose();
    byte risingEdge(){ return rose(); }

    // fell() is true for 1 scan after input goes 
    //     below hoverOffThreshold when read() is used OR
    //     below touchOffThreshold when responsiveRead() is used
    byte fell();
    byte fallingEdge(){ return fell(); }

    int hoverOnThreshold, hoverOffThreshold, touchOnThreshold, touchOffThreshold;

  protected:
    byte pin;
    bool stateChanged;
    int inHi, state, peak;
    int outLo = 1;
    int outHi = 127;
    elapsedMicros timer;
};

#endif

/*  HOW RESPONSIVE READ WORKS
    
    Assuming you're using your finger to trigger the input, it will start to
    change the value of touchRead() before actually making contact.
    When detected, the function will wait 9 milliseconds for your finger to
    make contact. Your finger might make contact in as fast as 1.6 milliseconds.
    
    If it takes less than 9 milliseconds to make contact, that means your finger
    is moving pretty fast. So velocity values between 1.6 and 9 milliseconds are
    mapped to the TOP HALF of the output range. This is a true 'velocity'.

    However, if contact is made anytime after 9 milliseconds, that means your
    finger is moving slow enough that a 'peak detect' approach can be used.
    When contact is made, the highest touchRead() value is stored. As soon as 
    the value decreases OR 5 milliseconds elapses (whichever is first), the 
    touchRead() value is mapped to the BOTTOM HALF of the output range.
    
    This allows for better low-velocity articulation because your finger doesn't
    need to move far from the input before another velocity reading can be
    determined. And a true velocity reading is better for high velocity-hits
    because the sponginess of your finger doesn't count for much when moving
    that fast and touchRead() values just spike. 
    
    It would probably be possible to use the 'peak detect' approach for the
    entire output range if there were a very thin layer of very soft sponge
    above the metal. But responsiveRead() works for solid surfaces like glass or
    just a piece of metal with a layer of celophane tape on it. Only the latter
    has been tested.
*/

