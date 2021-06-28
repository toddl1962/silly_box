/////////////////////////////////////////////////////////////////////////////////////////////
//
// Implementation for the soundSequence class
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

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !! I implemented this so as to be able to create some basic tunes.   Obvious    !!
// !! limitations are the quality of the sound, the time signatures which can be   !!
// !! used, and no support for dynamics.  The idea is to use an inexpensive piezo  !!
// !! (passive) device.                                                            !!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

// Define preprocessor DEBUG to enable debug output.  Also, be sure to use the F macro when
// printing strings in order to save RAM.

//#define DEBUG

#include "soundsequence.h"

soundSequence::soundSequence(const void* pSeqTable, seqType aSeqType, seqEnd aSeqEnd):sequence(pSeqTable, aSeqType, aSeqEnd)
{
  startSequence();
}

void soundSequence::setup()
{
  // Make sure we are silent!
  noTone(speakerPin);
}

void soundSequence::startSequence()
{
  // Set default tempo and articulation
  m_tempoNoteMs = 60000/TEMPO_ADAGIO/8;
  m_articulation = ARTICULATE_STACCATO/100;
  
  // Call base class
  sequence::startSequence();
  
  // Make sure tone is off
  noTone(speakerPin);
}

void soundSequence::stopSequence() 
{
  // Call base class
  sequence::stopSequence();
  
  // Make sure tone is off
  noTone(speakerPin);
}

sequence::actionState soundSequence::executeAction()
{
  if (m_seqEntry.action < ACTION_LAST_GENERIC)
  {
    // Generic action.  Call base class.
    return sequence::executeAction();
  }

  if (millis() - m_prevMillis >= m_seqEntry.data2)
  {
    // Sound action delay has expired.  Turn off sound and
    // indicate action complete.
    noTone(speakerPin);
    return ACTION_COMPLETE;
  }
  // If we got to here then the action is still executing
  return ACTION_EXECUTING;
}

void soundSequence::prepareAction()
{
  switch (m_seqEntry.action)
  {
    case TEMPO:
      //
      // Tempo is beats per minute (BPM).  Since we time all notes based on the
      // length of a 32nd note in milliseconds, we need divide 60000ms by both the
      // BPM and the number of 32nd notes in a quarter note.
      // 
      m_tempoNoteMs = 60000/m_seqEntry.data1/NOTE_QTR;
      m_seqEntry.data2 = 0; // force action complete on next call to executeAction()
      break;
      
    case ARTICULATE:
      //
      // Articulation is implemented as a percentage of the full duration of a note.
      //
      m_articulation = (static_cast<float>(m_seqEntry.data1))/100;
      m_seqEntry.data2 = 0; // force action complete on next call to executeAction()
      break;

    case PITCH_REST:
      // Reuse data2 to store the length of the rest
      m_seqEntry.data2 = m_tempoNoteMs*m_seqEntry.data1;
      break;
      
    default:
      if (m_seqEntry.action < ACTION_LAST_GENERIC)
      {
        // Generic action
        sequence::prepareAction();
      }
      else
      {
        // By deduction must be a note
        
        // Reuse data2 to store the length of the note
        m_seqEntry.data2 = m_tempoNoteMs*m_seqEntry.data1;
        
        // Start the tone with a duration scaled by the articulation value
        tone(speakerPin, m_seqEntry.action, static_cast<unsigned long>(m_seqEntry.data2 * m_articulation));
      }
      break;
  }
}
