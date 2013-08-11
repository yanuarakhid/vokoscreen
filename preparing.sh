#!/bin/bash

echo ================
echo Erstelle main.ts
echo ================
lupdate -locations none -recursive ./ -ts main.ts

echo =======================
echo Sprachpakete downloaden
echo =======================
tx pull -a

#
# Da die Sprachpakete automatisch gezogen werden,
# wird die screencast.qrc auch automatisch erstellt
#
########
# Bilder
########
echo =======================
echo Erstelle screencast.qrc
echo =======================
echo '<!DOCTYPE RCC><RCC version="1.0">' > screencast.qrc
echo "<qresource>" >> screencast.qrc
array='<file>'$( ls -al pictures/*.png | sed 's/ \+/ /g' | cut -d " " -f 9 | sed 's/ \+/ /g')'</file>'
# Doppelpunkt hier als sed trenner
echo $array | sed 's: :</file>\n<file>:g' >> screencast.qrc

##############
# Sprachpakete
##############
array='<file>'$( ls -al language/*.ts | sed 's/ \+/ /g' | cut -d " " -f 9 | sed 's/ \+/ /g')'</file>'
# Doppelpunkt hier als sed trenner
echo $array | sed 's: :</file>\n<file>:g' >> screencast.qrc
echo '</qresource>' >> screencast.qrc
echo '</RCC>'  >> screencast.qrc

echo ===========
echo Script Ende
echo ===========
