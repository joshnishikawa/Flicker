/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

 FLICKER 2.0 - A library for making capacitive touch easy to use.
 - by Josh Nishikawa (github.com/joshnishikawa/Flicker)

 - The "TouchSwitch" class was adapted from the Bounce library by:
   Thomas O Fredericks, Eric Lowry, Jim Schimpf and Tom Harkaway

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

 VERSION LOG
  2.0 (current)
  - Changed "Flicker" to "TouchSwitch" ("Flicker" deprecated but still works)
  - Added an option to LATCH the state of a TouchSwitch.
  - Added interval() (still useful even if noise reduction isn't time-based)
  - Added the TouchVariable class for stable reading of ranges of input.
  - Added the TouchVelocity class for detecting velocity of changes in input.
  - Started keeping this version log.

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 MA 02110-1301, USA.*/

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

