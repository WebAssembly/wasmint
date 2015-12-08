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

#ifndef WASMINT_SEXPRPARSER_H
#define WASMINT_SEXPRPARSER_H


#include "CharacterStream.h"
#include "SExpr.h"

namespace wasm_module { namespace sexpr {

    class UnknownDataAtEndOfStream : public std::exception {
    };

    class SExprParser {

        CharacterStream &stream_;

        void parseValues(SExpr &parent, bool allowsEndOfStream);

    public:
        SExprParser(CharacterStream &stream);

        SExpr parse(bool allowExitBeforeEOF = false);

        static SExpr parseString(const std::string& str) {
            CharacterStream stream(str);
            SExprParser parser(stream);
            return parser.parse();
        }
    };

}}

#endif //WASMINT_SEXPRPARSER_H
