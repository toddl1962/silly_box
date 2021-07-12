/////////////////////////////////////////////////////////////////////////////////////////////
//
// moveSequence class provides processing specific to the arm and lid servos.  It is derived
// from the sequence base class.
//
// The servos are MG996R Servo Motors.
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

