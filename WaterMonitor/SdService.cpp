
#if  defined(__AVR_ATmega2560__)

const int CsPin = 53;

#else

const int CsPin = 4;

#endif


#include "SdService.h"
#include <SPI.h>
#include "Debug.h"
#include "GravityRtc.h"

extern GravityRtc rtc;
//String dataString2 = "";
String dataString = "";

//ISensor * gravitySensor []
SdService :: SdService (): chipSelect (CsPin), sdDataUpdateTime ( 0 )
{
}


SdService :: ~ SdService ()
{
}

//********************************************************************************************
// function name: setup ()
// Function Description: Initialize the SD card
//********************************************************************************************
void SdService::setup()
{
  Debug::println(F("Initializing SD card..."));

  pinMode(SS, OUTPUT);

  if (!SD.begin(chipSelect))
  {
    Debug::println(F("Card failed, or not present"));
    return;
  }
  sdReady = true;
  Debug::println(F("card initialized."));

  // write the file header
  dataFile = SD.open("sensor.csv", FILE_WRITE);
  if (dataFile && dataFile.position() == 0) {
    dataFile.println(F("date,temp(C),pH,DO(%),DO(µg/L),ec(µs/m),turbi(ntu)"));
    dataFile.close();
  }

}


//********************************************************************************************
// function name: update ()
// Function Description: Update the data in the SD card
//********************************************************************************************
void SdService::update(String dataString3)
{
  //if (sdReady && millis() - sdDataUpdateTime > SDUPDATEDATATIME) 
  //{
    Serial.println(F("Write Sd card")); 
    dataString = "";
    // Year Month Day Hours Minute Seconds
    dataString += String(rtc.year,10);
    dataString += "/";
    dataString += String(rtc.month, 10);
    dataString += "/";
    dataString += String(rtc.day, 10);
    dataString += "/";
    dataString += String(rtc.hour, 10);
    dataString += "/";
    dataString += String(rtc.minute, 10);
    dataString += "/";
    dataString += String(rtc.second, 10);
    dataString += ",";

    // write SD card, write data twice, to prevent a single write data caused by the loss of too large
    dataFile = SD.open("sensor.csv", FILE_WRITE);
    if (dataFile)
    {
      dataFile.print(dataString);
      dataFile.close();
      Debug::print(dataString);

    }
    Serial.print(dataString);

    dataFile = SD.open("sensor.csv", FILE_WRITE);
    if (dataFile)
    {
      dataFile.println(dataString3);
      dataFile.close();
      Debug::println(dataString3);

    }
}


//********************************************************************************************
// function name: connectString ()
// Function Description: Connects the string data
//********************************************************************************************
String SdService::connectString(float value, String dataString2)
{
  dataString2 += String(value);
  dataString2 += ",";
  return dataString2;
}
