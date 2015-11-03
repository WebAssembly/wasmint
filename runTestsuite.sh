#!/bin/bash

echo "#####################################"
echo "###    Transforming wast files    ###"
echo "#####################################"

./wasm-module/testsuite_converter ${CMAKE_SOURCE_DIR}/testsuite/*.wast

totalTests=0
failedTests=0
FAILEDTESTS=()

echo "#####################################"
echo "###    Running positive tests     ###"
echo "#####################################"

for i in ./positive/*.wasm; do
    #valgrind --error-exitcode=1 -q ./wasmint $i
    { ./wasmint $i ;}
    if [ $? -ne 0 ]; then
        FAILEDTESTS+=("$i")
        failedTests=$((failedTests+1))
        printf "[FAIL %50s]\n" "`basename $i`"
        echo "Full path: `readlink -f $i`"
    else
        printf "[ OK  %50s]\n" "`basename $i`"
    fi

    totalTests=$((totalTests+1))
done

echo "#####################################"
echo "###    Running negative tests     ###"
echo "#####################################"

for i in ./negative/trap/*.wasm; do
    #valgrind --error-exitcode=1 -q ./wasmint $i
    { ./wasmint $i ;} &> /dev/null
    if [ $? -eq 0 ]; then
        FAILEDTESTS+=("$i")
        failedTests=$((failedTests+1))
        printf "[FAIL %50s]\n" "$i"
        echo "Full path: `readlink -f $i`"
    else
        printf "[ OK  %50s]\n" "$i"
    fi

    totalTests=$((totalTests+1))
done

echo "$failedTests out of $totalTests failed:"
echo "$FAILEDTESTS"