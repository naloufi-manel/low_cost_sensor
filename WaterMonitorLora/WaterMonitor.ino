
#include <DFRobot_ADS1115.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include "dfrobot_EC.h"
#include "dfrobot_PH.h"
#include "GravityRtc.h"
#include "OneWire.h"
#include "SdService.h"
#include "Debug.h"
#include "ReadSerial.h"
#include "turbidity.h"
#include "DO.h"
#include <DallasTemperature.h>
#include <lmic.h>
#include <hal/hal.h>

#define CHANNEL 0
GravityRtc rtc; // clock module
SdService sdService ;

#define ONE_WIRE_BUS_1 3
#define ONE_WIRE_BUS_2 5

OneWire oneWire_out(ONE_WIRE_BUS_1);
OneWire oneWire_in(ONE_WIRE_BUS_2);

DallasTemperature sensor_inbox(&oneWire_in);
DallasTemperature sensor_water(&oneWire_out);

DFRobot_ADS1115 ads(&Wire); //16 bit ADC module 
DFRobot_EC ec;
DFRobot_PH ph;

float voltagePH, voltage, volt, ecValue, ecval, phValue, temperature,temp, turbidit,DO, DO2; 
int16_t adc0, adc1, adc2;
String dataString2;
static osjob_t sendjob;
// Schedule TX every this many seconds (might become longer due to duty cycle limitations).
const unsigned TX_INTERVAL = 120;


typedef union {
 float f[6];
 unsigned char bytes[8];
} donnees;
donnees datas;

// LoRaWAN NwkSKey, network session key
// This is the default Semtech key, which is used by the prototype TTN
// network initially.
//ttn
static const PROGMEM u1_t NWKSKEY[16] = { 0x91, 0x78, 0x1D, 0xE3, 0xA1, 0x49, 0x86, 0x6C, 0x67, 0x06, 0xB0, 0xB8, 0xC2, 0x30, 0x70, 0xD0 };
static const PROGMEM u1_t APPSKEY[16] = { 0x07, 0x1D, 0x9B, 0x81, 0x11, 0x53, 0x90, 0x15, 0x37, 0xB8, 0x57, 0xF2, 0x6B, 0xCA, 0xB9, 0x6D };
static const PROGMEM u4_t DEVADDR = 0x260B9BC1;


// These callbacks are only used in over-the-air activation, so they are
// left empty here (we cannot leave them out completely unless
// DISABLE_JOIN is set in config.h, otherwise the linker will complain).
void os_getArtEui (u1_t* buf) { }
void os_getDevEui (u1_t* buf) { }
void os_getDevKey (u1_t* buf) { }


// Pin mapping
const lmic_pinmap lmic_pins = {
    .nss = 10,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = 9,
    .dio = {2, 6, 7},
};

void data(){
    sensor_inbox.requestTemperatures();
    temp = sensor_inbox.getTempCByIndex(0);
    datas.f[0]=round(temp*1000);
    
    sensor_water.requestTemperatures();
    temperature = sensor_water.getTempCByIndex(0);
    datas.f[1]=round(temperature*1000);
   
    voltagePH = analogRead(A1) / 1024.0 * 5000;   
    phValue    = ph.readPH(voltagePH, temperature);  
    //phV = phValue * 100; 
    //mydata[2] = highByte(phV);
    //mydata[3] = lowByte(phV); 
    datas.f[2]=round(phValue*1000);  
 
    adc2 = ads.readVoltage(2);
    readDO(adc2, temperature, &DO, &DO2);
    //D = DO; 
    //D2 = DO2 * 100; 
    //mydata[4] = highByte(D2);
    //mydata[5] = lowByte(D2);
    //Serial.println(D2);
    //Serial.println(D);
    //mydata[6] = highByte(D);
    //mydata[7] = lowByte(D);
    datas.f[3]=round(DO2*1000); //%
    datas.f[4]=round(DO*1000); // ug/l
 
    adc1 = ads.readVoltage(1); // read the conductivity voltage
    ecValue =  ec.readEC(adc1, temperature); // convert voltage to EC with temperature compensation
    ecval =ecValue*1000;
    //ecV = ecval * 100; 
    //mydata[8] = highByte(ecV);
    //mydata[9] = lowByte(ecV);  
    datas.f[5]=round(ecval*1000);
    
    //turbidity
    adc0 = ads.readVoltage(0);
    volt=(adc0/1000.0); 
    turbidit = turbidity(volt);
    //turbV= turbidit *10 ;
    //mydata[10] = highByte(turbV);
    //mydata[11] = lowByte(turbV); 
    datas.f[6]=round(turbidit); 
     
    
}

void printdata(){
    Serial.print(F("Temperature box= "));
    Serial.print(temp);
    Serial.print(F("°C, "));
    Serial.print(F("Temperature= "));
    Serial.print(temperature);
    Serial.print(F("°C, ph= "));    
    Serial.print(phValue, 3);
    Serial.print(F(",  Do= "));
    Serial.print(DO2);
    Serial.print(F("%"));
    Serial.print(F(",  Do= "));
    Serial.print(DO);
    Serial.print(F("µg/L"));
    Serial.print(F(", Ec= "));
    Serial.print(ecval);
    Serial.print(F("us/cm"));
    Serial.print(F(", Turbidity="));
    Serial.print(turbidit); 
    Serial.println(F(" NTU"));
    
    dataString2 = "";
    dataString2 = sdService.connectString(temp, dataString2);
    dataString2 = sdService.connectString(temperature, dataString2);
    dataString2 = sdService.connectString(phValue, dataString2);
    dataString2 = sdService.connectString(DO2, dataString2);
    dataString2 = sdService.connectString(DO, dataString2);
    dataString2 = sdService.connectString(ecval, dataString2);
    dataString2 = sdService.connectString(turbidit, dataString2);
    sdService.update(dataString2); // enregistrer dans un fichier .csv

}

void do_send(osjob_t* j){
    // Check if there is not a current TX/RX job running
    if (LMIC.opmode & OP_TXRXPEND) {
        Serial.println(F("OP_TXRXPEND, not sending"));
    } else {
        // Prepare upstream data transmission at the next possible time.
        data();
        printdata();
        Serial.print(sizeof(datas.bytes));
        LMIC_setTxData2(1, datas.bytes, 28, 0); // le 4eme attribu "0" : indique unconfirmed uplink  : LMIC_setTxData2(1, datas.bytes, sizeof(mydata), 0);
        Serial.println(F("Packet queued"));
        Serial.println(LMIC.freq);
    }
    // Next TX is scheduled after TX_COMPLETE event.
}

void onEvent (ev_t ev) {
    Serial.print(os_getTime());
    Serial.print(F(": "));
    Serial.println(ev);
    switch(ev) {
        case EV_SCAN_TIMEOUT:
            //Serial.println(F("EV_SCAN_TIMEOUT"));
            break;
        case EV_BEACON_FOUND:
            //Serial.println(F("EV_BEACON_FOUND"));
            break;
        case EV_BEACON_MISSED:
            //Serial.println(F("EV_BEACON_MISSED"));
            break;
        case EV_BEACON_TRACKED:
            //Serial.println(F("EV_BEACON_TRACKED"));
            break;
        case EV_JOINING:
            //Serial.println(F("EV_JOINING"));
            break;
        case EV_JOINED:
            //Serial.println(F("EV_JOINED"));
            break;
        case EV_RFU1:
            //Serial.println(F("EV_RFU1"));
            break;
        case EV_JOIN_FAILED:
            //Serial.println(F("EV_JOIN_FAILED"));
            break;
        case EV_REJOIN_FAILED:
            //Serial.println(F("EV_REJOIN_FAILED"));
            break;
        case EV_TXCOMPLETE:
            Serial.println(F("EV_TXCOMPLETE (includes waiting for RX windows)"));
            if(LMIC.dataLen) {
                // data received in rx slot after tx
                Serial.print(F("Data Received: "));
                Serial.write(LMIC.frame+LMIC.dataBeg, LMIC.dataLen);
                Serial.println();
            }
            // Schedule next transmission
            os_setTimedCallback(&sendjob, os_getTime()+sec2osticks(TX_INTERVAL), do_send);
            break;
        case EV_LOST_TSYNC:
            //Serial.println(F("EV_LOST_TSYNC"));
            break;
        case EV_RESET:
            //Serial.println(F("EV_RESET"));
            break;
        case EV_RXCOMPLETE:
            // data received in ping slot
            //Serial.println(F("EV_RXCOMPLETE"));
            break;
        case EV_LINK_DEAD:
            //Serial.println(F("EV_LINK_DEAD"));
            break;
        case EV_LINK_ALIVE:
            //Serial.println(F("EV_LINK_ALIVE"));
            break;
         default:
            //Serial.println(F("Unknown event"));
            break;
    }
}


void setup() {
  Serial.begin(115200);
  ec.begin();
  ph.begin(); 
  sensor_inbox.begin();
  sensor_water.begin();
  
  rtc.setup(); //pour televersement arduino
  sdService.setup();

  ads.setAddr_ADS1115(ADS1115_IIC_ADDRESS1);   // 0x49
  ads.setGain(eGAIN_TWOTHIRDS);   // 2/3x gain
  ads.setMode(eMODE_SINGLE);       // single-shot mode
  ads.setRate(eRATE_128);          // 128SPS (default)
  ads.setOSMode(eOSMODE_SINGLE);   // Set to start a single-conversion
  ads.init();

  
  while(!Serial);
  Serial.println(F("Starting"));
  #ifdef VCC_ENABLE
  // For Pinoccio Scout boards
  pinMode(VCC_ENABLE, OUTPUT);
  digitalWrite(VCC_ENABLE, HIGH);
  #endif

    // LMIC init
    os_init();
    // Reset the MAC state. Session and pending data transfers will be discarded.
    LMIC_reset();
    //LMIC_setClockError(MAX_CLOCK_ERROR * 1/100);
    // Set static session parameters. Instead of dynamically establishing a session
    // by joining the network, precomputed session parameters are be provided.
    #ifdef PROGMEM
    // On AVR, these values are stored in flash and only copied to RAM
    // once. Copy them to a temporary buffer here, LMIC_setSession will
    // copy them into a buffer of its own again.
    uint8_t appskey[sizeof(APPSKEY)];
    uint8_t nwkskey[sizeof(NWKSKEY)];
    memcpy_P(appskey, APPSKEY, sizeof(APPSKEY));
    memcpy_P(nwkskey, NWKSKEY, sizeof(NWKSKEY));
    LMIC_setSession (0x1, DEVADDR, nwkskey, appskey);
    #else
    // If not running an AVR with PROGMEM, just use the arrays directly 
    LMIC_setSession (0x1, DEVADDR, NWKSKEY, APPSKEY);
    #endif

    // Disable link check validation
    LMIC_setLinkCheckMode(0);

    // TTN uses SF9 for its RX2 window.
    LMIC.dn2Dr = DR_SF9;

    for (uint8_t i=0; i<10;i++){
      if (i!=CHANNEL){
        LMIC_disableChannel(i);
      }
    }
    // Set data rate and transmit power (note: txpow seems to be ignored by the library)
    // spreding factor and transmission power 
    LMIC_setDrTxpow(DR_SF12,20); //ici c'etait 14 # avec 0 le snr augmente de 10 à 13 # avec 100 : ca passe à 9 
    // Start job
    do_send(&sendjob);
}

void loop() {
  rtc.update();
  char cmd[10];
  
  if (readSerial(cmd)) {
    strupr(cmd);
    Serial.println(cmd);

    if (strstr(cmd, "PH")) {
      ph.calibration(voltagePH, temperature, cmd);     //PH calibration process by Serail CMD //enterph //calph //exitph
    }
    if (strstr(cmd, "EC")) {
      ec.calibration(voltage, temperature, cmd);     //EC calibration process by Serail CMD
    }
    if (strstr(cmd, "TRB")) {
      calibr_k(cmd, volt);                                   // trb1 (400) et trb2 (0)
    }
    if (strstr(cmd, "DO")) {
      calibr_DO(cmd, adc2, temperature);                     // DO (saturation à 25°C)
    }
  }
  os_runloop_once();  
}
