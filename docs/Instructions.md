# Instructions

This are the instructions that are provided by the wasmint interpreter.

# Terminologie

A **instruction** is AST node and it is identified by its opcode. 
Each instruction can have several **arguments** which are additional parameters that
are located directly behind the opcode in the module. 
Those additonal parameters need to have compile-time fixed values and
can not be instructions like `get_local`.


## `block`

The block instruction defines a array of instructions in the AST. A block instruction requires an additional argument
that specifies the number of instructions in this block (encoded as a LEB128 unsigned integer)

On execution the the children of this AST node will be executed in the order they appear in the module.

A block has a void return value.

### Example:

Code | Instruction
-----|-------------
0x0  | `block`
0x1  | `set_local`

```
0x0 0x2             # Start a `block` with 2 instructions
    0x1 0x0 0x0     # `set_local` - will be executed first
    0x1 0x1 0x0     # `set_local` - will be executed last
```


## `set_local`

The `set_local` instruction sets the value of a local variable. It requires two arguments:

1. The index of the local variable that is the target of this operation as a LEB128 encoded unsigned integer.
2. The value that should be copied to the local variable. The encoding of this value depends on the type of the local variable.

`set_local` has a void return value.

### Example:

Code | Instruction
-----|-------------
0x0  | `set_local`

```
0x0 0x5 0x3     # Will assign the value 3 to the local variable with the index 5
```

## `get_local`

The `get_local` instruction gets the value of a local variable. It requires as an argument the index of the local variable
which is then returned by this instruction upon execution.

The return type of `get_local` is the type of the local variable.

### Example:

Code | Instruction
-----|-------------
0x0  | `get_local`

```
0x0 0x1     # Returns the value of the local variable with the index 1
```

