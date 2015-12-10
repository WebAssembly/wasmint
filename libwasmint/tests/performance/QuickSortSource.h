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
        "module "
        "    (memory 5000 5000 (segment 0 \""
        WASMINT_PI_DIGITS_STR
                "\"))"
        "    (func $swap (param $a i32) (param $b i32)"
        "        (local $tmp i32)"
        "        (set_local $tmp (i32.load8_u (get_local $a)))"
        "        (i32.store8 (get_local $a) (i32.load8_u (get_local $b)))"
        "        (i32.store8 (get_local $b) (get_local $tmp))"
        "    )"
        "    (func $quicksort (param $begin i32) (param $end i32)"
        "        (local $ptr i32)"
        "        (local $split i32)"
        "        (if (i32.gt_u (i32.sub (get_local $end) (get_local $begin)) (i32.const 1))"
        "            ("
        "                (set_local $ptr (get_local $begin))"
        "                (set_local $split (i32.add (get_local $begin) (i32.const 1)))"
        "                (loop $exit $cont"
        "                    (set_local $ptr (i32.add (get_local $ptr) (i32.const 1)))"
        "                    (if (i32.gt_u (get_local $ptr) (get_local $end))"
        "                        (br $exit)"
        "                    )"
        "                    (if (i32.lt_u (i32.load8_u (get_local $ptr)) (i32.load8_u (get_local $begin)))"
        "                        ("
        "                            (call $swap (get_local $ptr) (get_local $split))"
        "                            (set_local $split (i32.add (get_local $split) (i32.const 1)))"
        "                        )"
        "                    )"
        "                    (br $cont)"
        "                )" // end loop
        "                (call $swap (get_local $begin) (i32.sub (get_local $split) (i32.const 1)))"
        "                (call $quicksort (get_local $begin) (i32.sub (get_local $split) (i32.const 1)))"
        "                (call $quicksort (get_local $split) (get_local $end))"
        "            )" // end if block
        "        )" // end if
        "    )" // end quicksort func
        "    (func main"
        "        (call $quicksort (i32.const 0) (i32.const 4999))"
        "    )";

#endif //WASMINT_QUICKSORTSOURCE_H
