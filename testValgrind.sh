#!/bin/bash

set -e

for i in *Test; do
    valgrind --error-exitcode=1 --leak-check=full ./$i
done

for i in ./wasm-module/*Test; do
    valgrind --error-exitcode=1 --leak-check=full ./$i
done

for i in ./libwasmint/tests/wast_tests/*.wasm; do
    valgrind --error-exitcode=1 --leak-check=full ./wasmint $i
done
