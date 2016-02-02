#!/usr/bin/env bash

fail="0"

for i in "${CMAKE_SOURCE_DIR}"/tools/halting_problem/infinite_tests/*.wasm ; do
    echo "##########################################################"
    echo "Running test $i"

    ./does_halt "$i"
    code="$?"

    if [ $code -eq 0 ]; then
        echo "Program stopped (but should run endlessly) $i"
        fail="1"
    elif [ $code -eq 1 ]; then
        : # successfully detected that the program doesn't halt
    else
        echo "Unknown problem with $i"
        fail="1"
    fi
done

for i in "${CMAKE_SOURCE_DIR}"/tools/halting_problem/non_infinite_tests/*.wasm ; do
    echo "##########################################################"
    echo "Running test $i"

    ./does_halt "$i"
    code="$?"

    if [ $code -eq 1 ]; then
        echo "Program detected to run endlessly but actually stops $i"
        fail="1"
    elif [ $code -eq 0 ]; then
        : # successfully detected that the program halts
    else
        echo "Unknown problem with $i"
        fail="1"
    fi
done

for i in "${CMAKE_SOURCE_DIR}"/tools/halting_problem/undecideable_tests/*.wasm ; do
    echo "##########################################################"
    echo "Running test $i"

    ./does_halt "$i"
    code="$?"

    if [ $code -eq 1 ]; then
        echo "Program detected to run endlessly but program is undecideable $i"
        fail="1"
    elif [ $code -eq 0 ]; then
        echo "Program detected to terminte but program is undecideable $i"
    elif [ $code -eq 3 ]; then
        : # successfully detected that the program cant be decided
    else
        echo "Unknown problem with $i"
        fail="1"
    fi
done

if [ $fail -eq 1 ]; then
    exit 1
fi