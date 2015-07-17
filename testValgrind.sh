#!/bin/bash

for i in *Test; do
    valgrind --error-exitcode=1 --leak-check=full ./$i
done