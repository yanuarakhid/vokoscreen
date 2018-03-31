#!/bin/bash 

./vokoscreen&
sleep 10


#exsample with qdbus
qdbus org.vokoscreen.screencast /record org.vokoscreen.screencast.vokoscreenInterface.setWebcamOn

sleep 10

#example with dbus-send
dbus-send --session --type=method_call --dest=org.vokoscreen.screencast /record org.vokoscreen.screencast.vokoscreenInterface.setWebcamOff


#show all method
qdbus org.vokoscreen.screencast /record | grep vokoscreen
