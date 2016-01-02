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
#include "MemoryViewer.h"
#include "InstructionViewer.h"

class WasmDbg {

    static WINDOW* mainwin;
    wasmint::Heap heap;
    MemoryViewer memoryViewer;
    InstructionViewer instructionViewer;


public:
    WasmDbg() {
        heap.grow(1000);
        heap.setByte(0, 0xFA);

        memoryViewer.setHeap(&heap);

        wasm_module::Module* positiveModule = wasm_module::sexpr::ModuleParser::parse(
                "module (func main "
                        "(if_else (i32.const 0) (unreachable) ())"
                        "(if_else (i32.const 1) (unreachable) ())"
                        "(if_else (i32.const 2) (unreachable) ())"
                        ")");

        instructionViewer.setInstruction(positiveModule->functions().front()->mainInstruction());
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

    void run() {
        int ch = 0;

        do {
            clear();

            memoryViewer.handleCharacter(ch);

            memoryViewer.render();
            memoryViewer.display();

            instructionViewer.handleCharacter(ch);
            instructionViewer.render();
            instructionViewer.display();

            refresh();
        } while ( (ch = getch()) != 'q' );
    }
};


#endif //WASMINT_WASMDBG_H
