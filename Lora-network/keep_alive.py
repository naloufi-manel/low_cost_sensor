# continue leating the server and the code run for ever
# https://www.youtube.com/watch?v=GBwNQImv9Ek
# https://www.youtube.com/watch?v=72HZPng3fhM
# https://uptimerobot.com/dashboard#792828925

from flask import Flask
from threading import Thread

app= Flask('')

@app.route('/')
def home():
    return "I'm alive"

def run():
  app.run(host='0.0.0.0',port=8080)
  #app.run()
  #host ='0.0.0.0', port =0000

def keep_alive():
    t = Thread(target=run)
    t.start()
