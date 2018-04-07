#!/bin/bash 

# Hint:
# The Part 1 you need only if you want start vokoscreen from a script.


# ********** Part 1 **********

# Please adjust path to vokoscreen

# Normally vokoscreen have many output, in this script we want this not
# and redirect with "> 1&>/dev/null". On end vokoscreen start with "&" in the background
./vokoscreen > 1&>/dev/null &


# We wait min. 1 second until vokoscreen started and logged in to DBus.
# Normally this is very fast, but better we wait and have no error.
sleep 1


# Now we wait to vokoscreen until it full loaded
# Wy full loaded?
# Vokoscreen search Audio, Video and other devices this takes time.
value=1
while [ $value -eq 1 ]
do
   ./vokoscreen isVokoscreenLoaded
   rc=$?
   if [ $rc -eq 0 ]; then
     echo "[SCRIPT] vokoscreen has loaded everything"
     value=0
     sleep 1
   else
     echo "[SCRIPT] vokoscreen not finish loaded"
     sleep 1
   fi
done

# ********** Part 2 **********
