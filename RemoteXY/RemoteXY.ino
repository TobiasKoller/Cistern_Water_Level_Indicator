/*
   -- cistern_waterlevel_indicator --
   
   This source code of graphical user interface 
   has been generated automatically by RemoteXY editor.
   To compile this code using RemoteXY library 3.1.13 or later version 
   download by link http://remotexy.com/en/library/
   To connect using RemoteXY mobile app by link http://remotexy.com/en/download/                   
     - for ANDROID 4.13.13 or later version;
     - for iOS 1.10.3 or later version;
    
   This source code is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.    
*/

//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////

// you can enable debug logging to Serial at 115200
//#define REMOTEXY__DEBUGLOG    

// RemoteXY select connection mode and include library 
#define REMOTEXY_MODE__ESP32CORE_BLE

#include <BLEDevice.h>

// RemoteXY connection settings 
#define REMOTEXY_BLUETOOTH_NAME "Cistern Waterlevel Indicator"

#include <Preferences.h>
#include <RemoteXY.h>

#include <string>
#include "DistanceSensor.h"

// RemoteXY GUI configuration  
#pragma pack(push, 1)  
uint8_t RemoteXY_CONF[] =   // 117 bytes
  { 255,4,0,4,0,110,0,17,0,0,0,30,1,106,200,1,1,5,0,71,
  7,93,92,92,56,0,2,24,135,0,0,0,0,0,0,200,66,0,0,160,
  65,0,0,32,65,0,0,0,64,24,0,7,11,13,85,12,52,2,26,2,
  7,11,38,85,12,52,2,26,2,129,12,5,56,7,26,65,98,115,116,97,
  110,100,32,86,111,108,108,32,40,99,109,41,0,129,12,30,58,7,26,65,
  98,115,116,97,110,100,32,76,101,101,114,32,40,99,109,41,0 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  int16_t distance_full; // -32768 .. +32767
  int16_t distance_empty; // -32768 .. +32767

    // output variables
  float waterlevel_control; // from 0 to 100

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0

} RemoteXY;   
#pragma pack(pop)
 
/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////

Preferences preferences;
DistanceSensor _distanceSensor;

int _init_distance_full = 0;
int _init_distance_empty = 0;


void setup() 
{
  RemoteXY_Init (); 

  Serial.begin(9600);
  
  // TODO you setup code
  initValuesFromStorage();
  
}

void initValuesFromStorage()
{
  preferences.begin("settings", true);

  _init_distance_empty = preferences.getInt("distance_empty", 0);
  _init_distance_full = preferences.getInt("distance_full", 0);

  RemoteXY.distance_empty = _init_distance_empty;
  RemoteXY.distance_full = _init_distance_full;

  preferences.end();
}

bool hasDistanceSettingsChanged()
{  
  return ( RemoteXY.distance_empty != _init_distance_empty || RemoteXY.distance_full != _init_distance_full);
}

void saveValuesToStorage()
{
  _init_distance_empty = RemoteXY.distance_empty;
  _init_distance_full = RemoteXY.distance_full;

  preferences.begin("settings", false);
  preferences.putInt("distance_empty", _init_distance_empty );
  preferences.putInt("distance_full", _init_distance_full );
  preferences.end();
}

void loop() 
{ 
  RemoteXY_Handler ();

  _distanceSensor.Setup(RemoteXY.distance_empty, RemoteXY.distance_full);

  int percentage = _distanceSensor.CalculateDistanceInPercentage();
  Serial.println("percentage: ");
  Serial.println(std::to_string(percentage).c_str());
  RemoteXY.waterlevel_control = static_cast<float>(percentage);
  
  if(hasDistanceSettingsChanged())
  {
    Serial.println("distance has changed");
    saveValuesToStorage();
  }
  delay(5000);
  // Serial.println(max);
  // TODO you loop code
  // use the RemoteXY structure for data transfer
  // do not call delay(), use instead RemoteXY_delay() 
}