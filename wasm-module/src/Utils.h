#ifndef WASMINT_UTILS_H
#define WASMINT_UTILS_H

#include <string>
#include <sstream>
#include "ExceptionWithMessage.h"

namespace wasm_module {

    ExceptionMessage(InvalidNumberLiteral)

    class Utils {

    public:
        static bool hasDollarPrefix(const std::string& s) {
            if (s.empty())
                return false;
            return s[0] == '$';
        }

        static std::size_t strToSizeT(const std::string& str) {
            // TODO check for overflow

            if (!isDecNumber(str, false)) {
                throw InvalidNumberLiteral(str);
            }

            std::istringstream iss(str);
            size_t result;
            iss >> result;

            if (std::to_string(result) != str) {
                throw InvalidNumberLiteral(str);
            }

            return result;
        }

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

        static bool isDecChar(char c) {
            if (c >= '0' && c <= '9')
                return true;
            return false;
        }

        static bool isHexChar(char c) {
            if (c >= '0' && c <= '9')
                return true;
            if (c >= 'a' && c <= 'f')
                return true;
            if (c >= 'A' && c <= 'F')
                return true;
            return false;
        }

        static bool isHexNumber(const std::string& str, std::size_t maxDigits) {
            if (str.size() < 2)
                return false;
            std::size_t startPos = 0;
            if (str.at(0) == '-') {
                startPos = 1;
            }
            if (str.at(startPos) != '0') {
                return false;
            }
            startPos++;
            if (str.at(startPos) != 'x' && str.at(startPos) != 'X') {
                return false;
            }
            startPos++;
            if (startPos >= str.size())
                return false;
            if (str.size() - startPos > maxDigits) {
                return false;
            }
            for (; startPos < str.size(); startPos++) {
                if (!isHexChar(str.at(startPos))) {
                    return false;
                }
            }
            return true;
        }

        static bool isDecNumber(const std::string& str, bool allowSigned = true) {
            if (str.empty())
                return false;
            std::size_t pos = 0;
            if (allowSigned) {
                if (str.at(pos) == '-') {
                    pos++;
                }
            }
            for (; pos < str.size(); pos++) {
                if (!isDecChar(str.at(pos))) {
                    return false;
                }
            }
            return true;
        }

        template <class vector>
        static bool comparePtrVector(const vector& a, const vector& b) {
            if (a.size() != b.size())
                return false;
            for (auto& element : a) {
                bool found = false;
                for (auto& otherElement : b) {
                    if ((*element) == (*otherElement)) {
                        found = true;
                        break;
                    }
                }
                if (!found)
                    return false;
            }
            return true;
        }

        template <class vector>
        static bool compareVector(const vector& a, const vector& b) {
            if (a.size() != b.size())
                return false;
            for (auto& element : a) {
                bool found = false;
                for (auto& otherElement : b) {
                    if (element == otherElement) {
                        found = true;
                        break;
                    }
                }
                if (!found)
                    return false;
            }
            return true;
        }

        template <class map>
        static bool compareMaps(const map& a, const map& b) {
            return a.size() == b.size()
                   && std::equal(a.begin(), a.end(),
                                 b.begin());
        }

    };
}

#endif //WASMINT_UTILS_H
