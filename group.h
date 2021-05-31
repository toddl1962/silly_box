/////////////////////////////////////////////////////////////////////////////////////////////
//
// group class provides processing for a group of parallel sequences
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
