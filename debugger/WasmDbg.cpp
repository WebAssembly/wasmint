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


#include "WasmDbg.h"

#include <interpreter/heap/Heap.h>
#include <Module.h>
#include <sexpr_parsing/ModuleParser.h>
#include "MemoryViewer.h"
#include "InstructionViewer.h"
#include "VMViewer.h"


WINDOW* WasmDbg::mainwin;
WasmDbg* WasmDbg::instance_;

WasmDbg::WasmDbg() {
    instance_ = this;
}

void WasmDbg::initCurses() {
    /*  Initialize ncurses  */
    if ( (mainwin = initscr()) == NULL ) {
        fprintf(stderr, "Error initializing ncurses.\n");
        exit(EXIT_FAILURE);
    }

    curs_set(0);
    noecho();                  /*  Turn off key echoing                 */
    keypad(mainwin, TRUE);     /*  Enable the keypad for non-char keys  */

}

void WasmDbg::deinitCurses() {

    /*  Clean up after ourselves  */

    delwin(mainwin);
    endwin();
    refresh();

}

void WasmDbg::run() {
    int h, w;
    getmaxyx(mainwin, h, w);
    vmViewer = new VMViewer(w, h);

    int ch = 0;

    do {
        erase();
        vmViewer->doHandleCharacter(ch);
        vmViewer->draw();
        doupdate();
    } while ( (ch = getch()) != 'q' );
}

WasmDbg::~WasmDbg() {
    delete vmViewer;
}