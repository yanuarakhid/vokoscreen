#!/bin/bash

echo ============
echo Make main.ts
echo ============
lupdate -locations none -recursive ./ -ts main.ts

echo ======================
echo Download language pack
echo ======================
tx pull -a

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
echo '</qresource>' >> screencast.qrc
echo '</RCC>'  >> screencast.qrc

echo ==========
echo Script end
echo ==========
