/////////////////////////////////////////////////////////////////////////////////////////////
//
// Implementation for the proximitySensor class. The proximity sensor is an ultrasonic 
// module HC-SR04.
//
/////////////////////////////////////////////////////////////////////////////////////////////

// Define preprocessor DEBUG to enable debug output.  Also, be sure to use the F macro when
// printing strings in order to save RAM.

#define DEBUG

#include "proximity.h"

proximitySensor proxSensor;
const float proximitySensor::maxProximityCm = 15; // cm

proximitySensor::proximitySensor()
{
}

proximitySensor::~proximitySensor()
{
}

void proximitySensor::setup()
{
  // Trigger and echo pins.  HC-SR04 uses a separate pin for each.
  // Parallax Ping uses the same pin for trigger and echo.
  pinMode(trigPin, OUTPUT);
  #if (trigPin != echoPin)
  pinMode(echoPin, INPUT);
  #endif
}

float proximitySensor::getDistanceCm()
{     
  static float measuredDistanceCm = 0;
  static float lastDistanceCm = 400;
  static uint32_t proxPrevMs = 0;
  uint32_t currentMs = millis();

  if ((currentMs - proxPrevMs) >= proximityScanMs)
  {
    proxPrevMs = currentMs;
    
    // Emit sound waves
    digitalWrite(trigPin, LOW);
    delayMicroseconds(5);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin,LOW);
    
    // Measure echo
    #if (trigPin == echoPin)
      // Parallax Ping
      pinMode(echoPin, INPUT);
      unsigned long duration = pulseIn(echoPin, HIGH);
      pinMode(trigPin, OUTPUT);
    #else
      // 
      unsigned long duration = pulseIn(echoPin, HIGH);
    #endif

    // Calculate distance
    measuredDistanceCm = duration*.034483/2;
    
    // Greater than 400cm is invalid; however I see this occasionally.
    // This helps smooth the reading.
    if (measuredDistanceCm > 400) 
    {
      measuredDistanceCm = lastDistanceCm;
    }
    
    // Save last distance measured
    lastDistanceCm = measuredDistanceCm;
  }
  return measuredDistanceCm;
}

//
// Read ultrasonic sensor and issue proximity alert if necessary
//
// NOTE: This should be called every time loop() is executed
//

bool proximitySensor::proximityAlertCheck()
{     
  static float lastMeasuredDistanceCm = 0;
  static bool lastDetected = true;
  bool alert = false; // return value
  float measuredDistanceCm = getDistanceCm();
  if (lastMeasuredDistanceCm != measuredDistanceCm)
  {
    lastMeasuredDistanceCm = measuredDistanceCm;
    
    // Check to see if object (probably a human hand) is within proximity of switch
    bool currentDetected = (measuredDistanceCm <= maxProximityCm);
    
    // We really only care if the object has just become within proximity so we don't get multiple
    // triggers.
    if (!lastDetected && currentDetected)
    {
      DebugPrintln(F("Approaching switch..."));
      // Just for fun we will ignore approaches 50% of the time.  We want to give the humans a chance!
      alert = static_cast<bool>(random(2));
      if (alert) DebugPrintln(F("--- issung proximity alert ---"));
      else DebugPrintln(F("ignoring approach"));
    }
    lastDetected = currentDetected;
  }
  return alert;
}
