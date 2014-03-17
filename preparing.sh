#!/bin/bash

# Testen ob lupdate auf dem System vorhanden ist.
progname=$(which lupdate)
if [ $? = 0 ]; then
    echo
else
    kdialog --title "preparing.sh" --msgbox "lupdate nicht auf System vorhanden. lupdate ist bestandteil von Programmpacket libqt4-devel"
    echo "[preparing.sh] lupdate nicht auf System vorhanden"
    echo "[preparing.sh] lupdate ist bestandteil von Programmpacket libqt4-devel"
    exit 1
fi

# Testen ob tx auf dem System vorhanden ist.
progname=$(which tx)
if [ $? = 0 ]; then
    echo
else
    kdialog --title "preparing.sh" --msgbox "tx nicht auf System vorhanden. tx ist bestandteil von Programmpacket transifex-client"
    echo "[preparing.sh] tx nicht auf System vorhanden"
    echo "[preparing.sh] tx ist bestandteil von Programmpacket transifex-client"
    exit 1
fi

# Funktioniert noch nicht so wie ich will
# Testen ob ~/.transifexrc im home vorhanden ist.
#$file="/home/vk/.transifexrc"
#if [ -f $file ]; then
#    echo "[preparing.sh] ~/.transifexrc nicht auf System vorhanden"
#    echo "[preparing.sh] ~/.transifexrcenthält den transifex Zugang"
#    kdialog --title "preparing.sh" --msgbox "~/.transifexrc nicht auf System vorhanden. ~/.transifexrc enthält den transifex Zugang"
#    exit 1   
#fi

echo ==================
echo Remove ffmpeg logs
echo ==================
rm *.log

echo ============
echo Make main.ts
echo ============
lupdate -locations none -recursive ./ -ts main.ts

echo ======================
echo Download language pack
echo ======================
tx pull -a

echo =========================
echo Remove old screencast.qrc
echo =========================
rm screencast.qrc

echo =====================
echo Create screencast.qrc
echo =====================
##########
# Pictures
##########
echo '<!DOCTYPE RCC><RCC version="1.0">' > screencast.qrc
echo "<qresource>" >> screencast.qrc
array='<file>'$( ls -al pictures/*.png | sed 's/ \+/ /g' | cut -d " " -f 9 | sed 's/ \+/ /g')'</file>'
# ":" is separatorr
echo $array | sed 's: :</file>\n<file>:g' >> screencast.qrc

################
# language packs
################
array='<file>'$( ls -al language/*.ts | sed 's/ \+/ /g' | cut -d " " -f 9 | sed 's/ \+/ /g')'</file>'
# ":" is separatorr
echo $array | sed 's: :</file>\n<file>:g' >> screencast.qrc
sed -i 's/.ts/.qm/g' screencast.qrc

#########
# CREDITS
#########
echo '<file>CREDITS</file>' >> screencast.qrc

#########
# VERSION
#########
echo '<file>VERSION</file>' >> screencast.qrc
echo '</qresource>' >> screencast.qrc
echo '</RCC>'  >> screencast.qrc


echo ==========
echo Script end
echo ==========
