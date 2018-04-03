#!/bin/bash 

# exsample qdbus
# qdbus org.vokoscreen.screencast /gui org.vokoscreen.gui.setWebcamOn

# example with dbus-send, it is the same as qdbus but a other command
# dbus-send --session --type=method_call --dest=org.vokoscreen.screencast /gui org.vokoscreen.gui.setWebcamOn


# Please adjust path to vokoscreen
# and start in background
./vokoscreen&


# Now we have to wait until vokoscreen is fully loaded
value=""
while [ "$value" = "" ]
do
  value=$(qdbus org.vokoscreen.screencast /gui org.vokoscreen.gui.isVokoscreenLoaded)
  echo "vokoscreen is not finish loaded, value = "$value
  sleep 1
done


# Start webcam
qdbus org.vokoscreen.screencast /gui org.vokoscreen.gui.setWebcamOn


# Show webcam 5 second
sleep 5


# Now we close the webcam
qdbus org.vokoscreen.screencast /gui org.vokoscreen.gui.setWebcamOff


# show all method
qdbus org.vokoscreen.screencast /gui  | grep vokoscreen
