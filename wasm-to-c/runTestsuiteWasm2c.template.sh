#!/bin/bash

compile() {
    filename=$(basename "$1")
    ./wasm2c "$1" > "$filename.c"
    if [ $? -ne 0 ]; then
        exit 1
    fi
    gcc -g "$filename.c" -o "$filename.binary" -lm
    if [ $? -ne 0 ]; then
        exit 1
    fi
}

echo "#####################################"
echo "###    Transforming wast files    ###"
echo "#####################################"

./testsuite_converter ${CMAKE_SOURCE_DIR}/testsuite/*.wast

totalTests=0
failedTests=0

echo "######################################"
echo "###    Running positive tests      ###"
echo "######################################"

for i in ./positive/*.wasm; do
    tmpfile="$(mktemp)"
    compile $i
    execname="`basename $i`.binary"
    { stdout=$( "./$execname" 2> $tmpfile ) ;}

    if [ $? -ne 0 ]; then
        failedTests=$((failedTests+1))
        printf "[FAIL %50s] " "`basename $i`"
        echo "Full path: `readlink -f $i`"
        echo ""
        echo "$stdout"
        cat $tmpfile
    else
        printf "[OK       %50s]\n" "`basename $i`"
    fi

    totalTests=$((totalTests+1))

    rm $tmpfile
done

echo "$failedTests out of $totalTests failed:"

if [ $failedTests -ne 0 ]; then
    exit 1
fi