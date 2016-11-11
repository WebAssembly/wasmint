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

failedTests="0"

for i in ${CMAKE_SOURCE_DIR}/testsuite/*.wast ; do
    $memCheckCmd ${CMAKE_BINARY_DIR}/wasmint_wast $i

    if [ $? -ne 0 ]; then
        failedTests=$((failedTests+1))
    fi

    totalTests=$((totalTests+1))
done

echo "$failedTests out of $totalTests failed:"

if [ $failedTests -ne 0 ]; then
    exit 1
fi
