
#include <cstdint>
#include <parsing/ByteStream.h>
#include <Module.h>
#include <parsing/ModuleParser.h>

int main() {
    Variable v2;
    {
        Variable v1(Int32::instance());
        Int32::setValue(v1, 2);

        v2 = v1;
    }
    assert(Int32::getValue(v2) == 2);
}
