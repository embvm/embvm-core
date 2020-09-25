# 16. How to Handle Meson Library Targets with respect to libc and libcxx header includes

Date: 2019-08-02

## Status

Accepted

## Context

When compiling static libraries within the framework, we need to have access to libc and libc++ includes. For example, when creating the nRF52840 library, there were build failures related to the library using system include headers instead of the intended headers.

This is an error-prone process and we need to figure out how to handle it.

## Decision

Because of the way the build system is structured, we can't easily use include_directories() and separate native/target header include alternatives. It will be easier to handle this instruction through documentation instead.

One option we considered was to add libc and libc++ include directories to the framework_includes, so all framework classes pick them up. However, this also impacts the test code (which could be worked around), which does not include our internal stdlib headers. However, we would need multiple variables - to differentiate between native and target headers.

Another option that was considered was to use add_project_options to foist includes on every target in the project. This would allow us to differentiate between native and target. However, the tests would have been affected by this without ability to work around it.

## Consequences

- We don't have to have an extensive re-architecture of the build system or complicated `add_project_arguments()` function calls to differentiate between includes for compilers + native/non-native arch.
- Users who don't look at the documentation may introduce an erroneous build component
    - However, this _largely_ matters only for internal framework libraries
    - Clients should always be consuming a general dependency for all targets which specifies framework include directories for everything
