# Register a device(=sensor) on the cloud the things network (TTN) console


## Create an account in things network cloud :

```https://eu1.cloud.thethings.network/console/```


## Create application : 

- Go to application and create application
  
- write the application identifier (a name chosen for the gateway)
  


## Configure devices at the console

- Go to the application previously created
  
- Register end device
  
- Select input method: "Enter end device specifics manually".
  
- Frequency plan: Europe 863-870 MHz (SF9 for RX2 - recommended)
  
- LoRawan version: LoRaWAN specification 1.0.0
  
- Click on "show advanced activation, lorawan class and cluster settings".
  
- Activation mode: Activation by personalization (ABP)
  
- Provisioning information > Dev EUI > click on generate
  
- Device address > click on generate
  
- appSKey > click on generate
  
- NwkSKey > click on generate
  
- Register end device
  
- in general settings > Networks layer > click on expand
  
- Click on advanced mac settings > check resets frame counters
  
