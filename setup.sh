#!/bin/bash

if [[ $1 = "clean" ]]; then
    rm -rf gcolor gcolorpy graphgen in1000.txt
    exit
fi

set -v
echo Compiling...
gcc -Wall -O3 -DATTACH -o gcolorpy gcolor.c -lm
gcc -Wall -O3 -o gcolor gcolor.c -lm
gcc -Wall -O3 -o graphgen graphgen.c
echo Done.

echo Producing test input...
./graphgen 1000 > in1000.txt
echo Done.

echo Testing standalone \(gcolor\)
./gcolor in1000.txt
echo Done.

echo Testing python wrapper
time python3 wrapper.py in1000.txt
echo Done.
