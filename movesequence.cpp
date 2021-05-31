/////////////////////////////////////////////////////////////////////////////////////////////
//
// This is the implementation of the moveSequence class
//
// The servos are MG996R Servo Motors.
//
/////////////////////////////////////////////////////////////////////////////////////////////

// Define preprocessor DEBUG to enable debug output.  Also, be sure to use the F macro when
// printing strings in order to save RAM.

#define DEBUG

#include "movesequence.h"

// Initialize static members of moveSequence
Servo moveSequence::armServo;
Servo moveSequence::lidServo;

//
// Implementation for the moveSequence class
//

moveSequence::moveSequence(const void* pSeqTable, seqType aSeqType, seqEnd aSeqEnd):sequence(pSeqTable, aSeqType, aSeqEnd)
{
  startSequence();
}

moveSequence::moveSequence(const void* pSeqTable):sequence(pSeqTable, PRIMARY_SEQ, ONE_SHOT)
{
  startSequence();
}

void moveSequence::setup()
{
  // Attach servos to their pins
  moveSequence::armServo.attach(moveSequence::armServoPin);
  moveSequence::lidServo.attach(moveSequence::lidServoPin);
  
  // Move servos to a known position
  moveSequence::armServo.write(moveSequence::armRetractedAngle);
  moveSequence::lidServo.write(moveSequence::lidClosedAngle);
}

void moveSequence::startSequence()
{
  // Call base class
  sequence::startSequence();
}

void moveSequence::stopSequence() 
{
  // Move the servos to a known position
  armServo.write(armRetractedAngle);
  lidServo.write(lidClosedAngle);
  
  // Call base class
  sequence::stopSequence();
}

void moveSequence::prepareAction()
{
  int peekAngle;
  
  // Prepare action
  switch (m_seqEntry.action)
  {
    // These cases will be executed immediately when the move is processed.
    case ACTION_OPEN_LID:
      DebugPrintln(F("ACTION_OPEN_LID"));
      m_pServo = &lidServo;
      m_seqEntry.data1 = lidOpenedAngle;
      break;

    case ACTION_CLOSE_LID:
      DebugPrintln(F("ACTION_CLOSE_LID"));
      m_pServo = &lidServo;
      m_seqEntry.data1 = lidClosedAngle;
      break;

    case ACTION_EXTEND_ARM:
      DebugPrintln(F("ACTION_EXTEND_ARM"));
      m_pServo = &armServo;
      m_seqEntry.data1 = armExtendedAngle;
      break;

    case ACTION_RETRACT_ARM:
      DebugPrintln(F("ACTION_RETRACT_ARM"));
      m_pServo = &armServo;
      m_seqEntry.data1 = armRetractedAngle;
      break;

    case ACTION_MOVE_LID:
      DebugPrintln(F("ACTION_MOVE_LID"));
      moveServoInit(&lidServo, m_seqEntry.data1, m_seqEntry.data2, m_seqEntry.data3);
      break;

    case ACTION_PEEK_LID_FROM_CLOSE:
      DebugPrintln(F("ACTION_PEEK_LID_FROM_CLOSE"));
      // In this case data1 is the "peek" degrees.  It is the number of degrees to adjust the 
      // fully closed position to deduce the "peek" angle.
      if (lidClosedAngle > lidOpenedAngle)
      {
        peekAngle = lidClosedAngle - m_seqEntry.data1;
      }
      else
      {
        peekAngle = lidClosedAngle + m_seqEntry.data1;
      }
      
      moveServoInit(&lidServo, lidClosedAngle, peekAngle, m_seqEntry.data2);
      break;

    case ACTION_CLOSE_LID_FROM_PEEK:
      DebugPrintln(F("ACTION_PEEK_LID_FROM_CLOSE"));
      // In this case data1 is the "peek" degrees.  It is the number of degrees to adjust the 
      // fully open position to deduce the "peek" angle.
      if (lidClosedAngle > lidOpenedAngle)
      {
        peekAngle = lidClosedAngle - m_seqEntry.data1;
      }
      else
      {
        peekAngle = lidClosedAngle + m_seqEntry.data1;
      }
      
      moveServoInit(&lidServo, peekAngle, lidClosedAngle, m_seqEntry.data2);
      break;

    case ACTION_OPEN_LID_FROM_CLOSE:
      DebugPrintln(F("ACTION_OPEN_LID_FROM_CLOSE"));
      moveServoInit(&lidServo, lidClosedAngle, lidOpenedAngle, m_seqEntry.data1);
      break;

    case ACTION_CLOSE_LID_FROM_OPEN:
      DebugPrintln(F("ACTION_CLOSE_LID_FROM_OPEN"));
      moveServoInit(&lidServo, lidOpenedAngle, lidClosedAngle, m_seqEntry.data1);
      break;

    case ACTION_MOVE_ARM:
      DebugPrintln(F("ACTION_MOVE_ARM"));
      moveServoInit(&armServo, m_seqEntry.data1, m_seqEntry.data2, m_seqEntry.data3);
      break;

    case ACTION_EXTEND_ARM_FROM_RETRACTED:
      DebugPrintln(F("ACTION_EXTEND_ARM_FROM_RETRACTED"));
      moveServoInit(&armServo, armRetractedAngle, armExtendedAngle, m_seqEntry.data1);
      break;

    case ACTION_ALMOST_EXTEND_ARM_FROM_RETRACTED:
      DebugPrintln(F("ACTION_ALMOST_EXTEND_ARM_FROM_RETRACTED"));
      moveServoInit(&armServo, armRetractedAngle, armAlmostExtendedAngle, m_seqEntry.data1);
      break;

    case ACTION_RETRACT_ARM_FROM_EXTENDED:
      DebugPrintln(F("ACTION_RETRACT_ARM_FROM_EXTENDED"));
      moveServoInit(&armServo, armExtendedAngle, armRetractedAngle, m_seqEntry.data1);
      break;

    // If not a movement then assume this is a generic action  
    default:
      if (m_seqEntry.action == ACTION_DELAY) DebugPrintln(F("ACTION_DELAY"));
      // Call base class if we don't process the action
      sequence::prepareAction();
  }
}

sequence::actionState moveSequence::executeAction()
{
  switch (m_seqEntry.action)
  {
    case ACTION_EXTEND_ARM:
        m_switchOffAttempted = true; // indicate this move should turn the switch off
        // fall through on purpose!
    case ACTION_OPEN_LID:
    case ACTION_CLOSE_LID:
    case ACTION_RETRACT_ARM:
      // These actions are immediate actions and require no further information.  This move
      // is completed.
      m_pServo->write(m_seqEntry.data1);
      return ACTION_COMPLETE;

    case ACTION_EXTEND_ARM_FROM_RETRACTED:
      if (moveServo() == ACTION_COMPLETE)
      {
        m_switchOffAttempted = true;  // indicate this move should turn the switch off
        return ACTION_COMPLETE;
      }
      break;

    case ACTION_PEEK_LID_FROM_CLOSE:
    case ACTION_OPEN_LID_FROM_CLOSE:
    case ACTION_CLOSE_LID_FROM_PEEK:
    case ACTION_CLOSE_LID_FROM_OPEN:
    case ACTION_MOVE_LID:
    case ACTION_MOVE_ARM:
    case ACTION_ALMOST_EXTEND_ARM_FROM_RETRACTED:
    case ACTION_RETRACT_ARM_FROM_EXTENDED:
      // These are all servo moves in 1 degree increments with a delay in between.  The moveServo() function
      // detects the completion of the move.
      return moveServo();
      break;

    // If not a movement then assume this is a generic action  
    default:
      // call base class
      return sequence::executeAction();
      break;
  }

  // If we got to here then the move is still executing.
  return ACTION_EXECUTING;
}

//
// moveServoInit
//
// This is a convenience function to initialize values in the current move information structure.
//  

void moveSequence::moveServoInit(Servo* pServo, int startAngle, int endAngle, int degDelay)
{
  m_pServo = pServo;              // Servo we are moving
  
  // These values originally came from the current sequence entry (action).  We are re-using them
  // in order to save RAM since the base class declares these for each object.  Sacrificing a little
  // readability to save memory.
  m_seqEntry.data1 = startAngle;  // degree counter. init'ed to the starting angle
  m_seqEntry.data2 = endAngle;
  m_seqEntry.data3 = degDelay;
  m_prevMillis = millis();  // used for timing delay
}

//
// moveServo
//
// This function is responsible for moving the servo 1 degree, checking for move completion, and timing of 
// the delay between 1 degree moves.  The move is complete once the end angle is reached and the final delay
// has been completed.
//  

sequence::actionState moveSequence::moveServo()
{
  // If there is no delay or the delay has expired then move the servo 1 degree and check for move completion.
  if (m_seqEntry.data3 == 0 || millis() - m_prevMillis > m_seqEntry.data3)
  {
    // Prepare for next delay
    m_prevMillis = millis();

    // Increment or decrement the angle based on the direction we are moving
    if (m_seqEntry.data1 < m_seqEntry.data2)
    {
      m_seqEntry.data1++; 
    }
    else if (m_seqEntry.data1 > m_seqEntry.data2)
    {
      m_seqEntry.data1--; 
    }
    else // angles are equal, therefore move is done!
    {
      // Current angle and end angle are equal
      return ACTION_COMPLETE;
    }
    
    // Move it!!!
    m_pServo->write(m_seqEntry.data1);
  }

  // If we got to here the move is still executing.
  return ACTION_EXECUTING;
}