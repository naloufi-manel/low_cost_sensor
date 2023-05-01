#!/usr/bin/env python3
# lien de la video pour le serveur : https://www.google.com/search?q=how+to+execute+a+code+in+the+cloud+for+ever&sxsrf=ALiCzsYN5ZDVmmQFs6FX0mFPbaAXhI7bkQ%3A1659719820702&ei=jFDtYqi0KoWEaYHtqrgH&ved=0ahUKEwioqtuSmrD5AhUFQhoKHYG2CncQ4dUDCA0&uact=5&oq=how+to+execute+a+code+in+the+cloud+for+ever&gs_lcp=Cgdnd3Mtd2l6EAMyBwghEKABEAo6BwgAEEcQsAM6BQghEKABOggIIRAeEBYQHToECCEQFUoECEEYAEoECEYYAFDiB1i4FmDXF2gBcAF4AIABtgGIAawHkgEDNS40mAEAoAEByAEIwAEB&sclient=gws-wiz#kpvalbx=_o1DtYuHDEtGMlwTVj4SwCA18
# cd Bureau/capteur/lora
## execution : python3  storage_data.py

from keep_alive import keep_alive 
import json
import requests
from time import time, sleep
import os
import filecmp 
import shutil
import smtplib
import ssl
import socket
#### install  pyopenssl #  pip install -U pyopenssl # Successfully installed cryptography-37.0.4 pyopenssl-22.0.0

# configuration de la connection a storage de TTN
id_device = "eui-70b3d57ed0051a0b"
APIKey = "NNSXS.PZFW4LAIIPRAXCDZKME6QVGNH6WSLOUNXAP3XQQ.MITEOENWAUD7WTKQTS66I2VA5VOP2E4TQE4VALELYHJOQJILIKTQ"
Fields = "up.uplink_message.decoded_payload,up.uplink_message.frm_payload"
#NumberOfRecords = 30
URL = "https://eu1.cloud.thethings.network/api/v3/as/applications/1testapp/devices/" + id_device + "/packages/storage/uplink_message?order=-received_at&field_mask=" + Fields
Header = { "Accept": "text/event-stream", "Authorization": "Bearer " + APIKey ,'Cache-Control': 'no-cache'}

print("\n\nFetching from data storage...\n")


## Configuration de l'envoie du mail
password = "hcekxxykhoycrkym"    # Your app password goes here
sender = "naloufi.send@gmail.com"    # Your e-mail address
receiver = "naloufi.send@gmail.com" # Recipient's address

server = smtplib.SMTP('smtp.gmail.com',587)
server.ehlo()
server.starttls()
server.ehlo()

server.login(sender,password)

body = """
Attention le capteur ne transmet plus depuis plus de 20 min.
"""

# fonction hashfile 
def hashfile(filename):
	m = hashlib.md5()
	with open(filename, 'rb') as f:
		m.update(f.read())
	return ''.join('%02x' % ord(b) for b in m.digest())


### faire le premier fichier (si le dossier est vide) # recuperer les données 
if len(os.listdir('file')) == 0:
	print("Le répertoire est vide")
	r = requests.get(URL, headers = Header)
	JSON = "{\"data\": [" + r.text.replace("\n\n", ",")[:-1] + "]}";
	f=open("file/file.txt","w")	
	f.write(json.dumps(json.loads(JSON), indent = 4))
	f.close()

keep_alive()
flag_notif=False
no_data=0;
i=0
l=0
while True:
  keep_alive()
  
  try :
    r = requests.get(URL, headers = Header)
  except requests.exceptions.ConnectionError:
    r.status_code = "Connection refused"
  """	 
	# 2eme teste a faire :
	except:
		print("Connection refused by the server..")
		print("Let me sleep")
		time.sleep(5)
		print("continue...")
		continue"""
	# ne marche pas 	
	#except ValueError:
        #	print("Oops!  That was no valid number.  Try again...")

  JSON = "{\"data\": [" + r.text.replace("\n\n", ",")[:-1] + "]}";
  f=open("file/file1.txt","w")	
  f.write(json.dumps(json.loads(JSON), indent = 4))
  f.close()
  F=open('file/file1.txt', 'r') 
  file = open("file/file3.txt", "w")

  j=0 
  for k in F:
	    file.write(k)
	    j=j+1
	    if j==20 :
	       break 
  F.close()
  file.close()

  F1=open('file/file.txt', 'r') 
  file1 = open("file/file4.txt", "w")
  j=0 
  for k in F1:
	    file1.write(k)
	    j=j+1
	    if j==20 :
	       break 
  F1.close()
  file1.close()
  result = filecmp.cmp("file/file4.txt","file/file3.txt", False) 
  print(result) # true si identique 


  if result == False : 
	    flag_notif=False
	    no_data=0
	    #print("test")
	    F2=open('file/file.txt', 'r') 
	    file5 = open("file/file5.txt", "w")
	    for k in F2:
		    file5.write(k)
	    print("ecrire1")
	    F2.close()
	    file5.close()
	    os.remove("file/file.txt")	

	    file2 = open("file/file3.txt", "r")
	    F3=open('file/file.txt', 'w')
	    m=1
	    for k in file2:
		    F3.write(k)
	    F3.close()
	    file2.close()

	    file5 = open("file/file5.txt", "r")
	    F4=open('file/file.txt', 'a')
	    for k in file5:
		    if m>2 :
		    	F4.write(k)
		    m=m+1
	    file5.close()
	    F4.close()
	
  else : 
    no_data=no_data+1 
	
  print("Ok")		
  print(i)
  if i==2880 : # 2880 ## sauvgarde dans archive toute les 24h (avec un nom de fichier en boucle)
    original=r'file/file.txt'
    target=r'archive/file_'+ str(l) +'.txt'	
    shutil.copyfile(original,target)
    i=0
    l=l+1
		#print("sauvgarde")
	
  if (no_data>40 and flag_notif==False): 
		# send a message (si plus de 20 minute sans recuperation de données 
    flag_notif=True
    try :
      server.sendmail(sender, receiver, body)
    except socket.error as e:
      print ("Could not connect to server")
    print("send")

  sleep(30)# sleep(30 - time() % 30)
  i=i+1



 






