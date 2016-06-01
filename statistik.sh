#!/bin/bash 

countCommit=6

echo "Eingefügte Zeilen: "$(git log --shortstat -n $countCommit | grep insertions | cut -d "," -f2 | sed 's/^[ \t]*//' | cut -d" " -f1 | paste -sd+ | bc)

echo "Gelöschte Zeilen:  "$(git log --shortstat -n $countCommit | grep deletions  | cut -d "," -f3 | sed 's/^[ \t]*//' | cut -d" " -f1 | paste -sd+ | bc)
