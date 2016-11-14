# wasmint
[![Build Status](https://travis-ci.org/WebAssembly/wasmint.svg?branch=master)](https://travis-ci.org/WebAssembly/wasmint)



A embeddable **work-in-progress** WebAssembly (**wasm**) **int**erpreter and debugger licensed under the Apache 2.0 License.

## Navigation

* `wasm-module` will build the wasm-module library that allows parsing/serializing wasm modules.

* `libwasmint` will build the libwasmint library that allows running wasm modules inside a VM. The library will also
   be wrapped in a normal binary called `wasmint` that can be used as a normal wasm interpreter.

* `debugger` will build wasmdbg - a interactive wasm debugger with a curses TUI.

* `wast-converter` can transform the *.wast-files that are used in the testsuite into normal wasm programs.

* `wasm-to-c` compiles wasm modules to C source code.

## Current state

The interpreter still needs to catch up with the latest spec.

## Building

libwasmint, wasmint and wasm-module require a C++11 compiler (including STL) and CMake as build dependencies.

wasmdbg also requires a curses implementation. The wast-converter binary requires boost. You only need to install
those dependencies if you require one of those two projects (they are otherwise skipped in the build process).

Make sure to pull all git submodules before the build:

```
git submodule init
git submodule update
```
### Linux/OS X/Unix/Posix

Run this in the source directory:

```
cmake . && make
```

If you want to contribute, out of source builds are preferred (as some files are modified by CMake).

### Windows

CMake can generate a visual studio solution which allows compiling all projects via Visual Studio.
