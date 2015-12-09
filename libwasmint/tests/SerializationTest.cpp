/*
 * Copyright 2015 WebAssembly Community Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include <cstdint>
#include <Module.h>
#include <serialization/ByteOutputStream.h>
#include <serialization/ByteInputStream.h>
#include <cassert>

using namespace wasm_module;
using namespace wasmint;

int main() {

    uint32_t uint32_testvalue = 0x12345678;
    uint64_t uint64_testvalue = 0x1234567812345678;
    uint32_t int32_testvalue = 0x12345677;
    uint64_t int64_testvalue = 0x1234567712345677;

    Variable varUint32 = Variable::createUInt32(0x98765432);
    Variable varUint64 = Variable::createUInt64(0x9876543298765432);
    Variable varFloat32 = Variable::createFloat32(-345.1f);
    Variable varFloat64 = Variable::createFloat64(-3453.1342);
    Variable varVoid = Variable::Void();
    Variable varBool = Variable::createBool(true);

    std::string str1 = "long test string";
    std::string str2 = "";
    std::string str3 = "FUUUUU";
    str3.push_back(0);
    str3.push_back('a');

    InstructionAddress instructionAddress("module", "function", {0, 4, 44});

    std::vector<uint8_t> testBytes = {2, 45, 34};

    std::vector<uint8_t> memory;

    ByteOutputStream outputStream(memory);
    ByteInputStream inputStream(memory);

    outputStream.writeByte(1);
    outputStream.writeByte(3);
    outputStream.writeUInt32(uint32_testvalue / 2);
    outputStream.writeUInt32(uint32_testvalue);
    outputStream.writeInt32(int32_testvalue);
    outputStream.writeUInt64(uint64_testvalue / 2);
    outputStream.writeUInt64(uint64_testvalue);
    outputStream.writeInt64(int64_testvalue);

    outputStream.writeVariable(varUint32);
    outputStream.writeVariable(varUint64);
    outputStream.writeVariable(varFloat32);
    outputStream.writeVariable(varFloat64);
    outputStream.writeVariable(varVoid);
    outputStream.writeVariable(varBool);

    outputStream.writeStr(str1);
    outputStream.writeStr(str2);
    outputStream.writeStr(str3);

    outputStream.writeBool(true);
    outputStream.writeBool(false);

    outputStream.writeInstructionAddress(instructionAddress);

    outputStream.writeBytes(testBytes);


    assert(inputStream.getByte() == 1);
    assert(inputStream.getByte() == 3);
    assert(inputStream.getUInt32() == uint32_testvalue / 2);
    assert(inputStream.getUInt32() == uint32_testvalue);
    assert(inputStream.getInt32() == int32_testvalue);
    assert(inputStream.getUInt64() == uint64_testvalue / 2);
    assert(inputStream.getUInt64() == uint64_testvalue);
    assert(inputStream.getInt64() == int64_testvalue);

    assert(inputStream.getVariable() == varUint32);
    assert(inputStream.getVariable() == varUint64);
    assert(inputStream.getVariable() == varFloat32);
    assert(inputStream.getVariable() == varFloat64);
    assert(inputStream.getVariable() == varVoid);
    assert(inputStream.getVariable() == varBool);

    assert(inputStream.getString() == str1);
    assert(inputStream.getString() == str2);
    assert(inputStream.getString() == str3);

    assert(inputStream.getBool() == true);
    assert(inputStream.getBool() == false);

    assert(inputStream.getInstructionAddress() == instructionAddress);

    std::vector<uint8_t> loadedBytes = inputStream.getBytes();

    assert(loadedBytes.size() == testBytes.size());
    for (std::size_t i = 0; i < loadedBytes.size(); i++) {
        assert(loadedBytes[i] == testBytes[i]);
    }
}