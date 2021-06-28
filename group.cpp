/////////////////////////////////////////////////////////////////////////////////////////////
//
// Implementation of the group class
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

// Define preprocessor DEBUG to enable debug output.  Also, be sure to use the F macro when
// printing strings in order to save RAM.

//#define DEBUG

#include "group.h"

//
// group class implementation
//

group::group(sequence* const* sequenceTbl)
{
  m_pSequenceTbl = sequenceTbl;
  m_groupState = GROUP_EXECUTING;
}

group::~group()
{
}

group::groupState group::getState() 
{
  return m_groupState;
}

bool group::getSwitchOffAttempted()
{
  sequence* pSequence;

  // Iterate through the table in PROGMEM using 'pgm_read_ptr_near'
  for (int i = 0; 
       (pSequence = static_cast<sequence *> (pgm_read_ptr_near(&m_pSequenceTbl[i]))) != NULL; 
       i++)
  {
    if (pSequence->getSeqType() == sequence::PRIMARY_SEQ)
    {
       return pSequence->getSwitchOffAttempted();
    }
  }
}

void group::reset()
{
  sequence* pSequence;

  // Iterate through the table in PROGMEM using 'pgm_read_ptr_near'
  for (int i = 0; 
       (pSequence = static_cast<sequence *> (pgm_read_ptr_near(&m_pSequenceTbl[i]))) != NULL; 
       i++)
  {
    pSequence->stopSequence();
  }
  m_groupState = GROUP_NOT_EXECUTING;
}

void group::start()
{
  sequence* pSequence;

  // Iterate through the table in PROGMEM using 'pgm_read_ptr_near'
  for (int i = 0; 
       (pSequence = static_cast<sequence *> (pgm_read_ptr_near(&m_pSequenceTbl[i]))) != NULL; 
       i++)
  {
    pSequence->startSequence();
  }
  m_groupState = GROUP_EXECUTING;
}

group::groupState group::loop() 
{
  if (m_groupState == GROUP_COMPLETE || m_groupState == GROUP_NOT_EXECUTING) return GROUP_COMPLETE;

  sequence* pSequence;

  // Iterate through the table in PROGMEM using 'pgm_read_ptr_near'
  for (int i = 0; 
       (pSequence = static_cast<sequence *> (pgm_read_ptr_near(&m_pSequenceTbl[i]))) != NULL; 
       i++)
  {
    pSequence->processSequence();
    if (pSequence->getSeqType() == sequence::PRIMARY_SEQ 
    &&  pSequence->getSeqState() == sequence::SEQ_COMPLETE)
    {
       m_groupState = GROUP_COMPLETE;
       break;
    }
  }
  
  if (m_groupState == GROUP_COMPLETE)
  {
    // stop all sequences
   // Iterate through the table in PROGMEM using 'pgm_read_ptr_near'
    for (int i = 0; 
         (pSequence = static_cast<sequence *> (pgm_read_ptr_near(&m_pSequenceTbl[i]))) != NULL; 
         i++)
    {
      pSequence->stopSequence();
    }
  }
  return m_groupState;
}
