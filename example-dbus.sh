#!/bin/bash 

./vokoscreen&
sleep 10


#exsample with qdbus
qdbus org.vokoscreen.screencast /GUI org.vokoscreenInterface.setWebcamOn
sleep 10

#example with dbus-send
dbus-send --session --type=method_call --dest=org.vokoscreen.screencast /GUI org.vokoscreenInterface.setWebcamOff


#show all method
qdbus org.vokoscreen.screencast /GUI | grep vokoscreen
