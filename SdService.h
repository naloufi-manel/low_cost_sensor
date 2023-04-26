#pragma once

#include "ISensor.h"
#include <SD.h>
#include "string.h"

class SdService
{

public:
  int chipSelect;
public:
  SdService(); //ISensor* gravitySensor[]
  ~ SdService ();

  // initialization
  void  setup ();

  // Update write SD card data
  void  update (String dataString3);

    // Connect the string data
  String connectString(float value, String dataString2);

private:
  // points to the pointer to the array of sensors
  bool sdReady = false;

  // file handle
  File dataFile;
  unsigned long sdDataUpdateTime;


};
