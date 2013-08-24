#!/bin/bash
 
find . -name '*.cpp' -type f -print0 | xargs -0 cat | wc -l
find . -name '*.h' -type f -print0 | xargs -0 cat | wc -l
