/////////////////////////////////////////////////////////////////////////////////////////////
//
// The sequence class supplies the basic processing of the actions including management of 
// the sequence table entries and initialization of the processing context for each action.
// Derived classes are responsible for preparing and executing the action by overriding 
// the prepareAction() and executeAction() methods.  Derived classes should also call the
// base class prepareAction() and executeAction() methods.
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
#include "debug.h"
#include "action.h"

class sequence
{
  // Enumerations
  public:
    enum seqState
    {
      SEQ_NOT_EXECUTING,
      SEQ_COMPLETE,
      SEQ_EXECUTING
    };

    enum seqType
    {
      PRIMARY_SEQ,
      SECONDARY_SEQ
    };

    enum seqEnd
    {
      ONE_SHOT,
      REPEATING
    };

    enum actionState
    {
      ACTION_EXECUTING,
      ACTION_COMPLETE
    };

    // Structures
    struct seqEntry
    {
      actionType action;
      uint32_t data1;
      uint32_t data2;
      uint32_t data3; // delay in ms
    };

  // Construction
  public:
  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  // ! Sequence tables are in program memory (PROGMEM) in order to save RAM.
  // ! This class is designed such that sequence tables MUST be in program memory
  // ! due to the limitations on how that memory is accessed.
  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    sequence(const void*, seqType = PRIMARY_SEQ, seqEnd = ONE_SHOT);
    ~sequence();

  // Public Methods
  public:
    seqState processSequence();
    seqType getSeqType();     
    seqEnd getSeqEnd();
    seqState getSeqState();
    bool getSwitchOffAttempted();
    virtual void startSequence();
    virtual void stopSequence();

  protected:
    virtual void prepareAction();
    virtual actionState executeAction();
  
  private:
    // Sequence Table Information
    const uint8_t* m_pSeqTable;
    const uint8_t* m_pSeqEntry;
    seqType m_seqType;     
    seqEnd m_seqEnd;
    seqState m_seqState;

  protected:
    // Context for processing current action
    seqEntry m_seqEntry;
    uint32_t m_prevMillis; // used for timing delays
    bool m_switchOffAttempted;
};
