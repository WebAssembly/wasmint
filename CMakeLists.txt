cmake_minimum_required(VERSION 2.8)
project(wasmint)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${wasmint_SOURCE_DIR}/cmake")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_subdirectory(wasm-module)

###########################
#       Plugins           #
###########################

####################
#       SDL        #
####################

find_package(SDL2)
find_package(SDL2_Image)

if (SDL2_FOUND AND SDL2_IMAGE_FOUND)
    include_directories(${SDL2_INCLUDE_DIR})
    add_definitions(-DWASMINT_HAS_SDL)
endif()


###########################
#      Common code        #
###########################

include_directories(wasm-module/src)
include_directories (libwasmint)

add_library(libwasmint
    libwasmint/builtins/StdioModule.cpp
    libwasmint/builtins/SDLModule.cpp
    libwasmint/builtins/AssertModule.cpp

    libwasmint/interpreter/Thread.cpp
    libwasmint/interpreter/FunctionState.cpp
    libwasmint/interpreter/InstructionState.cpp
    libwasmint/interpreter/MachineState.cpp
    libwasmint/interpreter/InstructionExecutor.cpp
    libwasmint/interpreter/Heap.cpp
    libwasmint/interpreter/StepResult.cpp
    libwasmint/interpreter/HeapPatch.cpp

    libwasmint/serialization/ByteOutputStream.cpp
    libwasmint/serialization/ByteInputStream.cpp
    libwasmint/serialization/Serializeable.cpp
)


###########################
#      Interpreter        #
###########################

add_executable(wasmint main.cpp)
target_link_libraries(wasmint libwasmint wasm-module)

if (SDL2_FOUND AND SDL2_IMAGE_FOUND)
    target_link_libraries(wasmint ${SDL2_LIBRARY}
                                  ${SDL2_IMAGE_LIBRARIES})
endif()

###########################
#        Tests            #
###########################
enable_testing()


####################
#    Unit tests    #
####################

file(GLOB TEST_FILES "libwasmint/tests/*Test.cpp")
foreach(TEST_FILE ${TEST_FILES})
  get_filename_component(BASENAME ${TEST_FILE} NAME_WE)
  add_executable(${BASENAME} ${TEST_FILE})
  target_link_libraries(${BASENAME} libwasmint wasm-module)
  add_test(${BASENAME} ${BASENAME})
endforeach()

####################
#  Test programs   #
####################

file(GLOB TEST_WAST_FILES "libwasmint/tests/wast_tests/*.wast")
foreach(TEST_WAST_FILE ${TEST_WAST_FILES})
    get_filename_component(BASENAME ${TEST_WAST_FILE} NAME)
    add_test(NAME ${BASENAME} COMMAND ./wasmint ${TEST_WAST_FILE})
endforeach()

####################
#   Fuzzing libs   #
####################

add_library(fuzzing_common
        libwasmint/tests/fuzzing/common/SExprExecutor.cpp
        libwasmint/tests/fuzzing/common/DataRescuer.cpp
        libwasmint/tests/fuzzing/generation_based/SmartSExprGenerator.cpp
)


####################
# Mutation fuzzer  #
####################

add_executable(mutation_fuzzer libwasmint/tests/fuzzing/mutation_based/mutation_fuzzer.cpp)
target_link_libraries(mutation_fuzzer libwasmint wasm-module fuzzing_common)
add_test(NAME mutation_fuzzing COMMAND ./mutation_fuzzer)

####################
# Test suite       #
####################

configure_file(runTestsuite.sh runTestsuite.sh)
configure_file(testValgrind.sh testValgrind.sh COPYONLY)


####################
# testsuite conv   #
####################

find_package(Boost COMPONENTS system filesystem)

if(${Boost_FOUND})
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
    add_executable(testsuite_converter wast-converter/main.cpp)
    target_link_libraries(testsuite_converter wasm-module ${Boost_LIBRARIES})
endif()

###########################
#    Ncurses debugger     #
###########################

find_package(Curses)

if(${Boost_FOUND})
    include_directories(${CURSES_INCLUDE_DIR})

    add_executable(wasmdbg debugger/main.cpp)

    target_link_libraries(wasmdbg libwasmint wasm-module ${CURSES_LIBRARIES})
endif()

###########################
#    Performance tests    #
###########################

add_executable(printQuicksortSource libwasmint/tests/performance/printProgram.cpp)

add_executable(perfQuickSort libwasmint/tests/performance/QuickSortTest.cpp)
target_link_libraries(perfQuickSort libwasmint wasm-module)