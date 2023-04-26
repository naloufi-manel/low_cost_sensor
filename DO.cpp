#include "DO.h"

#include "OneWire.h"
#include <EEPROM.h>


#define EEPROM_write(address, p) {int i = 0; byte *pp = (byte*)&(p);for(; i < sizeof(p); i++) EEPROM.write(address+i, pp[i]);}
#define EEPROM_read(address, p)  {int i = 0; byte *pp = (byte*)&(p);for(; i < sizeof(p); i++) pp[i]=EEPROM.read(address+i);}
#define adr 0XAA


#define READ_TEMP (25) //Current water temperature ℃, Or temperature sensor function

//Single point calibration needs to be filled CAL1_V and CAL1_T
float CAL1_V; //mv
#define CAL1_T (25)   //℃


// DO saturation table as a function of temperature  
const int DO_Table[41] = {
    14460, 14220, 13820, 13440, 13090, 12740, 12420, 12110, 11810, 11530,
    11260, 11010, 10770, 10530, 10300, 10080, 9860, 9660, 9460, 9270,
    9080, 8900, 8730, 8570, 8410, 8250, 8110, 7960, 7820, 7690,
    7560, 7430, 7300, 7180, 7070, 6950, 6840, 6730, 6630, 6530, 6410};


void readDO(float voltage_mv, int temperature_c, float *od1, float *od2)
{
EEPROM_read(adr, CAL1_V);
if(EEPROM.read(adr)==0xFF){
    CAL1_V=1600;                       // For new EEPROM, write default value( K = 1.0) to EEPROM
    EEPROM_write(adr, CAL1_V);
  }

int V_saturation = (int)CAL1_V + (int)35 * temperature_c - (int)CAL1_T * 35;
float volt= voltage_mv + (int)35 * temperature_c - (int)CAL1_T * 35;
*od1 = (float)(volt *DO_Table[temperature_c]/ (float)V_saturation);
*od2 = (float)(volt *100/ (float)V_saturation);
}



void calibr_DO(char cmd[10], float volt, float temperature){ 
  float CAL1_V;
  Serial.println(F("calibration DO"));
  Serial.println(F("calibration"));
  CAL1_V=volt;
  EEPROM_write(adr, CAL1_V);
}
