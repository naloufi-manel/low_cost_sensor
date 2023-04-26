

#include <EEPROM.h>
#include <DFRobot_ADS1115.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "GravityRtc.h"
#include "OneWire.h"
#include "SdService.h"
#include "Debug.h"
#include "ReadSerial.h"
#include "turbidity.h"
#include "dfrobot_EC.h"
#include "dfrobot_PH.h"
#include <SoftwareSerial.h>
#include "DO.h"
#include <DallasTemperature.h>
#include <LowPower.h>


// clock module
GravityRtc rtc;


OneWire oneWire_out(3);
OneWire oneWire_in(5);

DallasTemperature sensor_inbox(&oneWire_in);
DallasTemperature sensor_water(&oneWire_out);
SdService sdService ;

//16 bit ADC module 
DFRobot_ADS1115 ads(&Wire);

float voltagePH, voltage, volt, ecValue, phValue, temperature,temp, turbidit,DO, DO2;
int16_t adc0,adc1, adc2;
String dataString2;
DFRobot_EC ec;
DFRobot_PH ph;



void setup() {
  Serial.begin(115200);
  ec.begin();
  ph.begin(); //!\ PROBLEM HERE the neutral and acid voltage become different (look in the code it's important to initialize at a first use)

  
  rtc.setup(); //For the clock
  sdService.setup();
  sensor_inbox.begin();
  sensor_water.begin();

  ads.setAddr_ADS1115(ADS1115_IIC_ADDRESS1);   // 0x49
  ads.setGain(eGAIN_TWOTHIRDS);   // 2/3x gain
  ads.setMode(eMODE_SINGLE);       // single-shot mode
  ads.setRate(eRATE_128);          // 128SPS (default)
  ads.setOSMode(eOSMODE_SINGLE);   // Set to start a single-conversion
  ads.init();
}

unsigned long updateTime = 0;

void loop() {
  rtc.update();
  char cmd[10];
  // ************************* Serial debugging ******************
  dataString2 = "";

    Serial.print(F("Temperature= "));
    sensor_water.requestTemperatures();
    temperature = sensor_water.getTempCByIndex(0);
    Serial.print(temperature);
    Serial.print(String(temperature));
    dataString2 = sdService.connectString(temperature, dataString2);

    Serial.print(F("°C, ph= "));
    voltagePH = analogRead(A1) / 1024.0 * 5000;      // read the ph voltage
    phValue    = ph.readPH(voltagePH, temperature);      // convert voltage to pH with temperature compensation
    Serial.print(voltagePH, 2);
    Serial.print("_");
    Serial.print(phValue, 2);
    dataString2 = sdService.connectString(phValue, dataString2);

    adc2 = ads.readVoltage(2);
    Serial.print(F(",  Do= "));
    Serial.print(adc2, 2);
    Serial.print("_");
    readDO(adc2, temperature, &DO, &DO2);
    Serial.print(DO2);
    Serial.print(F("%"));
    Serial.print(F(",  Do= "));
    Serial.print(DO);
    Serial.print(F("µg/L"));
    dataString2 = sdService.connectString(DO2, dataString2);
    dataString2 = sdService.connectString(DO, dataString2);
 
    Serial.print(F(", Ec= "));
    adc1 = ads.readVoltage(1); // read the conductivity voltage
    ecValue =  ((ec.readEC(adc1, temperature))*1000); // convert voltage to EC with temperature compensation
    Serial.print(adc1, 2);
    Serial.print("_");
    Serial.print(ecValue);
    dataString2 = sdService.connectString(ecValue, dataString2);
    Serial.print(F("us/cm"));
    
    adc0 = ads.readVoltage(0);
    volt=(adc0/1000.0); // progress with a step of 0.001 volt for each step of 3.67 ntu (turbidity)
    Serial.print(adc0);
    Serial.print("_");
    turbidit = turbidity(volt);
    Serial.print(F(", Turbidity="));
    Serial.print(turbidit); // print out the value you read:
    dataString2 = sdService.connectString(turbidit, dataString2);  
    Serial.println(F(" NTU"));

    sdService.update(dataString2); // enregistrer dans un fichier .csv

    delay(200);
    //Loop for the delay time
    /*for (int l=1; l<113 ; l++){
       LowPower.idle(SLEEP_8S, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF,
                 SPI_OFF, USART0_OFF, TWI_OFF);
    }*/
  }


  // The calibration
  if (readSerial(cmd)) {
    strupr(cmd);

    if (strstr(cmd, "PH")) {
      ph.calibration(voltagePH, temperature, cmd);     //PH calibration process by Serail CMD //enterph //calph //exitph
    }
    if (strstr(cmd, "EC")) {
      //ec.calibration(voltage, temperature, cmd);     //EC calibration process by Serail CMD
      ec.calibration(adc1, temperature, cmd); 
    }
    if (strstr(cmd, "TRB")) {
      calibr_k(cmd, volt);                                // trb1 (400) and trb2 (0)
    }
    if (strstr(cmd, "DO")) {
      calibr_DO(cmd, adc2, temperature);                     // DO (saturation at 25°C and 15°C)
    }
  }
}
