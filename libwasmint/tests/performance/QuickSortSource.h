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


#ifndef WASMINT_QUICKSORTSOURCE_H
#define WASMINT_QUICKSORTSOURCE_H

#include "PiDigits.h"

const std::string quickSortSource =
        "module \n"
        "    (memory " + std::to_string((sizeof WASMINT_PI_DIGITS_STR)) + " " + std::to_string((sizeof WASMINT_PI_DIGITS_STR)) + " (segment 0 \""
        WASMINT_PI_DIGITS_STR
                "\"))\n"
        "    (func $swap (param $a i32) (param $b i32)\n"
        "        (local $tmp i32)\n"
        "        (set_local $tmp (i32.load8_u (get_local $a)))\n"
        "        (i32.store8 (get_local $a) (i32.load8_u (get_local $b)))\n"
        "        (i32.store8 (get_local $b) (get_local $tmp))\n"
        "    )\n"
        "    (func $quicksort (param $begin i32) (param $end i32)\n"
        "        (local $ptr i32)\n"
        "        (local $split i32)\n"
        "        (if (i32.gt_u (i32.sub (get_local $end) (get_local $begin)) (i32.const 1))\n"
        "            (\n"
        "                (set_local $ptr (get_local $begin))\n"
        "                (set_local $split (i32.add (get_local $begin) (i32.const 1)))\n"
        "                (loop $exit $cont\n"
        "                    (set_local $ptr (i32.add (get_local $ptr) (i32.const 1)))\n"
        "                    (if (i32.gt_u (get_local $ptr) (get_local $end))\n"
        "                        (br $exit)\n"
        "                    )\n"
        "                    (if (i32.lt_u (i32.load8_u (get_local $ptr)) (i32.load8_u (get_local $begin)))\n"
        "                        (\n"
        "                            (call $swap (get_local $ptr) (get_local $split))\n"
        "                            (set_local $split (i32.add (get_local $split) (i32.const 1)))\n"
        "                        )\n"
        "                    )\n"
        "                    (br $cont)\n"
        "                )\n" // end loop
        "                (call $swap (get_local $begin) (i32.sub (get_local $split) (i32.const 1)))\n"
        "                (call $quicksort (get_local $begin) (i32.sub (get_local $split) (i32.const 1)))\n"
        "                (call $quicksort (get_local $split) (get_local $end))\n"
        "            )\n" // end if block
        "        )\n" // end if
        "    )\n" // end quicksort func
        "    (func main\n"
        "        (call $quicksort (i32.const 0) (i32.const " + std::to_string((sizeof WASMINT_PI_DIGITS_STR) - 1) + "))\n"
        "    )\n";

#endif //WASMINT_QUICKSORTSOURCE_H
