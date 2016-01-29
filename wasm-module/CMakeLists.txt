cmake_minimum_required(VERSION 2.8)
project(wasm-module)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11")

####################
# Common code      #
####################

include_directories (src)

add_library(wasm-module
        src/ExceptionWithMessage.cpp
        src/Module.cpp
        src/FunctionContext.cpp
        src/FunctionSignature.cpp
        src/FunctionTable.cpp
        src/OpcodeTable.cpp
        src/Variable.cpp
        src/Function.cpp
        src/types/Type.cpp
        src/TypeTable.cpp
        src/ModuleContext.cpp
        src/HeapData.cpp
        src/HeapSegment.cpp
        src/SafeAddition.cpp
        src/FunctionTypeTable.cpp
        src/FunctionType.cpp

        src/branching/BranchInformation.cpp

        src/instructions/InstructionSet.cpp
        src/instructions/Instruction.cpp
        src/instructions/InstructionId.cpp
        src/instructions/Instructions.cpp
        src/instructions/UnreachableValidator.cpp
        src/instructions/InstructionAddress.cpp

        src/types/Int32.cpp
        src/types/Int64.cpp
        src/types/Void.cpp
        src/types/Float32.cpp
        src/types/Float64.cpp

        src/binary_parsing/ByteStream.cpp
        src/binary_parsing/FunctionParser.cpp
        src/binary_parsing/FunctionTableParser.cpp
        src/binary_parsing/TypeTableParser.cpp
        src/binary_parsing/ModuleParser.cpp
        src/binary_parsing/CodeSectionParser.cpp
        src/binary_parsing/OpcodeTableParser.cpp

        src/sexpr_parsing/SExprParser.cpp
        src/sexpr_parsing/CharacterStream.cpp
        src/sexpr_parsing/SExpr.cpp
        src/sexpr_parsing/ModuleParser.cpp
        src/sexpr_parsing/FunctionParser.cpp
        src/sexpr_parsing/Types.cpp
        src/sexpr_parsing/InstructionParser.cpp
        src/branching/BranchTypeValidator.cpp src/branching/BranchTypeValidator.h src/ModuleLoader.cpp src/ModuleLoader.h src/sexpr_serializing/ModuleSerializer.cpp src/sexpr_serializing/ModuleSerializer.h)


####################
# Tests            #
####################
enable_testing()

file(GLOB TEST_FILES "tests/*Test.cpp")
foreach(TEST_FILE ${TEST_FILES})
  get_filename_component(BASENAME ${TEST_FILE} NAME_WE)
  add_executable(${BASENAME} ${TEST_FILE})
  target_link_libraries(${BASENAME} wasm-module)
  add_test(${BASENAME} ${BASENAME})
endforeach()