#!/bin/bash

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
echo '</qresource>' >> screencast.qrc
echo '</RCC>'  >> screencast.qrc

echo ==========
echo Script end
echo ==========
