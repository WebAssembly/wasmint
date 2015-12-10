#!/bin/bash

memCheckCmd="valgrind --error-exitcode=1 -q"

if [ "$#" -gt 1 ]; then
    echo "Illegal number of parameters"
    exit 1
fi

if [ "$#" -eq 1 ]; then

    if [ "$1" = "--no-memcheck" ]; then
        echo "WARNING: Running without memcheck!"
        memCheckCmd=""
    else
        echo "Unknown parameter: $1"
        exit 1
    fi

fi


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
    { stdout=$($memCheckCmd ./wasmint $i 2> $tmpfile ) ;}

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

echo "######################################"
echo "###  Running negative trap tests   ###"
echo "######################################"


for i in ./negative/trap/*.wasm; do
    #valgrind --error-exitcode=1 -q ./wasmint $i
    tmpfile="$(mktemp)"
    { stdout=$($memCheckCmd ./wasmint $i 2> $tmpfile ) ;}

    if [ $? -eq 0 ]; then
        failedTests=$((failedTests+1))
        printf "[FAIL %50s] " "`basename $i`"
        echo "Full path: `readlink -f $i`"
        echo ""
        echo "$stdout"
        cat $tmpfile
    else
        stderrout=`cat $tmpfile`
        if [[ $stderrout == *"trap"* ]]
        then
            printf "[OK       %50s]\n" "`basename $i`"
        else
            printf "[NO TRAP  %50s]\n" "`basename $i`"
            echo "$stdout"
            cat $tmpfile
        fi
    fi

    totalTests=$((totalTests+1))

    rm $tmpfile
done

echo "######################################"
echo "### Running negative invalid tests ###"
echo "######################################"


for i in ./negative/invalid/*.wasm; do
    #
    tmpfile="$(mktemp)"
    { stdout=$($memCheckCmd  ./wasmint --no-run $i 2> $tmpfile ) ;}

    if [ $? -eq 0 ]; then
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
