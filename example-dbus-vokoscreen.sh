#!/bin/bash 

# Hint:
# The Part 1 you need only if you want start vokoscreen from a script.


# ********** Part 1 **********

# Please adjust path to vokoscreen

# Normally vokoscreen have many output, in this script we want this not
# and redirect with "2>/dev/null". On end vokoscreen start with "&" in the background
./vokoscreen 2>/dev/null &


# We wait until vokoscreen started and logged in to DBus.
# Normally this is very fast, but better we wait and have no error.
rc=$(ps -A | grep vokoscreen)
rc=$(echo $rc | rev | cut -c 1)
while [ $rc = "" ]
do
   echo "[SCRIPT] vokoscreen not loaded"
   sleep 1
done


# Now we wait to vokoscreen until it full loaded
# Wy full loaded?
# Vokoscreen search Audio, Video and other devices this takes time.
value="1"
while [ $value -eq "1" ]
do
   rc=$(./vokoscreen isVokoscreenLoaded)
   if [ "$rc" = "0" ]; then
     echo "[SCRIPT] vokoscreen has all devices loaded"
     value=0
     sleep 1
   else
     echo "[SCRIPT] vokoscreen has not devices finish loaded"
     sleep 1
   fi
done


# ********** Part 2 **********

# If you want show all methods this can do
./vokoscreen showAllMethods
echo
./vokoscreen magnifierOn
sleep 3
./vokoscreen MagnifierOff
sleep 3
./vokoscreen Area
sleep 6
./vokoscreen AreaReset
sleep 3
./vokoscreen Window
sleep 3
./vokoscreen FullScreen
sleep 3

