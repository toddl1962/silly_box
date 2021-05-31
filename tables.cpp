/////////////////////////////////////////////////////////////////////////////////////////////
//
// tables.cpp
//
// This file defines all of the tables needed to execute the movement, LED, and sound 
// sequences and groups.
//
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
//  
//  HOW THESE TABLES WORK
//  ---------------------
//
//  Definitions:
//
//    "Action" - an individual function performed (e.g. servo movement, LED manipulation, 
//    sound pitch/length, delay, etc) within the context of a "sequence".
//
//    "Sequence" - a set of actions performed serially until the end of the set is reached.
//
//    "Group" - set of sequences to be performed simultaneously.
//
//  Action specification:
//
//    The set of possible actions is defined in the "action.h" header file.  In this file you
//    will also find documented the data required for each action (if necessary).  There may 
//    be up to 3 32 bit data fields associated with each action.  Note that the structure
//    'sequence::seqEntry' is the data structure that fully defines an action
//
//  Sequence Specification:
//
//    1) Sequence tables MUST be defined with the keyword 'const' and in PROGMEM 
//       (program memory).  This is what allows us to fit this application within
//       UNO and Nano memory constraints.
//
//    2) All sequence tables must end with the enum value 'ACTION_END' to indicate the end 
//       of the sequence tables.
//
//    3) You cannot mix movement, LED, and sound actions within the same sequence table.
//
//    4) 'ACTION_DELAY' enum can be used in any sequence (movement, LED, or sound).  This
//       and 'ACTION_END' are the only generic action enums shared.
//
//    5) When instantiating a sequence object you MUST pass the address of the sequence table.
//       Optional arguments are seqType (2nd argument) and seqEnd (3rd) argument.
//
//         seqType can be:
//            PRIMARY_SEQ - essentially, when this sequence ends the group ends.  Usually a 
//              movement sequence in this application, however for a proximity alert this
//              could be another type of sequence if movement is not desired.
//            SECONDARY_SEQ - this sequence ends when the group ends no matter what action 
//              is being executed (LED and sound sequences in this appkication)
//
//         seqEnd can be:
//            ONE_SHOT - sequence executes one time, regardless of whether other sequences in
//              the group are still executing or not.
//            REPEATING - only applies to secondary sequence.  Sequence repeats until primary
//              sequence completes
//       
//       Default for seqType is PRIMARY_SEQ and default for seqEnd is ONE_SHOT.
//               
//    6) DO NOT instantiate the 'sequence' class!  Only derived sequence classes should be
//       instantiated.  For this application 'movesequence', 'ledsequence', or 'soundsequence'.
//
//    7) DO NOT pass a sequence table to objects that don't process those action types!  For
//       example, instantiating a 'movesequence' object with a sequence table that contains 
//       LED actions.
//
//    8) If the 'movesequence' object is part of a switch group then you will want to make sure
//       you have an action that turns the front switch off!
//    
//  Group Specification:
//
//    1) Group tables MUST be defined with the keyword 'const' and in PROGMEM 
//       (program memory).  This is what allows us to fit this application within
//       UNO and Nano memory constraints.
//
//    2) The last entry in a group table must be NULL.
//
//    3) There can only be one (1) primary sequence (PRIMARY_SEQ) object in a group table.
//       There can be multiple secondary sequence (SECONDARY_SEQ) objects in a group table.
//
//    4) The 'loop()' function requires a table of group objects called 'switchGroupTable' in
//       order to randomly choose a group to execute when the front switch is turned on.
//
//    5) The 'loop()' function requires a table of group objects called 'proxGroupTable' in
//       order to randomly choose a group to execute when the 'proximity' object issues a 
//       proximity alert.
//
//  Constraints:
//
//    1) Sequence and group tables MUST be defined with the keyword 'const' and in PROGMEM 
//       (program memory).  This is what allows us to fit this application within
//       UNO and Nano memory constraints.
//
//    2) You cannot mix movement, LED, and sound actions within the same sequence table.
//       There are a couple of reasons for this in this application:
//       - You risk two or more sequences in the same group trying to access hardware 
//         simultaneoulsly. Arduino does not have an OS so it would be difficult to serialize 
//         these accesses and I'm not sure why you would want that anyway.
//       - If one class handles all sequence types then instantiating that class for sequences
//         that only affect one hardware type will consume memory with baggage not needed.
//
//    3) "Sanity" checking is minimal.  This is not a manned rocket and memory is at a 
//       premium.
//
/////////////////////////////////////////////////////////////////////////////////////////////

#include "color.h"
#include "sequence.h"
#include "movesequence.h"
#include "ledsequence.h"
#include "soundsequence.h"
#include "group.h"

///////////////////////////////////////////////////////////////////////////////
// M o v e   S e q u e n c e s 
///////////////////////////////////////////////////////////////////////////////

const sequence::seqEntry moveTable1[] PROGMEM = 
{
  {ACTION_OPEN_LID_FROM_CLOSE, 20},
  {ACTION_ALMOST_EXTEND_ARM_FROM_RETRACTED, 20},
  {ACTION_DELAY, 1000},
  {ACTION_EXTEND_ARM},
  {ACTION_DELAY, 200},
  {ACTION_RETRACT_ARM_FROM_EXTENDED, 20},
  {ACTION_DELAY, 200},
  {ACTION_CLOSE_LID_FROM_OPEN, 20},
  {ACTION_END}
};

const sequence::seqEntry moveTable2[] PROGMEM = 
{
  {ACTION_OPEN_LID_FROM_CLOSE, 6},
  {ACTION_DELAY, 550},
  {ACTION_EXTEND_ARM},
  {ACTION_DELAY, 1000},
  {ACTION_RETRACT_ARM},
  {ACTION_DELAY, 550},
  {ACTION_CLOSE_LID},
  {ACTION_DELAY, 1500},
  {ACTION_OPEN_LID_FROM_CLOSE, 6},
  {ACTION_DELAY, 3000},
  {ACTION_CLOSE_LID},
  {ACTION_END}
};

const sequence::seqEntry moveTable3[] PROGMEM = 
{
  {ACTION_OPEN_LID_FROM_CLOSE, 6},
  {ACTION_DELAY, 1000},
  {ACTION_CLOSE_LID},
  {ACTION_DELAY, 2000},
  {ACTION_OPEN_LID_FROM_CLOSE, 6},
  {ACTION_EXTEND_ARM},
  {ACTION_DELAY, 650},
  {ACTION_RETRACT_ARM},
  {ACTION_DELAY, 500},
  {ACTION_CLOSE_LID},
  {ACTION_END}
};

const sequence::seqEntry moveTable4[] PROGMEM = 
{
  {ACTION_OPEN_LID_FROM_CLOSE, 20},
  {ACTION_ALMOST_EXTEND_ARM_FROM_RETRACTED, 20},
  {ACTION_DELAY, 2000},
  {ACTION_EXTEND_ARM},
  {ACTION_DELAY, 200},
  {ACTION_RETRACT_ARM},
  {ACTION_DELAY, 500},
  {ACTION_CLOSE_LID},
  {ACTION_END}
};

const sequence::seqEntry moveTable5[] PROGMEM = 
{
  {ACTION_OPEN_LID_FROM_CLOSE, 6},
  {ACTION_DELAY, 2000},
  {ACTION_MOVE_ARM, moveSequence::armRetractedAngle, 65, 0},
  {ACTION_DELAY, 550},
  {ACTION_MOVE_ARM, 65, 40, 0},
  {ACTION_DELAY, 200},
  {ACTION_MOVE_ARM, 40, 65, 0},
  {ACTION_DELAY, 200},
  {ACTION_MOVE_ARM, 65, 40, 0},
  {ACTION_DELAY, 200},
  {ACTION_MOVE_ARM, 40, 65, 0},
  {ACTION_DELAY, 200},
  {ACTION_MOVE_ARM, 65, 40, 0},
  {ACTION_DELAY, 200},
  {ACTION_MOVE_ARM, 40, 65, 0},
  {ACTION_DELAY, 200},
  {ACTION_MOVE_ARM, 65, 40, 0},
  {ACTION_DELAY, 200},
  {ACTION_MOVE_ARM, 40, 65, 0},
  {ACTION_DELAY, 200},
  {ACTION_MOVE_ARM, 65, 40, 0},
  {ACTION_DELAY, 200},
  {ACTION_MOVE_ARM, 40, 65, 0},
  {ACTION_DELAY, 200},
  {ACTION_EXTEND_ARM},
  {ACTION_DELAY, 400},
  {ACTION_RETRACT_ARM},
  {ACTION_DELAY, 500},
  {ACTION_CLOSE_LID},
  {ACTION_END}
};

const sequence::seqEntry moveTable6[] PROGMEM = 
{
  {ACTION_OPEN_LID_FROM_CLOSE, 20},
  {ACTION_DELAY, 1000},
  {ACTION_CLOSE_LID},
  {ACTION_OPEN_LID_FROM_CLOSE, 6},
  {ACTION_DELAY, 200},
  {ACTION_CLOSE_LID},
  {ACTION_OPEN_LID_FROM_CLOSE, 6},
  {ACTION_DELAY, 200},
  {ACTION_CLOSE_LID},
  {ACTION_OPEN_LID_FROM_CLOSE, 6},
  {ACTION_DELAY, 200},
  {ACTION_CLOSE_LID},
  {ACTION_OPEN_LID_FROM_CLOSE, 6},
  {ACTION_DELAY, 200},
  {ACTION_CLOSE_LID},
  {ACTION_OPEN_LID_FROM_CLOSE, 6},
  {ACTION_DELAY, 200},
  {ACTION_CLOSE_LID},
  {ACTION_OPEN_LID_FROM_CLOSE, 6},
  {ACTION_DELAY, 200},
  {ACTION_ALMOST_EXTEND_ARM_FROM_RETRACTED, 30},
  {ACTION_EXTEND_ARM},
  {ACTION_DELAY, 400},
  {ACTION_RETRACT_ARM},
  {ACTION_DELAY, 500},
  {ACTION_CLOSE_LID},
  {ACTION_DELAY, 1500},
  {ACTION_OPEN_LID_FROM_CLOSE, 20},
  {ACTION_DELAY, 3000},
  {ACTION_CLOSE_LID},
  {ACTION_END}
};

const sequence::seqEntry moveTable7[] PROGMEM = 
{
  {ACTION_OPEN_LID_FROM_CLOSE, 6},
  {ACTION_DELAY, 400},
  {ACTION_EXTEND_ARM},
  {ACTION_DELAY, 1000},
  {ACTION_CLOSE_LID},
  {ACTION_OPEN_LID_FROM_CLOSE, 6},
  {ACTION_DELAY, 200},
  {ACTION_CLOSE_LID},
  {ACTION_OPEN_LID_FROM_CLOSE, 6},
  {ACTION_DELAY, 200},
  {ACTION_CLOSE_LID},
  {ACTION_OPEN_LID_FROM_CLOSE, 6},
  {ACTION_DELAY, 200},
  {ACTION_CLOSE_LID},
  {ACTION_OPEN_LID_FROM_CLOSE, 6},
  {ACTION_DELAY, 200},
  {ACTION_CLOSE_LID},
  {ACTION_OPEN_LID_FROM_CLOSE, 6},
  {ACTION_DELAY, 200},
  {ACTION_CLOSE_LID},
  {ACTION_OPEN_LID_FROM_CLOSE, 6},
  {ACTION_DELAY, 700},
  {ACTION_RETRACT_ARM},
  {ACTION_DELAY, 500},
  {ACTION_CLOSE_LID},
  {ACTION_END}
};

const sequence::seqEntry moveTable8[] PROGMEM = 
{
  {ACTION_OPEN_LID_FROM_CLOSE, 6},
  {ACTION_EXTEND_ARM},
  {ACTION_DELAY, 2000},
  {ACTION_MOVE_ARM, moveSequence::armExtendedAngle, 75, 0},
  {ACTION_DELAY, 100},
  {ACTION_MOVE_ARM, 75, 40, 0},
  {ACTION_DELAY, 100},
  {ACTION_MOVE_ARM, 40, 75, 0},
  {ACTION_DELAY, 100},
  {ACTION_MOVE_ARM, 75, 40, 0},
  {ACTION_DELAY, 100},
  {ACTION_MOVE_ARM, 40, 75, 0},
  {ACTION_DELAY, 100},
  {ACTION_MOVE_ARM, 75, 40, 0},
  {ACTION_DELAY, 100},
  {ACTION_MOVE_ARM, 40, 75, 0},
  {ACTION_DELAY, 100},
  {ACTION_MOVE_ARM, 75, 40, 0},
  {ACTION_DELAY, 100},
  {ACTION_MOVE_ARM, 40, 75, 0},
  {ACTION_DELAY, 100},
  {ACTION_MOVE_ARM, 75, 40, 0},
  {ACTION_DELAY, 100},
  {ACTION_MOVE_ARM, 40, 75, 0},
  {ACTION_DELAY, 100},
  {ACTION_MOVE_ARM, 75, 40, 0},
  {ACTION_DELAY, 100},
  {ACTION_MOVE_ARM, 40, 75, 0},
  {ACTION_DELAY, 100},
  {ACTION_MOVE_ARM, 75, 40, 0},
  {ACTION_DELAY, 100},
  {ACTION_MOVE_ARM, 40, 75, 0},
  {ACTION_DELAY, 600},
  {ACTION_RETRACT_ARM},
  {ACTION_DELAY, 1000},
  {ACTION_CLOSE_LID},
  {ACTION_END}
};

const sequence::seqEntry moveTable9[] PROGMEM = 
{
  {ACTION_PEEK_LID_FROM_CLOSE, 5, 6},
  {ACTION_DELAY, 2000},
  {ACTION_OPEN_LID},
  {ACTION_ALMOST_EXTEND_ARM_FROM_RETRACTED, 40},
  {ACTION_DELAY, 500},
  {ACTION_EXTEND_ARM},
  {ACTION_DELAY, 200},
  {ACTION_RETRACT_ARM_FROM_EXTENDED, 40},
  {ACTION_CLOSE_LID},
  {ACTION_DELAY, 2000},
  {ACTION_PEEK_LID_FROM_CLOSE, 10, 0},
  {ACTION_DELAY, 3000},
  {ACTION_CLOSE_LID},
  {ACTION_END}
};

const sequence::seqEntry moveTable10[] PROGMEM = 
{
  {ACTION_PEEK_LID_FROM_CLOSE, 5, 100},
  {ACTION_DELAY, 2000},
  {ACTION_CLOSE_LID},
  {ACTION_PEEK_LID_FROM_CLOSE, 10, 6},
  {ACTION_DELAY, 100},
  {ACTION_CLOSE_LID},
  {ACTION_PEEK_LID_FROM_CLOSE, 10, 6},
  {ACTION_DELAY, 100},
  {ACTION_CLOSE_LID},
  {ACTION_PEEK_LID_FROM_CLOSE, 10, 6},
  {ACTION_DELAY, 100},
  {ACTION_CLOSE_LID},
  {ACTION_PEEK_LID_FROM_CLOSE, 10, 6},
  {ACTION_DELAY, 100},
  {ACTION_CLOSE_LID},
  {ACTION_PEEK_LID_FROM_CLOSE, 10, 6},
  {ACTION_DELAY, 100},
  {ACTION_CLOSE_LID},
  {ACTION_PEEK_LID_FROM_CLOSE, 10, 6},
  {ACTION_DELAY, 100},
  {ACTION_CLOSE_LID},
  {ACTION_PEEK_LID_FROM_CLOSE, 10, 6},
  {ACTION_DELAY, 100},
  {ACTION_CLOSE_LID},
  {ACTION_PEEK_LID_FROM_CLOSE, 10, 6},
  {ACTION_DELAY, 100},
  {ACTION_CLOSE_LID},
  {ACTION_PEEK_LID_FROM_CLOSE, 10, 6},
  {ACTION_DELAY, 100},
  {ACTION_CLOSE_LID},
  {ACTION_PEEK_LID_FROM_CLOSE, 10, 6},
  {ACTION_DELAY, 100},
  {ACTION_OPEN_LID},
  {ACTION_DELAY, 100},
  {ACTION_EXTEND_ARM},
  {ACTION_DELAY, 650},
  {ACTION_RETRACT_ARM},
  {ACTION_DELAY, 500},
  {ACTION_CLOSE_LID},
  {ACTION_END}
};

const sequence::seqEntry moveTable11[] PROGMEM = 
{
  {ACTION_OPEN_LID_FROM_CLOSE, 6},
  {ACTION_EXTEND_ARM},
  {ACTION_DELAY, 2000},
  {ACTION_MOVE_ARM, moveSequence::armExtendedAngle, 65, 0},
  {ACTION_DELAY, 200},
  {ACTION_MOVE_ARM, 65, 40, 0},
  {ACTION_DELAY, 200},
  {ACTION_MOVE_ARM, 40, 65, 0},
  {ACTION_DELAY, 200},
  {ACTION_MOVE_ARM, 65, 40, 0},
  {ACTION_DELAY, 200},
  {ACTION_MOVE_ARM, 40, 65, 0},
  {ACTION_DELAY, 200},
  {ACTION_MOVE_ARM, 65, 40, 0},
  {ACTION_DELAY, 1700},
  {ACTION_RETRACT_ARM},
  {ACTION_DELAY, 400},
  {ACTION_CLOSE_LID},
  {ACTION_DELAY, 1500},
  {ACTION_OPEN_LID_FROM_CLOSE, 6},
  {ACTION_EXTEND_ARM},
  {ACTION_DELAY, 3000},
  {ACTION_RETRACT_ARM},
  {ACTION_DELAY, 500},
  {ACTION_CLOSE_LID},
  {ACTION_END}
};

const sequence::seqEntry moveTable12[] PROGMEM = 
{
  {ACTION_OPEN_LID_FROM_CLOSE, 50},
  {ACTION_DELAY, 1000},
  {ACTION_EXTEND_ARM_FROM_RETRACTED, 50},
  {ACTION_DELAY, 2000},
  {ACTION_RETRACT_ARM},
  {ACTION_DELAY, 500},
  {ACTION_CLOSE_LID},
  {ACTION_END}
};

const sequence::seqEntry moveTable13[] PROGMEM = 
{
  {ACTION_OPEN_LID_FROM_CLOSE, 6},
  {ACTION_DELAY, 2000},
  {ACTION_MOVE_ARM, moveSequence::armRetractedAngle, 60, 40},
  {ACTION_MOVE_ARM, 60, 40, 0},
  {ACTION_DELAY, 800},
  {ACTION_MOVE_ARM, 40, 60, 0},
  {ACTION_DELAY, 200},
  {ACTION_MOVE_ARM, 60, 40, 0},
  {ACTION_DELAY, 800},
  {ACTION_MOVE_ARM, 40, 60, 0},
  {ACTION_DELAY, 200},
  {ACTION_MOVE_ARM, 60, 40, 0},
  {ACTION_DELAY, 800},
  {ACTION_EXTEND_ARM},
  {ACTION_DELAY, 2000},
  {ACTION_RETRACT_ARM},
  {ACTION_DELAY, 500},
  {ACTION_CLOSE_LID},
  {ACTION_END}
};
    
const sequence::seqEntry moveTable14[] PROGMEM = 
{
  {ACTION_PEEK_LID_FROM_CLOSE, 5, 6},
  {ACTION_DELAY, 4000},
  {ACTION_OPEN_LID},
  {ACTION_EXTEND_ARM},
  {ACTION_DELAY, 650},
  {ACTION_RETRACT_ARM},
  {ACTION_DELAY, 500},
  {ACTION_MOVE_LID, moveSequence::lidOpenedAngle, moveSequence::lidOpenedAngle - 10, 0},
  {ACTION_DELAY, 4000},
  {ACTION_CLOSE_LID},
  {ACTION_END}
};
   
const sequence::seqEntry moveTable15[] PROGMEM = 
{
  {ACTION_PEEK_LID_FROM_CLOSE, 5, 6},
  {ACTION_DELAY, 1000},
  {ACTION_PEEK_LID_FROM_CLOSE, 5, 50},
  {ACTION_CLOSE_LID_FROM_PEEK, 5, 50},
  {ACTION_PEEK_LID_FROM_CLOSE, 5, 50},
  {ACTION_CLOSE_LID_FROM_PEEK, 5, 50},
  {ACTION_PEEK_LID_FROM_CLOSE, 5, 50},
  {ACTION_CLOSE_LID_FROM_PEEK, 5, 50},
  {ACTION_OPEN_LID_FROM_CLOSE, 50},
  {ACTION_ALMOST_EXTEND_ARM_FROM_RETRACTED, 40},
  {ACTION_DELAY, 1000},
  {ACTION_EXTEND_ARM},
  {ACTION_DELAY, 400},
  {ACTION_RETRACT_ARM},
  {ACTION_DELAY, 500},
  {ACTION_CLOSE_LID},
  {ACTION_END}
};

moveSequence moveSequence1(moveTable1);
moveSequence moveSequence2(moveTable2);
moveSequence moveSequence3(moveTable3);
moveSequence moveSequence4(moveTable4);
moveSequence moveSequence5(moveTable5);
moveSequence moveSequence6(moveTable6);
moveSequence moveSequence7(moveTable7);
moveSequence moveSequence8(moveTable8);
moveSequence moveSequence9(moveTable9);
moveSequence moveSequence10(moveTable10);
moveSequence moveSequence11(moveTable11);
moveSequence moveSequence12(moveTable12);
moveSequence moveSequence13(moveTable13);
moveSequence moveSequence14(moveTable14);
moveSequence moveSequence15(moveTable15);

const sequence::seqEntry proxMoveTable1[] PROGMEM = 
{
  {ACTION_PEEK_LID_FROM_CLOSE, 10, 20},
  {ACTION_DELAY, 1000},
  {ACTION_CLOSE_LID},
  {ACTION_END}
};

const sequence::seqEntry proxMoveTable2[] PROGMEM = 
{
  {ACTION_OPEN_LID},
  {ACTION_DELAY, 1000},
  {ACTION_CLOSE_LID},
  {ACTION_END}
};

moveSequence proxMoveSequence1(proxMoveTable1);
moveSequence proxMoveSequence2(proxMoveTable2);


///////////////////////////////////////////////////////////////////////////////
// L E D   S e q u e n c e s 
///////////////////////////////////////////////////////////////////////////////

const sequence::seqEntry ledFastRotation[] PROGMEM = 
{
  {ACTION_SET_LED, ledSequence::ALL_LEDS, clRed},
  {ACTION_DELAY, 200},
  {ACTION_SET_LED, ledSequence::ALL_LEDS, clGreen},
  {ACTION_DELAY, 200},
  {ACTION_SET_LED, ledSequence::ALL_LEDS, clBlue},
  {ACTION_DELAY, 200},
  {ACTION_SET_LED, ledSequence::ALL_LEDS, clYellow},
  {ACTION_DELAY, 200},
  {ACTION_END}
};
  
// Slow color rotation
const sequence::seqEntry ledSlowRotation[] PROGMEM = 
{
  {ACTION_SET_LED, ledSequence::ALL_LEDS, clRed},
  {ACTION_DELAY, 500},
  {ACTION_SET_LED, ledSequence::ALL_LEDS,  clGreen},
  {ACTION_DELAY, 500},
  {ACTION_SET_LED, ledSequence::ALL_LEDS, clBlue},
  {ACTION_DELAY, 500},
  {ACTION_SET_LED, ledSequence::ALL_LEDS,  clYellow},
  {ACTION_DELAY, 500},
  {ACTION_END}
};

// Fast red blink
const sequence::seqEntry ledFastRedBlink[] PROGMEM = 
{
  {ACTION_SET_LED, ledSequence::ALL_LEDS, clRed},
  {ACTION_DELAY, 200},
  {ACTION_SET_LED, ledSequence::ALL_LEDS,  clBlack},
  {ACTION_DELAY, 200},
  {ACTION_END}
};

// Fast red/yellow blink
const sequence::seqEntry ledFastRedYellowBlink[] PROGMEM = 
{
  {ACTION_SET_LED, ledSequence::ALL_LEDS, clRed},
  {ACTION_DELAY, 200},
  {ACTION_SET_LED, ledSequence::ALL_LEDS,  clYellow},
  {ACTION_DELAY, 200},
  {ACTION_END}
};

// Fast blue/yellow blink
const sequence::seqEntry ledFastBlueYellowBlink[] PROGMEM = 
{
  {ACTION_SET_LED, ledSequence::ALL_LEDS, clBlue},
  {ACTION_DELAY, 200},
  {ACTION_SET_LED, ledSequence::ALL_LEDS,  clYellow},
  {ACTION_DELAY, 200},
  {ACTION_END}
};

// Solid Red
const sequence::seqEntry ledSolidRed[] PROGMEM = 
{
  {ACTION_SET_LED, ledSequence::ALL_LEDS, clRed},
  {ACTION_DELAY, 200},
  {ACTION_END}
};

// Solid Green
const sequence::seqEntry ledSolidGreen[] PROGMEM = 
{
  {ACTION_SET_LED, ledSequence::ALL_LEDS, clGreen},
  {ACTION_DELAY, 200},
  {ACTION_END}
};

// Solid Blue
const sequence::seqEntry ledSolidBlue[] PROGMEM = 
{
  {ACTION_SET_LED, ledSequence::ALL_LEDS, clBlue},
  {ACTION_DELAY, 200},
  {ACTION_END}
};

// Solid Yellow
const sequence::seqEntry ledSolidYellow[] PROGMEM = 
{
  {ACTION_SET_LED, ledSequence::ALL_LEDS, clYellow},
  {ACTION_DELAY, 200},
  {ACTION_END}
};

ledSequence ledFastRotationSequence(ledFastRotation, sequence::SECONDARY_SEQ, sequence::REPEATING); 
ledSequence ledSlowRotationSequence(ledSlowRotation, sequence::SECONDARY_SEQ, sequence::REPEATING); 
ledSequence ledFastRedBlinkSequence(ledFastRedBlink, sequence::SECONDARY_SEQ, sequence::REPEATING); 
ledSequence ledFastRedYellowBlinkSequence(ledFastRedYellowBlink, sequence::SECONDARY_SEQ, sequence::REPEATING);
ledSequence ledFastBlueYellowBlinkSequence(ledFastBlueYellowBlink, sequence::SECONDARY_SEQ, sequence::REPEATING);
ledSequence ledSolidRedSequence(ledSolidRed, sequence::SECONDARY_SEQ, sequence::REPEATING);
ledSequence ledSolidGreenSequence(ledSolidGreen, sequence::SECONDARY_SEQ, sequence::REPEATING);
ledSequence ledSolidBlueSequence(ledSolidBlue, sequence::SECONDARY_SEQ, sequence::REPEATING);
ledSequence ledSolidYellowSequence(ledSolidYellow, sequence::SECONDARY_SEQ, sequence::REPEATING);


///////////////////////////////////////////////////////////////////////////////
// S o u n d   S e q u e n c e s 
///////////////////////////////////////////////////////////////////////////////

const sequence::seqEntry soundStarsStripesTbl[] PROGMEM = 
{
  {TEMPO, TEMPO_ALLEGRO}, 
  {ARTICULATE, ARTICULATE_STACCATO},
  {PITCH_EF4, NOTE_QTR},
  {PITCH_D4, NOTE_DOT_8TH},
  {PITCH_EF4, NOTE_16TH},
  {PITCH_C4, NOTE_8TH},
  {PITCH_EF4, NOTE_QTR},
  {PITCH_F4, NOTE_8TH},
  {PITCH_GF4, NOTE_8TH},
  {PITCH_G4, NOTE_8TH},
  {PITCH_AF4, NOTE_8TH},
  {PITCH_A4, NOTE_8TH},
  {PITCH_BF4, NOTE_8TH},
  {ACTION_END}
};

const sequence::seqEntry soundChargeTbl[] PROGMEM = 
{
  {TEMPO, TEMPO_ALLEGRO}, 
  {ARTICULATE, ARTICULATE_STACCATO},
  {PITCH_G3, NOTE_8TH},
  {PITCH_C4, NOTE_8TH},
  {PITCH_E4, NOTE_8TH},
  {PITCH_G4, NOTE_DOT_8TH},
  {PITCH_E4, NOTE_16TH},
  {PITCH_G4, NOTE_QTR},
  {ACTION_END}
};

const sequence::seqEntry soundBackUpTbl[] PROGMEM = 
{
  {TEMPO, TEMPO_ALLEGRO}, 
  {ARTICULATE, ARTICULATE_STACCATO},
  {PITCH_B5, NOTE_QTR},
  {PITCH_REST, NOTE_QTR},
  {PITCH_B5, NOTE_QTR},
  {PITCH_REST, NOTE_QTR},
  {PITCH_B5, NOTE_QTR},
  {PITCH_REST, NOTE_QTR},
  {PITCH_B5, NOTE_QTR},
  {PITCH_REST, NOTE_HALF},
  {ACTION_END}
};

const sequence::seqEntry soundAnnoyedTbl[] PROGMEM = 
{
  {TEMPO, TEMPO_ALLEGRO}, 
  {ARTICULATE, ARTICULATE_STACCATO},
  {PITCH_C2, NOTE_HALF},
  {ACTION_END}
};

const sequence::seqEntry soundFussyTbl[] PROGMEM = 
{
  {TEMPO, TEMPO_ALLEGRO}, 
  {ARTICULATE, ARTICULATE_STACCATO},
  {PITCH_B3, NOTE_8TH},
  {PITCH_REST, NOTE_QTR},
  {PITCH_B3, NOTE_16TH},
  {PITCH_G3, NOTE_16TH},
  {PITCH_REST, NOTE_QTR},
  {PITCH_F3, NOTE_DOT_8TH},
  {PITCH_A3, NOTE_8TH},
  {PITCH_BF3, NOTE_8TH},
  {PITCH_BF3, NOTE_8TH},
  {ACTION_END}
};

soundSequence soundFussy(soundFussyTbl, sequence::SECONDARY_SEQ, sequence::ONE_SHOT);
soundSequence soundAnnoyed(soundAnnoyedTbl, sequence::SECONDARY_SEQ, sequence::ONE_SHOT);
soundSequence soundBackUp(soundBackUpTbl, sequence::SECONDARY_SEQ, sequence::ONE_SHOT);
soundSequence soundStarsStripes(soundStarsStripesTbl, sequence::SECONDARY_SEQ, sequence::ONE_SHOT);
soundSequence soundCharge(soundChargeTbl, sequence::SECONDARY_SEQ, sequence::ONE_SHOT);

///////////////////////////////////////////////////////////////////////////////
// S w i t c h   G r o u p s 
///////////////////////////////////////////////////////////////////////////////

sequence* const group1[] PROGMEM =  
{
  &moveSequence1,  
  &ledFastRotationSequence,        
  &soundStarsStripes, 
  NULL
};

sequence* const group2[] PROGMEM =  
{
  &moveSequence2,  
  &ledSlowRotationSequence,        
  &soundCharge, 
  NULL
};

sequence* const group3[] PROGMEM =  
{
  &moveSequence3,  
  &ledFastRedBlinkSequence,
  &soundStarsStripes, 
  NULL
};

sequence* const group4[] PROGMEM =  
{
  &moveSequence4,  
  &ledFastRedYellowBlinkSequence,  
  &soundCharge, 
  NULL
};

sequence* const group5[] PROGMEM =  
{
  &moveSequence5,  
  &ledFastBlueYellowBlinkSequence, 
  &soundStarsStripes, 
  NULL
};

sequence* const group6[] PROGMEM =  
{
  &moveSequence6,  
  &ledSolidGreenSequence,
  &soundBackUp, 
  NULL
};

sequence* const group7[] PROGMEM =  
{
  &moveSequence7,  
  &ledSolidBlueSequence,
  &soundFussy, 
  NULL
};

sequence* const group8[] PROGMEM =  
{
  &moveSequence8,  
  &ledSolidYellowSequence,
  &soundBackUp, 
  NULL
};

sequence* const group9[] PROGMEM =  
{
  &moveSequence9,  
  &ledFastRotationSequence,
  &soundFussy,
  NULL
};

sequence* const group10[] PROGMEM = 
{
  &moveSequence10, 
  &ledSlowRotationSequence,
  &soundBackUp, 
  NULL
};

sequence* const group11[] PROGMEM = 
{
  &moveSequence11, 
  &ledFastRedBlinkSequence,
  &soundAnnoyed, 
  NULL
};

sequence* const group12[] PROGMEM = 
{
  &moveSequence12, 
  &ledFastRedYellowBlinkSequence,  
  &soundAnnoyed, 
  NULL
};

sequence* const group13[] PROGMEM = 
{
  &moveSequence13, 
  &ledFastBlueYellowBlinkSequence, 
  &soundStarsStripes, 
  NULL
};

sequence* const group14[] PROGMEM = 
{
  &moveSequence14, 
  &ledSolidGreenSequence,
  &soundBackUp, 
  NULL
};

sequence* const group15[] PROGMEM = 
{
  &moveSequence15, 
  &ledSolidBlueSequence,
  &soundCharge, 
  NULL
};


group switchGroupTable[] =
{
  group(group1),
  group(group2),
  group(group3),
  group(group4),
  group(group5),
  group(group6),
  group(group7),
  group(group8),
  group(group9),
  group(group10),
  group(group11),
  group(group12),
  group(group13),
  group(group14),
  group(group15)
};

int numSwitchGroups = sizeof(switchGroupTable)/sizeof(switchGroupTable[0]);

///////////////////////////////////////////////////////////////////////////////
// P r o x i m i t y   G r o u p s 
///////////////////////////////////////////////////////////////////////////////

sequence* const PROGMEM proxGroup1[] = 
{
  &proxMoveSequence1, 
  &ledFastRedBlinkSequence, 
  &soundAnnoyed, 
  NULL
};

sequence* const PROGMEM proxGroup2[] = 
{
  &proxMoveSequence2, 
  &ledSolidBlueSequence, 
  &soundAnnoyed, 
  NULL
};

group proxGroupTable[] =
{
  group(proxGroup1),
  group(proxGroup2)
};

int numProxGroups = sizeof(proxGroupTable)/sizeof(proxGroupTable[0]);

///////////////////////////////////////////////////////////////////////////////
// T e s t   M o d e
///////////////////////////////////////////////////////////////////////////////

//
// The following sequence is for the test mode 
//
const sequence::seqEntry testModeTable[] PROGMEM = 
{
  {ACTION_OPEN_LID_FROM_CLOSE, 20},
  {ACTION_DELAY, 2000},
  {ACTION_ALMOST_EXTEND_ARM_FROM_RETRACTED, 20},
  {ACTION_DELAY, 2000},
  {ACTION_EXTEND_ARM},
  {ACTION_DELAY, 200},
  {ACTION_RETRACT_ARM},
  {ACTION_DELAY, 2000},
  {ACTION_CLOSE_LID},
  {ACTION_END}
};

moveSequence testModeSequence(testModeTable);

sequence* const testModeGroup[] PROGMEM =  
{
  &testModeSequence,  
  NULL
};

group testMode(testModeGroup);
