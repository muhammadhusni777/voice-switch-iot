######  PROGRAM MEMANGGIL WINDOWS PYQT5 ##########################

####### memanggil library PyQt5 ##################################
#----------------------------------------------------------------#
from PyQt5.QtCore import * 
from PyQt5.QtGui import * 
from PyQt5.QtQml import * 
from PyQt5.QtWidgets import *
from PyQt5.QtQuick import *  
import sys
import paho.mqtt.client as paho


broker="127.0.0.1"
#broker="mqtt.ardumeka.com"#"broker.emqx.io"
#port = 11219
port = 1883
topic_test = ""
button = ""

import speech_recognition as sr

r = sr.Recognizer()

print(sr.Microphone.list_microphone_names())

text = ""
words = ""

#----------------------------------------------------------------#


########## mengisi class table dengan instruksi pyqt5#############
#----------------------------------------------------------------#
class table(QObject):    
    def __init__(self, parent = None):
        super().__init__(parent)
        self.app = QApplication(sys.argv)
        self.engine = QQmlApplicationEngine(self)
        self.engine.rootContext().setContextProperty("backend", self)    
        self.engine.load(QUrl("main.qml"))
        sys.exit(self.app.exec_())
    
    @pyqtSlot(str)
    def button1(self, message):
        global button1_status
        print(message)
        button1_status = message
        client.publish("led",str(message))
        
    @pyqtSlot(str)
    def button(self, message):
        global words
        global text
        print(message)
        with sr.Microphone() as source:
            print('speak please:')
            audio = r.listen(source)
            # r.pause_threshold = 0.4
            r.energy_threshold = 1000
            r.duration = 3
            
        try:
            text = r.recognize_google(audio, language = "ID")
            words = text
            print(words)
            if (words == "nyalakan lampu"):
                client.publish("lights", "ON")
                
            if (words == "matikan lampu"):
                client.publish("lights", "OFF")
            
        except:
            text = "ulangi"
            words = ""
        
        print("sound finish")
    
    
        
    
    @pyqtSlot(result=str)
    def test_message(self):  return text
#----------------------------------------------------------------#


def on_message(client, userdata, message):
    msg = str(message.payload.decode("utf-8"))
    t = str(message.topic)

    if(msg[0] == 'c'):
        val =  1
    else:
        val = (msg)
    
    if (t == "sensor"):
        global topic_test
        topic_test = (msg)
        print(topic_test)
        



########## memanggil class table di mainloop######################
#----------------------------------------------------------------#    
if __name__ == "__main__":
    client= paho.Client("PC_1")
    client.on_message=on_message

    print("connecting to broker ",broker)
    client.connect(broker,port)#connect
    print(broker," connected")
    
    client.loop_start()
    print("Subscribing")

    client.subscribe("sensor")
    
    main = table()
    
#----------------------------------------------------------------#