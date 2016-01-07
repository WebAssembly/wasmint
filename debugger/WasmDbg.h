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


#ifndef WASMINT_WASMDBG_H
#define WASMINT_WASMDBG_H

#include <stdlib.h>
#include <curses.h>
#include <interpreter/heap/Heap.h>
#include <Module.h>
#include <sexpr_parsing/ModuleParser.h>
#include <interpreter/WasmintVM.h>
#include "MemoryViewer.h"
#include "InstructionViewer.h"
#include "VMViewer.h"

class WasmDbg {

    static WINDOW* mainwin;

    wasmint::WasmintVM vm_;

    VMViewer vmViewer;

public:
    WasmDbg() {
    }


    static void initCurses() {
        /*  Initialize ncurses  */
        if ( (mainwin = initscr()) == NULL ) {
            fprintf(stderr, "Error initializing ncurses.\n");
            exit(EXIT_FAILURE);
        }

        curs_set(0);
        noecho();                  /*  Turn off key echoing                 */
        keypad(mainwin, TRUE);     /*  Enable the keypad for non-char keys  */

    }

    static void deinitCurses() {

        /*  Clean up after ourselves  */

        delwin(mainwin);
        endwin();
        refresh();

    }

    wasmint::WasmintVM& vm() const {

    }

    void run() {
        int ch = 0;

        do {
            erase();

            vmViewer.handleCharacter(ch);
            vmViewer.draw();
            doupdate();
        } while ( (ch = getch()) != 'q' );
    }
};


#endif //WASMINT_WASMDBG_H
