/////////////////////////////////////////////////////////////////////////////////////////////
//
// Implementation for the sequence class
//
/////////////////////////////////////////////////////////////////////////////////////////////

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !! Sequence tables are in program memory (PROGMEM) in order to save RAM.      !!
// !! This class is designed such that sequence tables MUST be in program memory !!
// !! due to the limitations on how that memory is accessed.                     !!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

#include "sequence.h"

sequence::sequence(const void* pSeqTable, seqType aSeqType, seqEnd aSeqEnd)
{
  m_pSeqTable = static_cast<const uint8_t*> (pSeqTable);
  m_seqType = aSeqType;
  if (m_seqType == PRIMARY_SEQ) m_seqEnd = ONE_SHOT;
  else m_seqEnd = aSeqEnd;
  m_pSeqEntry = m_pSeqTable;
  memcpy_P(&m_seqEntry, m_pSeqEntry, sizeof(seqEntry));
  m_prevMillis = millis();
  m_seqState = SEQ_EXECUTING;
}

sequence::~sequence()
{
}

sequence::seqType sequence::getSeqType()
{
  return m_seqType;     
}

sequence::seqEnd sequence::getSeqEnd()
{
  return m_seqEnd;
}

sequence::seqState sequence::getSeqState()
{
  return m_seqState;
}

bool sequence::getSwitchOffAttempted()
{
  return m_switchOffAttempted;     
}

//
// processSequence
//
// This function is responsible for processing actions and determining if the current sequence is complete.  If 
// the sequence is complete the SEQ_COMPLETE value is returned, otherwise SEQ_EXECUTING is returned.
//
// While sequences are being processed this method should be called every time the loop() function is executed.
//  

sequence::seqState sequence::processSequence()
{
  // NOTE:  If this is a ONE_SHOT sequence and all of the actions are completed then
  // prepareAction() will return a SEQ_COMPLETE.  Once this happens all calls to 
  // processAction() will return a SEQ_COMPLETE.
  
  // Execute the action
  actionState actState = executeAction();

  // If the action is complete move to the next action and check for the end of the
  // sequence.
  if (m_seqState == SEQ_EXECUTING && actState == ACTION_COMPLETE)
  {
    m_pSeqEntry += sizeof(seqEntry);
    memcpy_P(&m_seqEntry, m_pSeqEntry, sizeof(seqEntry));
    if (m_seqEntry.action == ACTION_END) 
    {
      // At the end of the sequence table
      if (m_seqEnd == ONE_SHOT)
      {
        // Move sequence completed
        m_seqState = SEQ_COMPLETE;
        return SEQ_COMPLETE;
      }
      else // m_seqEnd == REPEATING
      {
        // Repeat sequence
        m_pSeqEntry = m_pSeqTable;
        memcpy_P(&m_seqEntry, m_pSeqEntry, sizeof(seqEntry));
      }
    }
    m_prevMillis = millis();
    prepareAction();
  }
  return m_seqState;
}

void sequence::startSequence()
{
  m_pSeqEntry = m_pSeqTable;
  memcpy_P(&m_seqEntry, m_pSeqEntry, sizeof(seqEntry));
  m_prevMillis = millis();
  prepareAction();
  m_seqState = SEQ_EXECUTING;
  m_switchOffAttempted = false;
}

void sequence::stopSequence()
{
  m_seqState = SEQ_COMPLETE;
}

void sequence::prepareAction()
{
  if (m_seqEntry.action == ACTION_DELAY) m_prevMillis = millis();
}

sequence::actionState sequence::executeAction()
{
  if (m_seqEntry.action == ACTION_DELAY)
  {
    if (millis() - m_prevMillis > m_seqEntry.data1) return ACTION_COMPLETE;
    else return ACTION_EXECUTING;
  }
  
  // If we got to here then we didn't recognize the action.
  return ACTION_COMPLETE;
}
