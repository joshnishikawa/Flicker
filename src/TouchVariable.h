#include "Arduino.h"

#ifndef TouchVariable_h
#define TouchVariable_h

class TouchVariable{

  public:
    // Default constructor
    TouchVariable();

    // Initialize a variable touch input.
    TouchVariable(byte pin);

    // Initialize a variable touch input with a predetermined output range.
    // WARNING! Also call setInputRange() for this to work properly.
    TouchVariable(byte pin, int outLo, int outHi);
    
    // Destructor
    ~TouchVariable();

    // setInputRange() assumes the input is NOT being touched and automatically
    // calculates a usable range. In setup() is a good place to call this Fn.
    void setInputRange();

    // Specific values can also be given as arguments. 
    void setInputRange(int inLo, int inHi);

    // Maps the inLo and inHi to a predetermined output range.
    void setOutputRange(int outLo, int outHi);

    // Returns the value of touchRead() with some noise reduction.
    int read();
    
    // Sets the level of noise reduction. 0 for none, 
    void setNR(int amount); // 0 = raw touchRead() values. 100 = maximum NR.
    
    int inLo = 0;
    int inHi = 0;
    int outLo = 0;
    int outHi = 0;
    
  protected:
    byte pin = 0;
    bool mapped = false; // true maps input to a predetermined output range
    bool adjustInHi = true;//false if using setInputRange(int inLo, int inHi)
    
    int buffer = 0;   /* Think of this variable as a container like a capacitor
                        acting as a low-pass filter. */

    int threshold = 0;// This would be the variable capacity of the capacitor.

    float NR = 100.0; /* This is the percent of the touchRead() value that will
                        be used as the threshold. A percent of the touchRead()
                        value is used because higher touchRead() values have
                        greater variance, thus requiring more noise reduction.*/

    int balancedValue = 0; // The "noise-reduced" output of touchRead()

};

#endif