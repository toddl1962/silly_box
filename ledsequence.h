/////////////////////////////////////////////////////////////////////////////////////////////
//
// ledSequence class provides processing specific to the 2 RGB LEDs.  It is derived
// from the sequence base class.
//
/////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "sequence.h"
#include "movesequence.h"

//
// This sequence class handles the lighting patterns of the LEDs
//

class ledSequence: public sequence
{
  public:
    static const int LEFT = 0;
    static const int RIGHT = 1;
    static const int ALL_LEDS = 0xFFF;

  private:
    static const int ledPins[2][3];
    static const int numLeds = sizeof(ledPins)/sizeof(ledPins[0]);

  // Construction/Destruction
  public:
    ledSequence(const void*, seqType = PRIMARY_SEQ, seqEnd = ONE_SHOT);

  // Methods
  public:
    static void setup();
    void setLed(int, const uint32_t);
    actionState transitionLed();
    void startSequence();
    void stopSequence();

  private:
    void prepareAction();
    actionState executeAction();

  // Attributes
  private:  
    // Additional context needed for processing LED actions
    uint32_t m_lastColor[numLeds];
};

