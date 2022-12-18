# Flicker 1.1.4
### A library for making capacitive touch easy to use.
###### by Josh Nishikawa <github.com/joshnishikawa/Flicker>
###### The "TouchSwitch" class was adapted from the Bounce library by: Thomas O Fredericks, Eric Lowry, Jim Schimpf and Tom Harkaway
Includes all the functions found in the Bounce library, smoothes eratic values to make variable input more useful and includes functions for measuring velocity.
___
### VERSION LOG:

1.1.4 
- Added previousDuration() function that can be used on rising/falling edge to return the length of the input's previous state.
- Added rose() and fell(). risingEdge() and fallingEdge() still work.
- Now using bool data type where appropriate.

1.0.4 
   - created a library.properties file
   - made the version number semver compliant (1.0.4 is the first official release)
   - moved source files into src/

1.0.3
  - Fixed a bug that pushed the quiescent touchRead value above the
    offThreshold when using the ADC.
    
1.0.2
  - Fine tuned ON and OFF thresholds for TouchSwitch and TouchVariable

1.0.0
  - Changed "Flicker" to "TouchSwitch" ("Flicker" deprecated but still works)
  - Added an option to LATCH the state of a TouchSwitch.
  - Added interval() (still useful even if noise reduction isn't time-based)
  - Added the TouchVariable class for stable reading of ranges of input.
  - Added the TouchVelocity class for detecting velocity of changes in input.
  - Started keeping this version log.
___
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
 MA 02110-1301, USA.
