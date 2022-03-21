#!/bin/bash

len=100
if [[ $1 = "clean" ]]; then
    rm -rf gcolor gcolorpy graphgen in$len.txt
    exit
fi

set -v
echo Compiling...
gcc -Wall -O3 -DATTACH -o gcolor{py,.c} -lm
gcc -Wall -O3 -o gcolor{,.c} -lm
gcc -Wall -O3 -o graphgen{,.c}
echo Done.

echo Producing test input...
./graphgen $len > in$len.txt
echo Done.

echo Testing standalone \(gcolor\)
./gcolor in$len.txt
echo Done.

echo Testing python wrapper
time python3 wrapper.py in$len.txt
echo Done.
