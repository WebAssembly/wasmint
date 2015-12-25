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

#include <iostream>
#include <cstdlib>
#include <climits>
#include <cstdint>
#include "PiDigits.h"
#include <vector>

void swap(char *a, char *b)
{
    char tmp = *a;
    *a = *b;
    *b = tmp;
}

void quicksort(char *begin, char *end)
{
    char *ptr;
    char *split;
    if (end - begin <= 1)
        return;
    ptr = begin;
    split = begin + 1;
    while (++ptr <= end) {
        if (*ptr < *begin) {
            swap(ptr, split);
            ++split;
        }
    }
    swap(begin, split - 1);
    quicksort(begin, split - 1);
    quicksort(split, end);
}

void randomString(std::vector<char>& str) {
    static const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < str.size(); ++i) {
        str[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }
}


void testQsort() {
    std::vector<char> data;
    data.resize(100000);

    quicksort(data.data(), data.data() + data.size());

    char smallestValue = -128;
    for (std::size_t i = 0; i < data.size(); i++) {
        char value = data[i];
        if (value < smallestValue) {
            std::cerr << "Heap unsorted" << std::endl;
            abort();
        }
        smallestValue = value;
    }
}

int main()
{
    for (std::size_t i = 0; i < 10; i++) {
        std::cout << i << std::endl;
        testQsort();
    }

    return 0;
}
