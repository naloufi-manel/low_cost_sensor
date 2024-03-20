#include "DO.h"

#include "OneWire.h"
#include <EEPROM.h>


#define EEPROM_write(address, p) {int i = 0; byte *pp = (byte*)&(p);for(; i < sizeof(p); i++) EEPROM.write(address+i, pp[i]);}
#define EEPROM_read(address, p)  {int i = 0; byte *pp = (byte*)&(p);for(; i < sizeof(p); i++) pp[i]=EEPROM.read(address+i);}
#define adr 0XAA

//Single-point calibration Mode=0
//Two-point calibration Mode=1
//#define TWO_POINT_CALIBRATION 0
// affichage en % (1) et en mg/l (0)
//#define Percentage 1

#define READ_TEMP (25) //Current water temperature ℃, Or temperature sensor function

//Single point calibration needs to be filled CAL1_V and CAL1_T
float CAL1_V; //mv
#define CAL1_T (25)   //℃
//Two-point calibration needs to be filled CAL2_V and CAL2_T
//CAL1 High temperature point, CAL2 Low temperature point
//int CAL2_V ; //mv
//#define CAL2_T (15)   //℃

// table do de saturation en fonction de la temperature 
const int DO_Table[41] = {
    14460, 14220, 13820, 13440, 13090, 12740, 12420, 12110, 11810, 11530,
    11260, 11010, 10770, 10530, 10300, 10080, 9860, 9660, 9460, 9270,
    9080, 8900, 8730, 8570, 8410, 8250, 8110, 7960, 7820, 7690,
    7560, 7430, 7300, 7180, 7070, 6950, 6840, 6730, 6630, 6530, 6410};


void readDO(float voltage_mv, int temperature_c, float *od1, float *od2)
{
EEPROM_read(adr, CAL1_V);
//EEPROM_read(adr+2, CAL2_V);
if(EEPROM.read(adr)==0xFF){
    CAL1_V=1600;                       // For new EEPROM, write default value( K = 1.0) to EEPROM
    EEPROM_write(adr, CAL1_V);
}
/*
if(EEPROM.read(adr+2)==0){
    CAL2_V=1300;                       // For new EEPROM, write default value( K = 1.0) to EEPROM
    EEPROM_write(adr+2, CAL2_V);
}*/
//*percentage=Percentage;
//#if TWO_POINT_CALIBRATION == 0 // single point calibration
  //Serial.print(CAL1_V);
  //Serial.print("_");
int V_saturation = (int)CAL1_V + (int)35 * temperature_c - (int)CAL1_T * 35;
//int V_saturation = CAL1_V;
//Serial.print(V_saturation);
float volt= voltage_mv + (int)35 * temperature_c - (int)CAL1_T * 35;
*od1 = (float)(volt *DO_Table[temperature_c]/ (float)V_saturation);
*od2 = (float)(volt *100/ (float)V_saturation);
  //Serial.print("_");
  //Serial.print(voltage_mv);
  //Serial.print("_");
  //Serial.print(*od);
  //Serial.print(V_saturation);
  //return (voltage_mv * DO_Table[temperature_c] / V_saturation);
//#endif
}



void calibr_DO(char cmd[10], float volt, float temperature){ 
  float CAL1_V;
  //float CAL2_V;
  Serial.println(F("calibration DO"));
  //if ((temperature > 23) & (temperature < 26) ){ //
  Serial.println(F("calibration"));
  CAL1_V=volt;
  EEPROM_write(adr, CAL1_V);
  /*}
  if ((temperature > 13) & (temperature < 16)){ //11.6 NTU // voir vers 5 ntu 
    Serial.println(F("calibration  15°C"));
    CAL2_V=volt;
    EEPROM_write(adr+2, CAL2_V);
  }*/
}
