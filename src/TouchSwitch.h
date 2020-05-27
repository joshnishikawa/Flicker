#include "Arduino.h"

#ifndef TouchSwitch_h
#define TouchSwitch_h

#define MOMENTARY 0
#define LATCH 1


class TouchSwitch{

  public:
    // Default constructor
    TouchSwitch();
    
    // Initialize a momentary switch (Also use setThreshold()).
    TouchSwitch(byte pin);

    // Initialize a latched switch (Also use setThreshold()).
    TouchSwitch(byte pin, byte mode);

    // Initialize a momentary or latched switch with a specific threshold.
    TouchSwitch(byte pin, byte mode, int onThreshold);

    // Destructor
    ~TouchSwitch();

    // setThreshold() assumes the input is NOT being touched and automatically
    // calculates a threshold. In setup() is a good place to call this function.
    void setThreshold();

    // A specific value can also be given as an argument. 
    void setThreshold(int threshold);

    // Updates the pin. Returns 1 if state changed ; Returns 0 if not.
    int update(); 

    // Returns the updated pin state.
    int read();

    // risingEdge() is true for 1 scan after input goes above the threshold.
    byte risingEdge();

    // fallingEdge() is true for 1 scan after input goes below the threshold.
    byte fallingEdge();

    // Returns the number of milliseconds the pin has been in the current state.
    unsigned long duration();

    // Forces the pin to signal a change (through update()) in X milliseconds 
    // even if the state does not actually change.
    // Example: press and hold a button and have it repeat every X milliseconds
    void retrigger(unsigned long interval); 

   	// A 'debounce' interval isn't really needed but, if you want one...
    void interval(unsigned long interval_millis); 

  protected:
    int trigger();
    byte state = 0;
    int onThreshold = 0;
    int offThreshold = 0;
    elapsedMillis held_millis = 0;
    unsigned long previous_millis = 0;
    unsigned long interval_millis = 0;
    unsigned long retrigger_millis = 0;
    byte pin = 0;
    byte latched = false;
    byte waiting = false;
    byte stateChanged = false;
};

#endif