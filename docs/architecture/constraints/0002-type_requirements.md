# Constraint: Type Requirements

* Name: Type Requirements
* Reference Number: 0002

**Table of Contents:**

1. [Description](#description)
2. [Rationale](#rationale)
3. [Source](#source)
4. [Notes](#notes)

## Description

* The Framework shall be built on `type_safe` (using Strong Types where possible)
* The Framework shall be built on `SaferC++`
* The Framework shall disable `SaferC++` type checks by default during release builds
* The Framework shall provide a configuration option to control `SaferC++` passthrough mode

## Rationale

We do not want to ship buggy software in our framework, so we will utilize safer type constructs to help ensure the correctness of our software.

`type_safe` provides:

* Strong type support
* improved built-in types (`ts::integer`, `ts::floating_point`, and `ts::boolean`) which prevent dangerous implicit operations like signed-to-unsigned promotion
* The library also provides "vocabulary" types to help us write more expressive code, such as `object_ref` (non-null ptr), `array_ref` (reference to an array), and `function_ref` (reference to a function)
* Other interesting concepts are also provided, such as `deferred_construction`, a wrapper which allows you to create an object without constructing

`SaferC++` provides:

* Data types that are designed for multi-threaded use and asynchronous access
* Drop-in replacements for `std::vector`, `std::array`, `std::string`, `std::string_view` that provide improved memory safety
* Drop-in replacements for `int`, `size_t`, and `bool` that protect against use of uninitialized values and sign-unsigned comparison issues (similar to `type_safe`)
* Improved pointer & reference types with different compatibility and performance tradeoffs

## Source

Phillip set this constraint at the beginning of the project.

## Notes

* Where `type_safe` and `SaferC++` provide equivalent features, we will prefer `type_safe`
* `type_safe` repository: https://github.com/foonathan/type_safe
* `SaferC++` repository: https://github.com/duneroadrunner/SaferCPlusPlus
