/////////////////////////////////////////////////////////////////////////////////////////////
//
// moveSequence class provides processing of sound.  It is derived from the sequence 
// base class.
//
/////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "sequence.h"

class soundSequence: public sequence
{
  // Construction/Destruction
  public:
    soundSequence(const void*, seqType = PRIMARY_SEQ, seqEnd = ONE_SHOT);

  // Methods
  public:
    static void setup();
    void startSequence();
    void stopSequence();

  private:
    void prepareAction();
    actionState executeAction();

  // Attributes
  private:  
    static const int speakerPin = 7;
    // Additional context needed for processing actions
    uint32_t m_tempoNoteMs;
    float m_articulation;
};
