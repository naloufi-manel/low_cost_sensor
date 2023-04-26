
#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "dfrobot_EC.h"
#include <EEPROM.h>

#define EEPROM_write(address, p) {int i = 0; byte *pp = (byte*)&(p);for(; i < sizeof(p); i++) EEPROM.write(address+i, pp[i]);}
#define EEPROM_read(address, p)  {int i = 0; byte *pp = (byte*)&(p);for(; i < sizeof(p); i++) pp[i]=EEPROM.read(address+i);}

#define KVALUEADDR 0x0A    //the start address of the K value stored in the EEPROM
#define RES2 820.0
#define ECREF 200.0

DFRobot_EC::DFRobot_EC()
{
    this->_ecvalue                = 0.0;
    this->_kvalue                = 1.0;
    this->_inter                 = 0.0;
    this->_kvalueLow              = 1.0;
    this->_kvalueHigh             = 1.0;
    this->_cmdReceivedBufferIndex = 0;
    this->_voltage                = 0.0;
    this->_temperature            = 25.0;
} 

DFRobot_EC::~DFRobot_EC()
{

}

void DFRobot_EC::begin()
{
    EEPROM_read(KVALUEADDR, this->_kvalue);        //read the calibrated K value from EEPROM
    if(EEPROM.read(KVALUEADDR)==0xFF && EEPROM.read(KVALUEADDR+1)==0xFF && EEPROM.read(KVALUEADDR+2)==0xFF && EEPROM.read(KVALUEADDR+3)==0xFF){
        this->_kvalueLow = 1.0;                       // For new EEPROM, write default value( K = 1.0) to EEPROM
        EEPROM_write(KVALUEADDR, this->_kvalue);
    }
    EEPROM_read(KVALUEADDR+4, this->_inter);     //read the calibrated K value from EEPRM
    if(EEPROM.read(KVALUEADDR+4)==0xFF && EEPROM.read(KVALUEADDR+5)==0xFF && EEPROM.read(KVALUEADDR+6)==0xFF && EEPROM.read(KVALUEADDR+7)==0xFF){
        this->_kvalueHigh = 0.0;                      // For new EEPROM, write default value( K = 1.0) to EEPROM
        EEPROM_write(KVALUEADDR+4, this->_inter);
    }
}

float DFRobot_EC::readEC(float voltage, float temperature)
{
    float val;
    this->_rawEC = 1000*voltage/RES2/ECREF;
    Serial.print(voltage);
    Serial.print(this->_rawEC);
    val = this->_rawEC * (1.0+0.0185*(temperature-25.0)); //temperature compensation
    this->_ecvalue = val * this->_kvalue + this->_inter;           //calculate the EC value after automatic shift  
    if (this->_ecvalue <=this->_inter){
      this->_ecvalue =0;
    }
    return this->_ecvalue;     //store the EC value for Serial CMD calibration
}

void DFRobot_EC::calibration(float voltage, float temperature,char* cmd)
{   
    this->_voltage = voltage;
    this->_temperature = temperature;
    strupr(cmd);
    ecCalibration(cmdParse(cmd));                     //if received Serial CMD from the serial monitor, enter into the calibration mode
}

void DFRobot_EC::calibration(float voltage, float temperature)
{   
    this->_voltage = voltage;
    this->_temperature = temperature;
    
    if(cmdSerialDataAvailable() > 0)
    {
        ecCalibration(cmdParse());  // if received Serial CMD from the serial monitor, enter into the calibration mode
    }
}

boolean DFRobot_EC::cmdSerialDataAvailable()
{
    char cmdReceivedChar;
    static unsigned long cmdReceivedTimeOut = millis();
    while (Serial.available()>0) 
    {
        if(millis() - cmdReceivedTimeOut > 500U){
            this->_cmdReceivedBufferIndex = 0;
            memset(this->_cmdReceivedBuffer,0,(ReceivedBufferLength));
        }
        cmdReceivedTimeOut = millis();
        cmdReceivedChar = Serial.read();
        if(cmdReceivedChar == '\n' || this->_cmdReceivedBufferIndex==ReceivedBufferLength-1){
            this->_cmdReceivedBufferIndex = 0;
            strupr(this->_cmdReceivedBuffer);
            return true;
        }else{
            this->_cmdReceivedBuffer[this->_cmdReceivedBufferIndex] = cmdReceivedChar;
            this->_cmdReceivedBufferIndex++;
        }
    }
    return false;
}

byte DFRobot_EC::cmdParse(const char* PROGMEM cmd)
{
    byte modeIndex = 0;
    if(strstr(cmd, "ENTEREC")      != NULL){
        modeIndex = 1;
    }else if(strstr(cmd, "EXITEC") != NULL){
        modeIndex = 3;
    }else if(strstr(cmd, "CALEC")  != NULL){
        modeIndex = 2;
    }
    return modeIndex;
}

byte DFRobot_EC::cmdParse()
{
    byte modeIndex = 0;
    if(strstr(this->_cmdReceivedBuffer, "ENTEREC")     != NULL)
        modeIndex = 1;
    else if(strstr(this->_cmdReceivedBuffer, "EXITEC") != NULL)
        modeIndex = 3;
    else if(strstr(this->_cmdReceivedBuffer, "CALEC")  != NULL)
        modeIndex = 2;
    return modeIndex;
}

void DFRobot_EC::ecCalibration(byte mode)
{
    char *receivedBufferPtr;
    static boolean ecCalibrationFinish  = 0;
    static boolean enterCalibrationFlag = 0;
    static float compECsolutionHigh, compECsolutionLow;
    float KValueTemp, KValueInter;
    switch(mode){
        case 0:
        if(enterCalibrationFlag){
            Serial.println(F(">>>Command Error<<<"));
        }
        break;
        case 1:
        enterCalibrationFlag = 1;
        ecCalibrationFinish  = 0;
        Serial.println();
        Serial.println(F(">>>Enter EC Calibration Mode<<<"));
        Serial.println(F(">>>Please put the probe into the 1413us/cm or 360us/cm buffer solution<<<"));
        Serial.println();
        break;
        case 2:
        if(enterCalibrationFlag){
            if((this->_rawEC>0.88)&&(this->_rawEC<1.9)){                         //recognize 1.413us/cm buffer solution
                Serial.println(F("enterCalibrationFlag : 0.88 < _rawEC < 1.9"));
                this->_kvalueHigh =  this->_rawEC*(1.0+0.0185*(this->_temperature-25.0)); 
                Serial.print(F(">>>Successful,"));
                Serial.println(F(" Send EXITEC to Save and Exit<<<"));
                ecCalibrationFinish = 1;
            }else if((this->_rawEC>0)&&(this->_rawEC<0.88)){                    //recognize 12.88ms/cm buffer solution
                Serial.println(F("enterCalibrationFlag : 0 < _rawEC < 0.88"));  //temperature compensation
                this->_kvalueLow =  this->_rawEC*(1.0+0.0185*(this->_temperature-25.0)); 
                Serial.print(F(">>>Successful,"));
                Serial.println(F(" Send EXITEC to Save and Exit<<<"));
                ecCalibrationFinish = 1;
            }else{
                Serial.print(F(">>>Buffer Solution Error Try Again<<<   "));
                ecCalibrationFinish = 0;
            }

          }
        break;
        case 3:
        if(enterCalibrationFlag){
                if(ecCalibrationFinish){   
                  this->_kvalue = (1.413-0.36)/(this->_kvalueHigh-this->_kvalueLow);
                  this->_inter = 1.413 - this->_kvalue* this->_kvalueHigh;
                  EEPROM_write(KVALUEADDR, this->_kvalue);
                  EEPROM_write(KVALUEADDR+4, this->_inter);
                     Serial.print(F(">>>Calibration Successful"));
                }else{
                    Serial.print(F(">>>Calibration Failed"));
                }
                Serial.println(F(",Exit EC Calibration Mode<<<"));
                ecCalibrationFinish  = 0;
                enterCalibrationFlag = 0;
        }
        break;
    }
}
