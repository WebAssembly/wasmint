#include "Utils.h"

namespace wasm_module {

    const std::regex Utils::decNumber("[0-9]+");

    std::size_t Utils::strToSizeT(const std::string& str) {
        if (!std::regex_match(str, decNumber)) {
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
}

