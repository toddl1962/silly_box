/////////////////////////////////////////////////////////////////////////////////////////////
//
// Sequence actions and associated constants
//
/////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Arduino.h"

enum actionType
{
//  Action Enum                                      Data1                      Data2                         Data3   
//  ---------------------------------------------------------------------------------------------------------------------------------------
    // Generic Actions
    ACTION_DELAY,                                 // Delay (ms)                 Not Used                      Not Used
    ACTION_LAST_GENERIC,                          // [placeholder only]
    
    // Move Actions
    ACTION_OPEN_LID,                              // Not Used                   Not Used                      Not Used
    ACTION_CLOSE_LID,                             // Not Used                   Not Used                      Not Used
    ACTION_MOVE_LID,                              // Start angle                End angle                     Delay(ms) between degrees  
    ACTION_PEEK_LID_FROM_CLOSE,                   // Peek angle offset degrees  Delay(ms) between degrees
    ACTION_CLOSE_LID_FROM_PEEK,                   // Peek angle offset degrees  Delay(ms) between degrees
    ACTION_OPEN_LID_FROM_CLOSE,                   // Delay(ms) between degrees  Not Used                      Not Used
    ACTION_CLOSE_LID_FROM_OPEN,                   // Delay(ms) between degrees  Not Used                      Not Used
    ACTION_EXTEND_ARM,                            //
    ACTION_RETRACT_ARM,                           //
    ACTION_MOVE_ARM,                              // Start angle                End angle                     Delay(ms) between degrees  
    ACTION_EXTEND_ARM_FROM_RETRACTED,             // Delay(ms) between degrees  Not Used                      Not Used
    ACTION_ALMOST_EXTEND_ARM_FROM_RETRACTED,      // Delay(ms) between degrees  Not Used                      Not Used
    ACTION_RETRACT_ARM_FROM_EXTENDED,             // Delay(ms) between degrees  Not Used                      Not Used
  
    // LED Actions
    ACTION_SET_LED,                               // LEFT, RIGHT, or ALL_LEDS   LED color (from color.h)      Not Used
    ACTION_TRANS_LED,                             // LEFT, RIGHT, or ALL_LEDS   End LED color (from color.h)  Delay(ms) between increments           
    
    // Sound Actions
    PITCH_B0  = 31,                               // NOTE_XXX (see below)       Not Used                      Not Used
    PITCH_C1  = 33,                               // ^^^^^^^^^^^^^^^^^^^^ Same for all PITCH_XXX
    PITCH_CS1 = 35,
    PITCH_D1  = 37,
    PITCH_DS1 = 39,
    PITCH_EF1 = 39,                             
    PITCH_E1  = 41,
    PITCH_F1  = 44,
    PITCH_FS1 = 46,
    PITCH_G1  = 49,
    PITCH_GS1 = 52,
    PITCH_AF1 = 52,
    PITCH_A1  = 55,
    PITCH_AS1 = 58,
    PITCH_BF1 = 58,
    PITCH_B1  = 62,
    PITCH_C2  = 65,
    PITCH_CS2 = 69,
    PITCH_D2  = 73,
    PITCH_DS2 = 78,
    PITCH_EF2 = 78,
    PITCH_E2  = 82,
    PITCH_F2  = 87,
    PITCH_FS2 = 93,
    PITCH_GF2 = 93,
    PITCH_G2  = 98,
    PITCH_GS2 = 104,
    PITCH_AF2 = 104,
    PITCH_A2  = 110,
    PITCH_AS2 = 117,
    PITCH_BF2 = 117,
    PITCH_B2  = 123,
    PITCH_C3  = 131,
    PITCH_CS3 = 139,
    PITCH_D3  = 147,
    PITCH_DS3 = 156,
    PITCH_EF3 = 156,
    PITCH_E3  = 165,
    PITCH_F3  = 175,
    PITCH_FS3 = 185,
    PITCH_GF3 = 185,
    PITCH_G3  = 196,
    PITCH_GS3 = 208,
    PITCH_AF3 = 208,
    PITCH_A3  = 220,
    PITCH_AS3 = 233,
    PITCH_BF3 = 233,
    PITCH_B3  = 247,
    PITCH_C4  = 262,
    PITCH_CS4 = 277,
    PITCH_D4  = 294,
    PITCH_DS4 = 311,
    PITCH_EF4 = 311,
    PITCH_E4  = 330,
    PITCH_F4  = 349,
    PITCH_FS4 = 370,
    PITCH_GF4 = 370,
    PITCH_G4  = 392,
    PITCH_GS4 = 415,
    PITCH_AF4 = 415,
    PITCH_A4  = 440,
    PITCH_AS4 = 466,
    PITCH_BF4 = 466,
    PITCH_B4  = 494,
    PITCH_C5  = 523,
    PITCH_CS5 = 554,
    PITCH_D5  = 587,
    PITCH_DS5 = 622,
    PITCH_EF5 = 622,
    PITCH_E5  = 659,
    PITCH_F5  = 698,
    PITCH_FS5 = 740,
    PITCH_G5  = 784,
    PITCH_GS5 = 831,
    PITCH_AF5 = 831,
    PITCH_A5  = 880,
    PITCH_AS5 = 932,
    PITCH_BF5 = 932,
    PITCH_B5  = 988,
    PITCH_C6  = 1047,
    PITCH_CS6 = 1109,
    PITCH_D6  = 1175,
    PITCH_DS6 = 1245,
    PITCH_EF6 = 1245,
    PITCH_E6  = 1319,
    PITCH_F6  = 1397,
    PITCH_FS6 = 1480,
    PITCH_G6  = 1568,
    PITCH_GS6 = 1661,
    PITCH_AF6 = 1661,
    PITCH_A6  = 1760,
    PITCH_AS6 = 1865,
    PITCH_BF6 = 1865,
    PITCH_B6  = 1976,
    PITCH_C7  = 2093,
    PITCH_CS7 = 2217,
    PITCH_D7  = 2349,
    PITCH_DS7 = 2489,
    PITCH_EF7 = 2489,
    PITCH_E7  = 2637,
    PITCH_F7  = 2794,
    PITCH_FS7 = 2960,
    PITCH_G7  = 3136,
    PITCH_GS7 = 3322,
    PITCH_AF7 = 3322,
    PITCH_A7  = 3520,
    PITCH_AS7 = 3729,
    PITCH_BF7 = 3729,
    PITCH_B7  = 3951,
    PITCH_C8  = 4186,
    PITCH_CS8 = 4435,
    PITCH_D8  = 4699,
    PITCH_DS8 = 4978,
    PITCH_REST,
    TEMPO,                                        // TEMPO_XXX (see below)        Not Used                      Not Used
    NEW_TEMPO = TEMPO,                            // TEMPO_XXX (see below)        Not Used                      Not Used
    ARTICULATE,                                   // ARTICULATE_XXX (see below)   Not Used                      Not Used
    ACTION_END
};

// TEMPO
const uint32_t TEMPO_PRESTO = 200; // BPM
const uint32_t TEMPO_ALLEGRO = 150; 
const uint32_t TEMPO_MODERATO = 125; 
const uint32_t TEMPO_ANDANTE = 100;
const uint32_t TEMPO_ADAGIO = 75;
const uint32_t TEMPO_LARGHETTO = 60;
const uint32_t TEMPO_LARGO = 40;

// NOTE
const uint32_t NOTE_WHOLE = 32; // multiplier based on 32nd note
const uint32_t NOTE_DOT_HALF = 24;
const uint32_t NOTE_HALF = 16;
const uint32_t NOTE_DOT_QTR = 12;
const uint32_t NOTE_QTR = 8;
const uint32_t NOTE_DOT_8TH = 6;
const uint32_t NOTE_8TH = 4;
const uint32_t NOTE_DOT_16TH = 3;
const uint32_t NOTE_16TH = 2;
const uint32_t NOTE_32ND = 1;

// ARTICULATE
const uint32_t ARTICULATE_TENUDO = 100; // Percent of note length
const uint32_t ARTICULATE_STACCATO = 80;
const uint32_t ARTICULATE_LEGATO = 95;
