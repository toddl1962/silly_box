/////////////////////////////////////////////////////////////////////////////////////////////
//
// "Silly Box" Project,  created by Todd Lumpkin
//
// This is a "useless box" project with some added personality!  It is based on
// the Arduino useless box project found at:
//
//    https://create.arduino.cc/projecthub/viorelracoviteanu/useless-box-with-arduino-d67b47
//
// However, I added light, sound, and an ultrasonic sensor for proximity detection.  I also
// created new software in C++ and "table-ized" all of the light, sound, and movement
// sequences in order to make them easily modifiable and executable in parallel.
//
// In it's current state the software still fits in an Arduino Uno or Nano but the concession
// is that most of the tables are placed in PROGMEM.  This results in a little bit of 
// shenanigans when accessing the tables since PROGMEM has to be accessed with special 
// functions.
//
// Tables.cpp file contains all of the tables for the light, sound, and movement
// sequences. Instructions on modifying and creating new sequences are found in the
// tables.cpp and action.h files.
//
// You are free to copy, distribute, and modify this software as long as credit is
// maintained.
//
/////////////////////////////////////////////////////////////////////////////////////////////

// Define preprocessor DEBUG to enable debug output.  Also, be sure to use the F macro when
// printing strings in order to save RAM.

//#define DEBUG

#include "sequence.h"
#include "movesequence.h"
#include "ledsequence.h"
#include "soundsequence.h"
#include "group.h"
#include "debug.h"

// Front switch pin
const int switchPin = 2;

// Front switch action
enum switchActionEnum
{
  NO_CHANGE,
  TRANS_TO_ON,
  TRANS_TO_OFF
};

enum sillyStateEnum
{
  SILLY_IDLE,
  SILLY_START_SWITCH_GROUP,
  SILLY_EXEC_SWITCH_GROUP,
  SILLY_EXEC_PROX_GROUP,
};

// Test Mode pin
const int testModePin = 11;

// Group tables
extern group switchGroupTable[];
extern const int numSwitchGroups;
extern group proxGroupTable[];
extern const int numProxGroups;

// Test mode group
extern group testMode;

/////////////////////////////////////////////////////////////////////////////////////////////
//
// switchActionCheck()
//
// Read the switch and determine if it has transitioned.  We don't really care about the
// 'state' of the switch but only changes in state.  FYI, this is the only place you will 
// find a delay() in this code.  Every other delay is timed using the millis() function.
// The reason it is done here is because it is a quick and dirty way of ignoring switch
// bounce.  Since the switch is transitioning anyway I don't worry with affecting the 
// timing of the sequences since a transition usually means a sequence group is beginning
// or ending.
//
// NOTE: This should be called every time loop() is executed
//
// Why am I returning an 'int' rather than a 'switchActionEnum'?  Good question! I used
// https://www.tinkercad.com/ to debug a lot of this code.  For some unexplained reason
// tinkercad compilation barfed on returning a switchActionEnum whereas the IDE compiler 
// did not.  I left it this way for tinkercad.
//
/////////////////////////////////////////////////////////////////////////////////////////////

int switchActionCheck()
{
  static byte lastSwitchState = digitalRead(switchPin);
  byte currentSwitchState = digitalRead(switchPin);
  switchActionEnum switchAction = NO_CHANGE;

  if (currentSwitchState != lastSwitchState)
  {
    delay(50); // debounce
    lastSwitchState = currentSwitchState;
    if (currentSwitchState == LOW)
    {
      // Switch has transitioned to on
      switchAction = TRANS_TO_ON;
    }
    else // currentSwitchState == HIGH
    {
      // Switch has transitioned to off
      switchAction = TRANS_TO_OFF;
    }
  }
  return switchAction;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//
// testModeLoop()
//
// Servo test mode.  Test mode can only be entered if the testModePin is grounded
// at power up.  This function is only called for test mode and never exits!
//
/////////////////////////////////////////////////////////////////////////////////////////////

void testModeLoop()
{
  bool executingTestGroup = false;

  while (1)
  {
    switchActionEnum switchAction = (switchActionEnum) switchActionCheck();

    if (switchAction == TRANS_TO_ON)
    {
      DebugPrintln(F("Start Test Group"));
      testMode.start();
      executingTestGroup = true;
    }
    
    if (executingTestGroup)
    {
      if (testMode.loop() == group::GROUP_COMPLETE)
      {
        DebugPrintln(F("Test Group Complete"));
        testMode.reset();
        executingTestGroup = false;
      }
    }
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////
//
// setup()
//
// Arduino required setup() function for hardware and software initialization
//
/////////////////////////////////////////////////////////////////////////////////////////////

void setup()
{
  // Serial debug
  DebugInit(115200);
  
  // Switch pin input
  pinMode(switchPin, INPUT_PULLUP);

  // Randon number seeding
  randomSeed(analogRead(A0));
  
  // Static setup of hardware
  proximitySensor::setup(); // Proximity sensor initialization
  moveSequence::setup();    // Movement hardware (servo) initialization
  ledSequence::setup();     // LED hardware initialization

  DebugPrintln(F("Setup Complete"));
  
  // 
  // Test mode can only be entered if the testModePin is grounded at power up.
  //
  pinMode(testModePin, INPUT_PULLUP);
  if (digitalRead(testModePin) == LOW) 
  {
    DebugPrintln(F("********** ENTERING TEST MODE ************"));
    testModeLoop();
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////
//
// loop()
//
// Arduino required loop() function.  Called on a continuous basis by the Arduino system
// software.
//
/////////////////////////////////////////////////////////////////////////////////////////////

//
// A couple of definitions:
//
//   "Proximity (Prox) Group" is a group of sequences executed when the ultrasonic
//   sensor has detected an object (probably a hand) approaching the switch.
//
//   "Switch Group" is a group of sequences executed when the front switch has 
//   been turned ON.
//

void loop()
{
  ///////////////////////////////////////////////////////////////////////////////////////////
  // These static variables are retained between calls to loop() in order to provide context 
  // for the state of the silly box.  Defined here to limit scope.
  ///////////////////////////////////////////////////////////////////////////////////////////
  
  static sillyStateEnum sillyState = SILLY_IDLE; // Silly Box state
  static int switchGroupIndex;  // currently processing switch group
  static int proxGroupIndex;    // currently processing prox group
  
  ///////////////////////////////////////////////////////////////////////////////////////////
  // Process the current state of the silly box
  ///////////////////////////////////////////////////////////////////////////////////////////
  
  // Get front switch action. Must be called every time through loop()!
  switchActionEnum switchAction = (switchActionEnum) switchActionCheck();

  switch (sillyState)
  {
    //    
    // State SILLY_IDLE waits for the human to cause something to happen
    //
    case SILLY_IDLE:
      if (switchAction == TRANS_TO_ON)
      {
        // A human has turned the switch on so execute a switch group
        sillyState = SILLY_START_SWITCH_GROUP;
      }
      else if (proxSensor.proximityAlertCheck())
      {
        // Switch has not transitioned so check to see if a human is
        // approaching the switch and begin the harassment procedure.
        proxGroupIndex = random(numProxGroups);
        DebugPrint(F("Start Prox Group "));
        DebugPrintln(proxGroupIndex);
        proxGroupTable[proxGroupIndex].start();
        sillyState = SILLY_EXEC_PROX_GROUP;
      }
      break;
      
    //    
    // State SILLY_START_SWITCH_GROUP chooses a random switch group and starts it
    //
    case SILLY_START_SWITCH_GROUP:
      DebugPrintln(F("SILLY_START_SWITCH_GROUP"));
      switchGroupIndex = random(numSwitchGroups);
      DebugPrint(F("Start Switch Group "));
      DebugPrintln(switchGroupIndex);
      switchGroupTable[switchGroupIndex].start();
      sillyState = SILLY_EXEC_SWITCH_GROUP;
      break;
      
    //    
    // State SILLY_EXEC_SWITCH_GROUP executes a switch group until either the human turns off
    // the switch or the switch group completes
    //
    case SILLY_EXEC_SWITCH_GROUP:
      // If the switch was turned off and the sequence has not yet attempted to turn off the switch 
      // then a human turned the switch off before the arm servo had a chance to.  If so, stop the 
      // current group.  Otherwise continue executing the group until it is complete
      if (switchAction == TRANS_TO_OFF && !switchGroupTable[switchGroupIndex].getSwitchOffAttempted()
      ||  switchGroupTable[switchGroupIndex].loop() == group::GROUP_COMPLETE)
      {
        DebugPrintln(F("Switch Group Complete"));
        switchGroupTable[switchGroupIndex].reset();
        sillyState = SILLY_IDLE;
      } 
      break;
      
    //    
    // State SILLY_EXEC_PROX_GROUP executes a prox group until either the human turns on
    // the switch or the switch group completes
    //
    case SILLY_EXEC_PROX_GROUP:
      if (switchAction == TRANS_TO_ON)
      {
        // Human was brave enough to mo e the switch anyhow.
        proxGroupTable[proxGroupIndex].reset();
        sillyState = SILLY_START_SWITCH_GROUP;
      }
      else if (proxGroupTable[proxGroupIndex].loop() == group::GROUP_COMPLETE)
      {
        DebugPrintln(F("Prox Group Complete"));
        proxGroupTable[proxGroupIndex].reset();
        sillyState = SILLY_IDLE;
      }
      break;

    default:
      // !!!!!!!!!!!!!!!!!!!!!!  SHOULD NEVER HAPPEN  !!!!!!!!!!!!!!!!!!!!!!
      DebugPrintln(F("ERROR: Invalid sillyState value!!! Resetting to idle state..."));
      sillyState = SILLY_IDLE;
      break;
  }
}

