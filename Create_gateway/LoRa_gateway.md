
# Create a lora single channel gateway (1channel = 1 frequence)


## Configure raspberry

Use raspberry imager to burn raspberry pi 0S image (32 bit)

Configure raspberry pi 

In interface option (```sudo raspi-config```) and enable SPI 


## Install git 

```sudo apt-get install git-core```

```sudo apt-get update```

```sudo apt-get upgrade```


## Install wiringpi

```git clone https://github.com/WiringPi/WiringPi.git```

```cd WiringPi ```

```git pull origin```

```ls```

```./build```


## Download single channel gateway

```wget https://github.com/tftelkamp/single_chan_pkt_fwd/archive/master.zip```

unzip master.zip

```cd single_chan_pkt_fwd-master/```


##  Modify single channel code 

```nano main.cpp```

Change line : ```#define SERVER1  "54.72.145.119"``` in ```#define SERVER1 "63.34.215.128"```


## Execute code

```make ```

```sudo ./single_chan_pkt_fwd``` (run packet forwarder and get gateway ID)


### Once executed, the gateway ID is displayed 

Gateway ID : b8:27:eb:ff:ff:12:a8:e5 

Spreading factor and frequency will be displayed 



## Register the gateway on the cloud the things network (TTN) console

### Go to cloud the things network :

```https://eu1.cloud.thethings.network/console/```


### Configure gateway : 

#### In the gateway section :

- Add gateway
  
- Write the gateway identifier (a name chosen for the gateway)
  
- Fill in Gateway EUI (represents the gateway identifier retrieved when executing the gateway code)
  
- In loRaWan options -> Frequency plan -> select Europe 863-870 MHz (SF9 for RX2 - recommended)
  
- Click on create gateway 


## Run a program when the raspberry pi GUI is started: 


### Create file and change rights 

```sudo nano /home/pi/gateway.sh```

```sudo chmod +x /home/pi/gateway.sh```


### Configure autostart 

```sudo nano /etc/xdg/lxsession/LXDE-pi/autostart```

#### Adds the lines at the end  : 

```@lxterminal --command="./gateway.sh"```

```dot-rpi```

#### Reboot 

