#!/usr/bin/env bash


for i in ${CMAKE_SOURCE_DIR}/tools/halting_problem/tests/*.wasm ; do
    echo "##########################################################"
    echo "Running test $i"

    ./does_halt $i

    if [ $? -eq 0 ]; then
        echo "Program stopped (but should run endlessly) $i"
    elif [ $? -eq 1 ]; then
        : # successfully detected that the program doesn't halt
    else
        echo "Unknown problem with $i"
        exit 1
    fi
done