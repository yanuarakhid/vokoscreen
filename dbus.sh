#!/bin/bash 

# Please adjust path to vokoscreen
# and start in background
./vokoscreen&

# Wait until vokoscreen is fully loaded
value="1"
while [ $value -eq "1" ]
do
   rc=$(dbus-send --type=method_call --print-reply --dest=org.vokoscreen.screencast /gui org.vokoscreen.gui.isVokoscreenLoaded)
   rc=$(echo $rc | rev | cut -c 1)
   if [ "$rc" = "0" ]; then
     # Enable output in red
     echo -e "\033[31m"
     echo "[SCRIPT] vokoscreen has loaded everything return value $rc"
     # Disable output in normal color
     echo -e "\033[0m"
     value="0"
     sleep 1
   else
     # Enable output in red
     echo -e "\033[31m"
     echo "[SCRIPT] vokoscreen not finish loaded return value $rc"
     # Enable output in normal color
     echo -e "\033[0m"
     sleep 1
   fi
done

# Show all methods supported by vokoscreen
rc=$(dbus-send --type=method_call --print-reply --dest=org.vokoscreen.screencast /gui org.vokoscreen.gui.showAllMethode)
rc=$(echo $rc | cut -d "\"" -f 2)
echo $rc

exit

# Set Tab 0
dbus-send --type=method_call --dest=org.vokoscreen.screencast /gui org.vokoscreen.gui.setTab int32:0
sleep 3

# Set Window
dbus-send --type=method_call --dest=org.vokoscreen.screencast /gui org.vokoscreen.gui.setWindow
sleep 3

# Set Area
setArea=dbus-send --type=method_call --dest=org.vokoscreen.screencast /gui org.vokoscreen.gui.setArea
sleep 3

# Set Fullscreen
dbus-send --type=method_call --dest=org.vokoscreen.screencast /gui org.vokoscreen.gui.setFullScreen
sleep 3

# Set Tab 4 webcam
dbus-send --type=method_call --dest=org.vokoscreen.screencast /gui org.vokoscreen.gui.setTab int32:4
sleep 3

# Start webcam
dbus-send --type=method_call --dest=org.vokoscreen.screencast /gui org.vokoscreen.gui.setWebcamOn
# Show webcam 10 second
sleep 10

# Now we close the webcam
dbus-send --type=method_call --dest=org.vokoscreen.screencast /gui org.vokoscreen.gui.setWebcamOff
sleep 5

# show all method
dbus-send --print-reply --type=method_call --dest=org.vokoscreen.screencast /gui org.freedesktop.DBus.Introspectable.Introspect

# quit vokoscreen
dbus-send --type=method_call --dest=org.vokoscreen.screencast /gui org.vokoscreen.gui.quit

