/////////////////////////////////////////////////////////////////////////////////////////////
//
// moveSequence class provides processing specific to the arm and lid servos.  It is derived
// from the sequence base class.
//
// The servos are MG996R Servo Motors.
//
/////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <Servo.h>
#include "sequence.h"
#include "proximity.h"

//
// This sequence class handles the movement of the arm and lid servos.
//
 
class moveSequence: public sequence
{
  public:
    // Servos
    static Servo armServo;
    static Servo lidServo;

    static const int lidClosedAngle = 170;
    static const int lidOpenedAngle = 130;
    static const int armRetractedAngle = 180;
    static const int armExtendedAngle = 9;
    static const int armAlmostExtendedAngle = armExtendedAngle + 11;

  private:
    // Constants
    static const int armServoPin = 5;
    static const int lidServoPin = 6;

  // Construction/Destruction
  public:
    moveSequence(const void* pSeqTable, seqType aSeqType, seqEnd aSeqEnd);
    moveSequence(const void* pSeqTable);

  // Methods
  public:
    static void setup();
    void startSequence();
    void stopSequence();

  private:
    actionState executeAction();
    void prepareAction();
    void moveServoInit(Servo* pServo, int startAngle, int endAngle, int degDelay);
    actionState moveServo();

  // Attributes
  private:
    // Additional context for processing servo actions
    Servo* m_pServo;
};

