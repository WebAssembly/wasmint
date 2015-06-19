cmake_minimum_required(VERSION 2.8)
project(wasmint)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

####################
# Common code      #
####################

include_directories (src)

add_library(core
    main.cpp
    src/parsing/ByteStream.cpp
    src/Module.cpp
    src/Section.cpp
    src/OpcodeTable.cpp
    src/parsing/ModuleParser.cpp
    src/parsing/SectionParser.cpp
    src/parsing/OpcodeTableParser.cpp
)


####################
# interpreter      #
####################

add_executable(wasmint main.cpp)

target_link_libraries(wasmint core)


####################
# Tests            #
####################
enable_testing()

file(GLOB TEST_FILES "tests/*Test.cpp")
foreach(TEST_FILE ${TEST_FILES})
  get_filename_component(BASENAME ${TEST_FILE} NAME_WE)
  add_executable(${BASENAME} ${TEST_FILE})
  target_link_libraries(${BASENAME} core)
  add_test(${BASENAME} ${BASENAME})
endforeach()