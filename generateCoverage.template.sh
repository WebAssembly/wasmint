#!/bin/bash

ctest . wasm-module
bash runTestsuite.sh --no-memcheck

lcov --rc lcov_branch_coverage=1 -c -i -d ${CMAKE_CURRENT_BINARY_DIR} -o ${CMAKE_CURRENT_BINARY_DIR}/coverage_init.info
lcov --rc lcov_branch_coverage=1 --capture -d ${CMAKE_CURRENT_BINARY_DIR} --output-file ${CMAKE_CURRENT_BINARY_DIR}/coverage.info
lcov --rc lcov_branch_coverage=1 -a ${CMAKE_CURRENT_BINARY_DIR}/coverage_init.info -a ${CMAKE_CURRENT_BINARY_DIR}/coverage.info -o ${CMAKE_CURRENT_BINARY_DIR}/coverage.info

lcov --rc lcov_branch_coverage=1 --remove ${CMAKE_CURRENT_BINARY_DIR}/coverage.info "/usr*" -o ${CMAKE_CURRENT_BINARY_DIR}/coverage.info
lcov --rc lcov_branch_coverage=1 --remove ${CMAKE_CURRENT_BINARY_DIR}/coverage.info "*/libwasmint/tests/*" -o ${CMAKE_CURRENT_BINARY_DIR}/coverage.info

mkdir ${CMAKE_CURRENT_BINARY_DIR}/out
genhtml --branch-coverage ${CMAKE_CURRENT_BINARY_DIR}/coverage.info --output-directory ${CMAKE_CURRENT_BINARY_DIR}/out