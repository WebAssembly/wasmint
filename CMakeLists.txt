cmake_minimum_required(VERSION 2.8)
project(wasmint)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_subdirectory(wasm-module)

####################
# Common code      #
####################

include_directories(wasm-module/src)
include_directories (src)

add_library(core
    src/interpreter/Thread.cpp
    src/interpreter/FunctionState.cpp
    src/interpreter/InstructionState.cpp
    src/interpreter/MachineState.cpp
    src/interpreter/InstructionExecutor.cpp
    src/interpreter/Heap.cpp
    src/interpreter/StepResult.cpp
    src/builtins/StdioModule.cpp
)


####################
# interpreter      #
####################

add_executable(wasmint main.cpp)

target_link_libraries(wasmint core wasm-module)


####################
# Tests            #
####################
enable_testing()

file(GLOB TEST_FILES "tests/*Test.cpp")
foreach(TEST_FILE ${TEST_FILES})
  get_filename_component(BASENAME ${TEST_FILE} NAME_WE)
  add_executable(${BASENAME} ${TEST_FILE})
  target_link_libraries(${BASENAME} core wasm-module)
  add_test(${BASENAME} ${BASENAME})
endforeach()

file(GLOB TEST_WAST_FILES "tests/wast_tests/*.wast")
foreach(TEST_WAST_FILE ${TEST_WAST_FILES})
    get_filename_component(BASENAME ${TEST_WAST_FILE} NAME)
    add_test(NAME ${BASENAME} COMMAND ./wasmint ${TEST_WAST_FILE})
endforeach()

