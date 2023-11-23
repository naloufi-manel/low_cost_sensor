
crée une lora single channel gateway (1channel = 1 frequence): 

Utiliser raspberry imager pour graver l'image de raspberry pi 0S (32 bit)

configurer le raspberry pi 

dans interface option (sudo raspi-config) et activer SPI 

########## instaler git 
sudo apt-get install git-core
sudo apt-get update
sudo apt-get upgrade

############ install wiringpi
```git clone https://github.com/WiringPi/WiringPi.git'''

cd WiringPi 
git pull origin
ls
./build

############ TELECHARGER SINGLE CHANNEL GATEWAY 
wget https://github.com/tftelkamp/single_chan_pkt_fwd/archive/master.zip
unzip master.zip
cd single_chan_pkt_fwd-master/

###### modifier le code le la single channel 
nano main.cpp
modifier la ligne 
#define SERVER1  "54.72.145.119" en # define SERVER1 "63.34.215.128"

###########Executer le code
make 
sudo ./single_chan_pkt_fwd (run packet forwarder and get gateway ID)
# une fois executer l'identifiant de la gateway s'affiche 
Gateway ID : b8:27:eb:ff:ff:12:a8:e5 
SF7 : le spreading factor et la frequence 


###########################enregistrer la gateway au niveau de la console du cloud the things network (TTN)
### aller sur le cloud the think network :
https://eu1.cloud.thethings.network/console/

### configuere la gateway : 
dans la rubrique gateway :
- add gateway 
- ecrire l'identifiant de la gateway (un nom choisie pour la gateway)
- remplir Gateway EUI ( represente l'identifiant de la gateway recuperer lors de l'execution du code de la gateway 
- dans loRaWan options -> Frequency plan -> selectionner Europe 863-870 MHz (SF9 for RX2 - recommended) 
- create gateway 


################################### executer un programme au demarage de l'interface graphique du raspberry pi : 
https://trevilly.com/lancement-automatique-dun-script-au-demarrage/

# crée le fichier et changer les droit 
sudo nano /home/pi/test.sh
sudo chmod +x /home/pi/test.sh

# configurer le demarage automatique 
sudo nano /etc/xdg/lxsession/LXDE-pi/autostart

ecrire a la fin du fichier : 
@lxterminal --command="./test.sh"
point-rpi

# pour faire un reboot via le terminal 
