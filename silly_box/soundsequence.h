/////////////////////////////////////////////////////////////////////////////////////////////
//
// moveSequence class provides processing of sound.  It is derived from the sequence 
// base class.
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

class soundSequence: public sequence
{
  // Construction/Destruction
  public:
    soundSequence(const void*, seqType = PRIMARY_SEQ, seqEnd = ONE_SHOT);

  // Methods
  public:
    static void setup();
    void startSequence();
    void stopSequence();

  private:
    void prepareAction();
    actionState executeAction();

  // Attributes
  private:  
    static const int speakerPin = 7;
    // Additional context needed for processing actions
    uint32_t m_tempoNoteMs;
    float m_articulation;
};
