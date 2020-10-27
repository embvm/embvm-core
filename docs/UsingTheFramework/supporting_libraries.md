## Supporting Libraries Available Through embvm-core

In addition to the types and modules supplied by the `embvm-core` module, your applications will also be able to access a number of supporting libraries that are bundled with (and used by) the `embvm-core` project.

## Standard Libraries

The `embvm-core` is dependent upon the Embedded Artistry standard library suite, which provides library implementations that are targeted for microcontroller-based embedded systems. 

The following libraries are used by `embvm-core`:

* [embeddedartistry/libc](https://github.com/embeddedartistry/libc)
* [embeddedartistry/libcpp](https://github.com/embeddedartistry/libcpp)
* [embeddedartistry/libmemory](https://github.com/embeddedartistry/libmemory) - provides memory allocator implementations that can be used with the framework
* [embeddedartistry/compiler-rt](https://github.com/embeddedartistry/compiler-rt) - supplies implementations for "compiler built-in" style functions, as well as `__aeabi` functions.

## Embedded Template Library

The [Embedded Template Library](https://etlcpp.com) is a core component in the `embvm` ecosystem. This library provides static-memory-only STL type alternatives, static memory pools, and other useful types. The library's design goal is to support C++ on embedded systems, so it only makes sense that we designed the `embvm` framework to include the ETL.

## C++ "Extensions"

- others??? plf-stack, ring_span, etc.

## Improving Code Quality

- gsl-lite
    - type-safe

## Test Libraries

Both `catch2` and `cmocka` wrap files are provided with the [`embvm-project-skeleton`](https://github.com/embvm/embvm-project-skeleton) template. Catch2 is the primary test library used by the `embvm-core` project.

For more information on testing, see [Unit Testing with Catch2](unit_testing_with_catch2.md).
