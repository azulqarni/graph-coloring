#!/bin/bash

if [[ $1 = "clean" ]]; then
    rm -rf gcolor gcolorpy graphgen in1000.txt
    exit
fi

set +v
echo Compiling...
for file in gcolor gcolorpy graphgen; do
    gcc -Wall -O3 -o $file{,.c} -lm
done
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
