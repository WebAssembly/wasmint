# Binary Format

This is the documentation to the binary format that is used by wasmint.

## Common types

### uLEB128

A unsigned integer encoded in LEB128

### String

A series of UTF-8 characters terminated by a zero byte ('\0').

## Module

Each module file starts with a module header. This header contains as of now

1. the local opcode table
2. the local type table
3. the function table for external functions
4. the section table

in the order given above.

The sections of the module are located directly behind the header.

### Local opcode table

The local opcode table provides functionality to translate opcodes (integers)
to the string names of the instructions.

The opcode table starts with a uLEB128 (from now on called OT_LEN) 
that specifies how many opcodes are used in this module.

After that exactly OT_LEN strings follow that contain the names of the used opcodes.
The order of the strings define their local opcode.

As an example, this opcode table would define 2 opcodes for the instruction `call`
and for the instruction `int32.add`:

```
2, // number of opcodes
'i', 'n', 't', '3', '2', '.', 'a', 'd', 'd', '\0', // opcode = 0x0
'c', 'a', 'l', 'l', '\0', // opcode = 0x1 and so on
```

For information what instructions are available, see the (Instructions.md)[Instructions.md)

### Local type table

The local opcode table provides functionality to translate type indizes (integers)
to the string names of the types.

The local type table starts with a uLEB128 (from now on called LT_LEN) 
that specifies how many types are used in this module.

After that exactly LT_LEN strings follow that contain the names of the used types.
The order of the strings define their local type index.

As an example, this type table would define indizes for the types `int32` and `void`.

```
2, // number of types
'i', 'n', 't', '3', '2', '\0', // index = 0x0
'v', 'o', 'i', 'd', '\0', // index = 0x1 and so on
```

### Function table for external functions

This has to be empty right now and should only consist of a zero byte (0x0).

### Section table

The section table identifies the number, location and type of all sections in this module.

It starts with a uLEB128 that identifies how many sections 
are in this module.

Then all sections are described in the order of appearance in this module (lowest offset first).

Each section is described by a uLEB128 that identifies the type of the section. Currently
only the type 0x1 is supported which means code section.

Afterwards a uLEB128 provides the offset of the section in this module (e.g. the index of the first byte
in this file. The first byte of the file has the index 0).

Those two uLEB128 are repeated for each section.

## Code section

A code section contains multiple functions. It starts with the signatures of all functions and then the 
serialized AST nodes of each function follows.

### Function signatures

First  with a uLEB128 that identifies the number of functions this section contains.

Now, for each function in this section, the signature of each function follows.

Each signatures consists of those elements in the given order:

1. A string that contains the name of this function
2. A uLEB128 which is the type index that identifies the return type of this function
3. A uLEB128 that identifies the number of parameters of this function
4. A uLEB128 the offset of the first byte of the serialized AST in the module

Directly after the last signature, the serialized AST of the first function follows. 
Then the serialized AST of the secondfunction and so on.

### Serialized AST

The serialized AST directly starts witht he first opcode. The end of the AST can only be identified
by parsing the whole AST.