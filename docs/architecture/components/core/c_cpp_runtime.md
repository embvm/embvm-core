# CRC-R: C/C++ Runtime

The C/C++ runtime handles the setup of the C and C++ runtime environment and provides standard library functions expected by C and C++ developers.

**Status:** Complete

## Responsibilities

* Perform setup required by the C/C++ languages:
    * Clear `.bss` section contents
    * Call constructors & initializers
    * Initialize `argc` and `argv` (in our case, to 0 and `NULL`)
* Provide runtime functionality required by the C/C++ library

## Requirements

1. Handle memory relocations
2. Zero-initialize the `.bss` section
3. Initialize the standard library runtime
5. Call constructors in the constructor segment

## Collaborators

* The [Boot Sequencer](boot_sequencer.md) invokes C/C++ Runtime Startup process

## Rationale

C and C++ applications require memory relocations, zero-initialization, and invoking static constructors prior to entering the `main()` function. Rather than making this part of the Boot Sequencer, it should be kept within the runtime to decouple the Boot Sequencer from the language runtime.

## Source Links

* [libc](src/stdlibs/libc)
* [libmemory](src/stdlibs/libmemory)
* [libcpp](src/stdlibs/libcpp) - contains `libcxx` and `libcxxabi`

## Notes

N/A
