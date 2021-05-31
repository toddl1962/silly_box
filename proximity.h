/////////////////////////////////////////////////////////////////////////////////////////////
//
// This class defines the methods and attributes for handling of the proximity sensor.
// The proximity sensor is an ultrasonic module HC-SR04.
//
/////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Arduino.h"
#include "debug.h"

class proximitySensor
{
  private:
    static const int trigPin = 8; // "trig" pin on the ultrasonic sensor
    static const int echoPin = 9; // "echo" pin on the ultrasonic sensor
    static const float maxProximityCm;
    static const uint32_t proximityScanMs = 100;

  // Methods
  public:
    proximitySensor();    
    ~proximitySensor();    
    float getDistanceCm();
    bool proximityAlertCheck();
    static void setup();
};

extern proximitySensor proxSensor;
