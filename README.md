

# River quality sensor with Arduino

KnowFlow is an open source water monitoring device and an education program.

![](https://user-images.githubusercontent.com/63862628/233610190-d4e71730-28c1-4eff-9fb9-56338c6961b2.jpg)



For the device part, this sensor is designed for environmental activists to monitor the water quality using low cost sensor. It is based on arduino uno and 
can currently monitor 5 parameters: Temperature, pH, Electrical conductivity, Turbidity and Dissolved Oxygen. The data is stored on a micro SD card. All the modules are easy to change or add. Most of the sensors used are from DFRobot.
In addition to the 5 included sensors, you can add other environmental sensors, such as light, temperature, humidity sensors and a communication module to collect data from distance. 

## Step by step demos in the course walk you through the learning process.


### KnowFlow Hardware 

A complete list of components, measurements, drawings, and other specifications can be found [here](mettre le lien).

#### Central Control Unit: 
* Arduino Uno
* Expansion Shield (DFRobot Expansion Shield V7.1)  
* real time clock circuit board
* 16 Bit ADC module V1.0
#### Water Sensors: 
* Temperature (temperature probe and Terminal sensor adapter V2.0)
* 2nd temperature Sensor (DS18B20 temperature sensor V2)
* pH (pH probe and pH circuit board)
* Electrical Conductivity (EC probe, EC circuit board and Anolog isolation module)
* Turbidity (turbity probe and turbidity circuit board)
* Dissolved Oxygen (DO probe, circuit board and Anolog isolation module)
#### Data Storage:
* Micro-SD module
* Micro SD card
#### Fit and finish:
* Water proof box(25cm*20cm*9.5cm)
* Heat Shrink Tubing (For the turbidity sensor)
#### Other parts: 
* Wires
* Bolts, nuts and Screws
* Battery and micro USB cable (to connect with the arduino Uno)
* heat gun

### Installing KnowFlow software
You don’t need to have experience with Arduino or software development. 
KnowFlow is packaged wtth supporting software libraries to make it easier for you to enable different sensor features 
for your application. Feel free to post your software questions on our wiki page on public lab or github.

1. Download Arduino IDE
1. Download the code from [github]
1. Open "WaterMonitor.ino" from the downloaded file with Arduino IDE
1. Connect your Arduino Uno board
1. Select Tools>Board: Arduino Uno and Tools>Ports
1. Click "Verify" then "Upload" the software to your board.



### Sensor building: water quality monitoring 

We need to connect all the sensors, MicroSD card module to the Arduino, fixed it to  the waterproof box and then connect to the power supply.

#### 1. Connect all the modules to the Arduino expansion (I/O Expansion Shield V7.1)

For the following step, the connection to the Arduino expansion the color should be matched.

##### 16-bit ADC module 
Plug the white end of the "red-black-blue-green" cable to the 16-bit ADC module and the black end to the blue I2C interface of the Expansion Shield : 
  -  red : 5V 
  -  black : GND
  -  blue : SCL
  -  green : SDA

##### Temperature 
Plug the white end of the "green-red-black" cable to the Plugable Terminal V2 (Terminal sensor adapter) and the black end to Digital Port D3 (Expansion Shield). 
Plug the "yellow-red-black" cable to the grey end of the temperature sensor adapter : 
  - Yellow : A
  - Red : B
  - Black : C
 
Add the second temperature sensor. Plug the white end of the "green-red-black" cable to the DS18B20 temperature sensor and the black end to Digital Port D5 (Expansion Shield). 

##### pH
Plug the white end of the "blue-red-black" cable to the pH circuit board and the black end to Analog port A1 (Expansion Shield).

##### Electrical Conductivity
Plug the "orange-red-brown" cable to the EC circuit board and to the SEN end of the isolation module. 
Plug the white end of the "blue-red-black" cable to the MCU of the isolation module, the black end to the Analog port A1 of the 16-bit ADC module. 

##### Turbidity
The cable sensor does not have the same color connection at both ends. Plug the "red-yellow-black" end of the cable to the sensor and the "yellow-black-red" end to the Turbidity circuit board.  
Plug the white end of the "blue-red-black" cable to the Turbidity circuit board and the black end to the Analog port A0 of the 16-bit ADC module. 

##### dissolved oxygen
Plug the "orange-red-brown" cable to the EC circuit board and to the SEN end of the isolation module. 
Plug the white end of the "blue-red-black" cable to the MCU of the isolation module, the black end to the Analog port A2 of the 16-bit ADC module. 

##### Real Time Clock and SD card
Plug the white end of the "red-black-blue-green" cable to the RTC module, and the black end to the blue I2C interface of the expansion shield (same color matching as for the 16-bit ADC module). 

##### All the sensors are connected now.
