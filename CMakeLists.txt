cmake_minimum_required(VERSION 2.8)
project(wasmint)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

####################
# Common code      #
####################

include_directories (src)

add_library(core
    main.cpp
    src/ExceptionWithMessage.cpp
    src/instructions/InstructionSet.cpp
    src/Module.cpp
    src/Section.cpp
    src/FunctionSignature.cpp
    src/FunctionTable.cpp
    src/Instruction.cpp
    src/OpcodeTable.cpp
    src/Variable.cpp
    src/Function.cpp
    src/Type.cpp
    src/TypeTable.cpp
    src/ModuleContext.cpp

    src/interpreter/Environment.cpp
    src/interpreter/Heap.cpp

    src/instructions/I32/I32Add.cpp
    src/instructions/Print.cpp
    src/instructions/GetLocal.cpp
    src/instructions/SetLocal.cpp
    src/instructions/controlflow/Block.cpp
    src/instructions/controlflow/Break.cpp
    src/instructions/controlflow/Continue.cpp
    src/instructions/controlflow/DoWhile.cpp
    src/instructions/controlflow/Forever.cpp
    src/instructions/controlflow/If.cpp
    src/instructions/controlflow/Return.cpp
    src/instructions/FunctionCall.cpp

    src/types/Int32.cpp
    src/types/Int64.cpp
    src/types/Void.cpp
    src/types/Float32.cpp
    src/types/Float64.cpp

    src/parsing/ByteStream.cpp
    src/parsing/FunctionParser.cpp
    src/parsing/FunctionTableParser.cpp
    src/parsing/TypeTableParser.cpp
    src/parsing/ModuleParser.cpp
    src/parsing/CodeSectionParser.cpp
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