/////////////////////////////////////////////////////////////////////////////////////////////
//
// ledSequence class provides processing specific to the 2 RGB LEDs.  It is derived
// from the sequence base class.
//
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
//
//  Copyright 2021, Todd W. Lumpkin
//
//  This file is part of the "Silly Box" program.
//
//  "Silly Box" is free software: you can redistribute it and/or modify it under the terms 
//  of the GNU General Public License as published by the Free Software Foundation, 
//  version 3 of the License.
//
//  "Silly Box" is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
//  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//  See the GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License along with "Silly Box" 
//  in a file named gpl-3.0.txt.  If not, see <https://www.gnu.org/licenses/>.
//
/////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "sequence.h"
#include "movesequence.h"

//
// This sequence class handles the lighting patterns of the LEDs
//

class ledSequence: public sequence
{
  public:
    static const int LEFT = 0;
    static const int RIGHT = 1;
    static const int ALL_LEDS = 0xFFF;

  private:
    static const int ledPins[2][3];
    static const int numLeds = sizeof(ledPins)/sizeof(ledPins[0]);

  // Construction/Destruction
  public:
    ledSequence(const void*, seqType = PRIMARY_SEQ, seqEnd = ONE_SHOT);

  // Methods
  public:
    static void setup();
    void setLed(int, const uint32_t);
    actionState transitionLed();
    void startSequence();
    void stopSequence();

  private:
    void prepareAction();
    actionState executeAction();

  // Attributes
  private:  
    // Additional context needed for processing LED actions
    uint32_t m_lastColor[numLeds];
};

