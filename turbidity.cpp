
#include "turbidity.h"
#include "OneWire.h"
#include <EEPROM.h>


#define EEPROM_write(address, p) {int i = 0; byte *pp = (byte*)&(p);for(; i < sizeof(p); i++) EEPROM.write(address+i, pp[i]);}
#define EEPROM_read(address, p)  {int i = 0; byte *pp = (byte*)&(p);for(; i < sizeof(p); i++) pp[i]=EEPROM.read(address+i);}
#define adr 0XA0


double turbidity( float volt){
  float a;
  float b;
  float difference;
  float voltage;
  double turbidity;

  EEPROM_read(adr+8, difference);
  EEPROM_read(adr,a);
  EEPROM_read(adr+4,b);

  voltage=(a*volt)+b;
  turbidity=(voltage-3.9994)/(-0.0008); //Hakim et al., 2019
  if (turbidity<0){
    turbidity=0;
  }
  return turbidity;
}


void calibr_k(char cmd[10], float volt){ 
  float x1,x2,a,b;
  Serial.println(F("calibration turbidity"));
  if (strstr(cmd,"TRB1")){ //400ntu : 3.8394
    Serial.println(F("calibration x1"));
    x1=volt;
    x2=3.9994;
  }
  if (strstr(cmd,"TRB2")){ //0 NTU : around 5 ntu 
    Serial.println(F("calibration x2"));
    x2 =volt;
  }
  if ((x1 > 0) && (x2 > 0)){
    Serial.print(F(">>>Calibration Successful"));
    a=(3.9994 - 3.6794)/(x2-x1); // 4.087 volt for 400 ntu
    b=3.9994- a*x2;
    Serial.print(a);
    Serial.print(b); 
    EEPROM_write(adr, a) 
    EEPROM_write(adr+4, b)
  }
  
}
