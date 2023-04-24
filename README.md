

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
* Bolts and nuts
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

#### Connect all the modules to the Arduino expansion (I/O Expansion Shield V7.1)

For the following step, the connection to the Arduino expansion the color should be matched.

##### 16-bit ADC module 
Plug the white end of the "red-black-blue-green" cable to the 16-bit ADC module and the black end to the blue I2C interface of the Expansion Shield : 
  -  red : 5V 
  -  black : GND
  -  blue : SCL
  -  green : SDA
![](https://user-images.githubusercontent.com/63862628/234029967-c6534c24-5e5d-4007-82e3-0702aa023548.jpg)
Then put the I2C Address (ADDR on 0x49).

##### 1. Temperature 
Plug the white end of the "green-red-black" cable to the Plugable Terminal V2 (Terminal sensor adapter) and the black end to Digital Port D3 (Expansion Shield). 
Plug the "yellow-red-black" cable to the grey end of the temperature sensor adapter : 
  - Yellow : A
  - Red : B
  - Black : C
 ![](https://user-images.githubusercontent.com/63862628/234031650-c3e43a25-60cc-4ca2-acc7-0f4334faef03.jpg)
Add the second temperature sensor. Plug the white end of the "green-red-black" cable to the DS18B20 temperature sensor and the black end to Digital Port D5 (Expansion Shield). 

##### 2. pH
Plug the white end of the "blue-red-black" cable to the pH circuit board and the black end to Analog port A1 (Expansion Shield).

##### 3. Electrical Conductivity
Plug the "orange-red-brown" cable to the EC circuit board and to the SEN end of the isolation module. 
Plug the white end of the "blue-red-black" cable to the MCU of the isolation module, the black end to the Analog port A1 of the 16-bit ADC module. 

##### 4. Turbidity
The cable sensor does not have the same color connection at both ends. 
Remove the black cap from the sensor. Plug the "red-yellow-black" end of the cable to the sensor. Add the heat shrink tubing and heat with the heat gun to make the sensor waterproof.  
 ![](https://user-images.githubusercontent.com/63862628/234028698-d0f3633c-0cc3-4f47-aeeb-a31a3aff885b.jpg)
 
Plug the white end of the "blue-red-black" cable to the Turbidity circuit board and the black end to the Analog port A0 of the 16-bit ADC module. 

##### 5. dissolved oxygen
Plug the "orange-red-brown" cable to the EC circuit board and to the SEN end of the isolation module. 
Plug the white end of the "blue-red-black" cable to the MCU of the isolation module, the black end to the Analog port A2 of the 16-bit ADC module. 

##### Real Time Clock and SD card
Plug the white end of the "red-black-blue-green" cable to the RTC module, and the black end to the blue I2C interface of the expansion shield (same color matching as for the 16-bit ADC module). 
Connect the MicroSD card module to the blue SD card slot and insert the Micro SD card.
 ![](https://user-images.githubusercontent.com/63862628/234029981-3ea9b8c1-d831-4a09-9afb-8a073fbb7051.jpg)
 ![](https://user-images.githubusercontent.com/63862628/234028706-34e0be27-760a-4a07-a9d1-3208af13e2b8.jpg)

##### All the sensors are connected now. Connect the expansion shield to the arduino UNO 



##### Attach the electronic components to the plate
Check the size of the plate in regard to the box and identify the placement of each component on the plate. I used a foam plate.  You can use double-sided adhesive tape or Bolts and nuts to fix each board to the plate. 
Place the plate in the waterproof box. 
Make holes at the inlets of the waterproof box for each sensor and then pass the BNC interface through the hole as shown in the picture below (for the pH, EC and DO sensors). 
 ![](https://user-images.githubusercontent.com/63862628/234028708-197e813f-6773-4ba9-899d-ff0cd1938f83.jpg)
 ![](https://user-images.githubusercontent.com/63862628/234028699-be3eedc5-a5da-4c01-a79a-1a5ff92c0a64.jpg)

For the tubidity sensor, make a small opening at the level of the entrance of the sensor on the box and introduce the "yellow-black-red" cable like in the picture. Then plug the "yellow-black-red" end to the Turbidity circuit board and put it on analog signal output. 
 ![](https://user-images.githubusercontent.com/63862628/234048848-8ce6cbff-50ea-46e1-a6c1-bd3e152d8aa4.jpg)
 ![](https://user-images.githubusercontent.com/63862628/234036700-a236d5ca-c7c7-47dc-ab5d-fea2ab4b40a8.jpg)
  
Then for the temperature sensors, make a small opening in the water proof box and pull the waterproof part of the sensor out of the box through the hole. 
Connect each probes. 

Now connect the battery to the arduino uno using the micro USB cable. If the light turns on, it means that it works.

###### The circuits is good. Close the box
 ![](https://user-images.githubusercontent.com/63862628/234042985-8cb406f9-b010-46b1-903e-d1c3c4c8d045.jpg)
