/////////////////////////////////////////////////////////////////////////////////////////////
//
// This class defines the methods and attributes for handling of the proximity sensor.
// The proximity sensor is an ultrasonic module HC-SR04.
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
