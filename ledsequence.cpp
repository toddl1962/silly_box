/////////////////////////////////////////////////////////////////////////////////////////////
//
// Implementation of the ledSequence class
//
/////////////////////////////////////////////////////////////////////////////////////////////

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !!                                                                                        !!  
// !! LED implementation limitations for UNO and Nano                                        !!
// !!                                                                                        !!  
// !! In order to perform true RGB, each component (color) intensity value is between 0      !!
// !! and 255. This is implemented by using the Arduino 'analogWrite()' function             !!
// !! (which, oddly, has nothing to do with analog output) to set a PWM duty cycle on the    !!
// !! LED output pin.  Unfortunately there are not enough available PWM output pins on the   !!
// !! UNO/Nano to support both the servos and LEDs.  However, because the analogWrite()      !!
// !! function uses digitalWrite() for intentity values 0 and 255 you can use non-PWM        !!
// !! outputs and only use RGB colors that have 0 and 255 intensities on the UNO and Nano.   !!  
// !! Therefore, the same code supports all Arduino platforms by constraining the colors     !!
// !! used.                                                                                  !!
// !!                                                                                        !!  
// !! To further confuse matters, the code below uses the analog pins A0 thru A5 as digital  !!
// !! outputs.                                                                               !!
// !!                                                                                        !!  
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

// Writes an analog value (PWM wave) to a pin.

// Define preprocessor DEBUG to enable debug output.  Also, be sure to use the F macro when
// printing strings in order to save RAM.

//#define DEBUG

#include "ledsequence.h"
#include "color.h"
const int ledSequence::ledPins[2][3] = {{A0, A1, A2}, {A3, A4, A5}};

//
// Implementation for the ledSequence class
//

ledSequence::ledSequence(const void* pSeqTable, seqType aSeqType, seqEnd aSeqEnd):sequence(pSeqTable, aSeqType, aSeqEnd)
{
  startSequence();
}

void ledSequence::setup()
{
  for (int i = 0; i < numLeds; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      pinMode(ledPins[i][j], OUTPUT);
    }
  }
}

void ledSequence::startSequence()
{
  sequence::startSequence();
}

void ledSequence::stopSequence() 
{
  for (int i = 0; i < numLeds; i++)
  {
    setLed(i, 0);
  }
  sequence::stopSequence();
}

sequence::actionState ledSequence::executeAction()
{
  switch (m_seqEntry.action)
  {
    case  ACTION_SET_LED:
      setLed(m_seqEntry.data1, m_seqEntry.data2);
      return ACTION_COMPLETE;

    case  ACTION_TRANS_LED:
      return transitionLed();

    // If not an LED action then assume this is a generic action  
    default:
      return sequence::executeAction();
  }

  return ACTION_EXECUTING;
}

void ledSequence::prepareAction()
{
  sequence::prepareAction();
}

void ledSequence::setLed(int ledNum, const uint32_t color)
{
  if (ledNum == ALL_LEDS)
  {
    // Set each LED to the same color
    for (int i = 0; i < numLeds; i++)
    {
      // Remember the last color
      m_lastColor[i] = color;
      
      // See the note at the beginning of this file regarding RGB
      // implementation
      analogWrite(ledPins[i][0], (color>>16) & 0xff);
      analogWrite(ledPins[i][1], (color>>8) & 0xff);
      analogWrite(ledPins[i][2], color & 0xff);
    }
  }
  else
  {
    // Set an indivudual LED color
    
    // Remember the last color
    m_lastColor[ledNum] = color;
      
    // See the note at the beginning of this file regarding RGB
    // implementation
    analogWrite(ledPins[ledNum][0], (color>>16) & 0xff);
    analogWrite(ledPins[ledNum][1], (color>>8) & 0xff);
    analogWrite(ledPins[ledNum][2], color & 0xff);
  }
}

sequence::actionState ledSequence::transitionLed()
{
  // This action is not really useful for UNO or Nano
  
  // This transition implementation is pretty simple.  Moving toward the
  // destination color is simply a matter of incrementing each color
  // until the source color equals the destination color.  Some colors
  // may finish transitioning before others.
  //
  // The speed is dictated by the delay between increments.  Keep in mind
  // there could be up to 255 increment cycles.

  actionState retVal = ACTION_EXECUTING;
  
  uint32_t newColor;  
  uint8_t sR, sG, sB;
  uint8_t dR, dG, dB;
  
  // If there is no delay or the delay has expired transition one color point for each color
  if (m_seqEntry.data3 == 0 || millis() - m_prevMillis > m_seqEntry.data3)
  {
    // Prepare for next delay
    m_prevMillis = millis();
    
    // Mark the action complete.  This will be overwritten if any of the LEDs
    // are not finished transitioning.
    retVal = ACTION_COMPLETE;
    
    if (m_seqEntry.data1 == ALL_LEDS)
    {
      // Transition all of the LEDs
      for (int i = 0; i < numLeds; i++)
      {
        if (m_lastColor[i] != m_seqEntry.data2)
        {
          // If this LED has not finished transitioning then perform
          // an increment and indicate the action is still executing
          
          // Extract the individual RGB values for the source 
          // color (the last color displayed) and the destination
          // color (contained in data2)
          sR = (m_lastColor[i]>>16) & 0xff;
          sG = (m_lastColor[i]>>8) & 0xff;
          sB = m_lastColor[i] & 0xff;
          dR = (m_seqEntry.data2>>16) & 0xff;
          dG = (m_seqEntry.data2>>8) & 0xff;
          dB = m_seqEntry.data2 & 0xff;
          
          // Increment or decrement based on the direction the
          // color is moving
          if (sR < dR) sR++;
          else if (sR > dR) sR--;
          if (sG < dG) sG++;
          else if (sG > dG) sG--;
          if (sB < dB) sB++;
          else if (sB > dB) sB--;
          
          // Set the new color
          newColor = RGB(sR, sG, sB);
          setLed(i, newColor);
          
          // Indicate this action is still executing
          retVal = ACTION_EXECUTING;
        }
      }
    }
    else
    {
      if (m_lastColor[m_seqEntry.data1] != m_seqEntry.data2)
      {
        // If the LED has not finished transitioning then perform
        // an increment and indicate the action is still executing
          
        // Extract the individual RGB values for the source 
        // color (the last color displayed) and the destination
        // color (contained in data2)
        sR = (m_lastColor[m_seqEntry.data1]>>16) & 0xff;
        sG = (m_lastColor[m_seqEntry.data1]>>8) & 0xff;
        sB = m_lastColor[m_seqEntry.data1] & 0xff;
        dR = (m_seqEntry.data2>>16) & 0xff;
        dG = (m_seqEntry.data2>>8) & 0xff;
        dB = m_seqEntry.data2 & 0xff;
          
        // Increment or decrement based on the direction the
        // color is moving
        if (sR < dR) sR++;
        else if (sR > dR) sR--;
        if (sG < dG) sG++;
        else if (sG > dG) sG--;
        if (sB < dR) sR++;
        else if (sR > dR) sB--;
          
        // Set the new color
        newColor = RGB(sR, sG, sB);
        setLed(m_seqEntry.data1, newColor);
          
        // Indicate this action is still executing
        retVal = ACTION_EXECUTING;
      }
    }
  }
  return retVal;
}
