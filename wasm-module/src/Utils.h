#ifndef WASMINT_UTILS_H
#define WASMINT_UTILS_H

#include <string>
#include <sstream>
#include <regex>
#include "ExceptionWithMessage.h"

namespace wasm_module {

    ExceptionMessage(InvalidNumberLiteral)

    class Utils {

        static const std::regex decNumber;

    public:
        static bool hasDollarPrefix(const std::string& s) {
            if (s.empty())
                return false;
            return s[0] == '$';
        }

        static std::size_t strToSizeT(const std::string& str);

        static uint32_t reinterpretFloatAsInt(float value) {
            uint32_t* result = (uint32_t *) &value;
            return *result;
        }

        static uint64_t reinterpretDoubleAsInt(double value) {
            uint64_t* result = (uint64_t *) &value;
            return *result;
        }

        static float reinterpretIntAsFloat(uint32_t value) {
            float* result = (float *) &value;
            return *result;
        }

        static double reinterpretIntAsDouble(uint64_t value) {
            double* result = (double *) &value;
            return *result;

        }
    };
}

#endif //WASMINT_UTILS_H
