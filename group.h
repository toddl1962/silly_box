/////////////////////////////////////////////////////////////////////////////////////////////
//
// group class provides processing for a group of parallel sequences
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

class group
{
  public:
    enum groupState
    {
      GROUP_NOT_EXECUTING,
      GROUP_COMPLETE,
      GROUP_EXECUTING
    };
  
  // Methods
  public:
    group(sequence* const* sequenceTbl);    
    ~group();    
    void reset();
    void start();
    groupState getState(); 
    bool getSwitchOffAttempted();
    groupState loop(); 

  // Attributes
  private:
    sequence* const* m_pSequenceTbl;
    groupState m_groupState;
};
