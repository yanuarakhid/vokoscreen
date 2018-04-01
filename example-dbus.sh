#!/bin/bash 

# Please adjust path to vokoscreen
# and start in background
./vokoscreen&


value=""
while [ "$value" = "" ]
do
  value=$(qdbus org.vokoscreen.screencast /GUI org.vokoscreenInterface.isVokoscreenLoaded)
  echo "vokoscreen is not finish loaded, value = "$value
  sleep 1
done


# exsample with qdbus
qdbus org.vokoscreen.screencast /GUI org.vokoscreenInterface.setWebcamOn


# Show webcam 5 second
sleep 5


# Now we close the webcam
# example with dbus-send, it is the same as qdbus but an other command
dbus-send --session --type=method_call --dest=org.vokoscreen.screencast /GUI org.vokoscreenInterface.setWebcamOff


# show all method
qdbus org.vokoscreen.screencast /GUI | grep vokoscreen
