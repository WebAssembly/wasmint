#!/bin/bash

set -e

for i in *Test; do
    valgrind --error-exitcode=1 --leak-check=full ./$i
done

for i in ./wast_tests/*.wast; do
    valgrind --error-exitcode=1 --leak-check=full ./wasmint $i
done