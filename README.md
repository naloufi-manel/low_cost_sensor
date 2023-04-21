

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
#### Water Sensors: 
* Temperature (temperature probe and temperature circuit board)
* pH (pH probe and pH circuit board)
* EC (EC probe, EC circuit board and Anolog isolation module)
* Turbidity (ORP probe and ORP circuit board)
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
* Double-sided adhesive

## Installing KnowFlow software
You don’t need to have experience with Arduino or software development. 
KnowFlow is packaged wtth supporting software libraries to make it easier for you to enable different sensor features 
for your application. Feel free to post your software questions on our wiki page on public lab or github.

1. Download Arduino IDE
1. Download the code from [github]
1. Open "WaterMonitor.ino" from the downloaded file with Arduino IDE
1. Connect your Arduino Uno board
1. Select Tools>Board: Arduino Uno and Tools>Ports
1. Click "Verify" then "Upload" the software to your board.


