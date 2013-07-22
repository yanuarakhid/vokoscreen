#!/bin/bash

echo =====================
echo Begin lupdate main.ts
echo =====================
lupdate -locations none -recursive ./ -ts main.ts

echo ====================
echo Begin Transifex pull
echo ====================
tx pull -a

echo ===========
echo Ende Script
echo ===========