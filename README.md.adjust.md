# Embedded Artistry's Embedded Virtual Machine

**Table of Contents**

1. [About the Embedded Virtual Machine](#about-the-embedded-virtual-machine)
2. [Documentation Overview](#documentation-overview)
3. [Getting Started](#getting-started)
	1. [Dependencies](#dependencies)
		1. [git-lfs](#git-lfs)
		2. [meson](#meson)
		3. [adr-tools](#adr-tools)
	2. [Getting the Source](#getting-the-source)
	3. [Building the Framework](#building-the-framework)
		1. [Listing Targets](#listing-targets)
		2. [Cross-compilation](#cross-compilation)
		3. [Using an Alternate Toolchain](#using-an-alternate-toolchain)
		4. [Debug vs Release](#debug-vs-release)
	4. [Building Documentation](#building-documentation)
	5. [Running Tests](#running-tests)
	6. [Running Static Analysis](#running-static-analysis)
	7. [Running Demo Applications](#running-demo-applications)
	8. [Building Your Own Framework Program](#buildling-your-own-framework-program)
4. [Build Configuration Options](#build-configuration-options)
5. [Formatting](#formatting)
6. [Release Process](#release-process)
6. [Need Help?](#need-help)
7. [Authors](#authors)
8. [License](#license)
9. [Acknowledgments](#acknowledgments)

## About the Embedded Virtual Machine

## Documentation Overview

* The [Glossary](docs/glossary.md) contains definitions for concepts used in the documentation and code
* The [Software Inventory](docs/software_inventory.xlsx) contains a list of all open source projects included in this framework, with version numbers and licensing information
* The [Development/](docs/development) folder contains guidelines for developers who are building framework components:
	* [Developer Guide](docs/development/developer_guide.md) contains guidelines for developers working on the project
	* [C++ Guidelines](docs/development/cpp_guidelines.md) contains guidelines for the C++ code used in the framework
	* [Documentation Guidelines](docs/development/documentation.md) clarify documentation practices for framework modules
	* [Error Model](docs/development/error_model.md) describes the framework's approach to error handling
	* [Namespaces.xlsx](docs/development/namespaces.xlsx) contains a list of namespaces defined by the framework
	* [Refactoring](docs/development/refactoring.md) provides guidelines for cleaning up existing code
	* The [Patterns/](docs/development/patterns) directory contains notes about patterns used throughout the framework
	* The [References/](docs/development/references) directory contains useful references for developers

## Getting Started

1. [Dependencies](#dependencies)
	1. [git-lfs](#git-lfs)
	2. [meson](#meson)
2. [Building the Framework](#building-the-framework)
3. [Building Documentation](#building-documentation)
3. [Running Tests](#running-tests)
4. [Running Demo Applications](#running-demo-applications)
5. [Building Your Own Framework Program](#buildling-your-own-framework-program)

### Dependencies

The primary requirements for building the framework are:

* [meson](#meson) (build system)
* A C++17-capable compiler for the host machine (simulator applications and tests)
* A C++17-capable compiler for the target machine

If you are cross-compiling for ARM using the arm-none-eabi-gcc toolchain, you will need to use **version 9.0 or later**. If you cannot get this version for your platform due to package availability, you can build the most recent compiler version using the [arm-gcc-bleeding-edge](https://github.com/embeddedartistry/arm-gcc-bleeding-edge) project.

For access to the full framework documentation, you will need [`git-lfs`](#git-lfs).

A variety of support tools are used by the framework for testing and static analysis:

* lizard (ccc)
* brew install gcovr (code coverage)
* unittest-cpp (to run ETL unit tests)
* cmocka (to run libmemory and libc unit tests)

To develop for the framework, you will need:

* [adr-tools](#adr-tools)
* [pottery](#pottery)

#### git-lfs

This repository requires git-lfs. If you do not have this installed, please visit [the git-lfs web page](https://git-lfs.github.com).

If you cloned this repository before installing git-lfs, please run `git lfs pull`. Otherwise clone will automatically perform a `git lfs pull`.

#### meson

This repository builds with [meson](https://mesonbuild.com), which requires Python 3 and Ninja.

To install on Linux:

```
sudo apt-get install python3 python3-pip ninja-build
```

To install on OS X:

```
brew install python3 ninja
```

Meson can be installed through `pip3`:

```
pip3 install meson
```

If you want to install Meson globally on Linux, use:

```
sudo -H pip3 install meson
```

#### adr-tools

This repository uses [Architecture Decision Records](https://embeddedartistry.com/blog/2018/4/5/documenting-architectural-decisions-within-our-repositories). Please install [`adr-tools`](https://github.com/npryce/adr-tools) to contribute to architecture decisions.

If you are using OSX, you can install `adr-tools` through Homebrew:

```
brew install adr-tools
```

If you are using Windows or Linux, please install `adr-tools` via [GitHub](https://github.com/npryce/adr-tools).

#### pottery

TBD

#### Supporting Tools

* lizard (ccc)
* brew install gcovr (code coverage)
* clang-format
* [cmocka](https://cmocka.org) (for some framework tests)
* vale
* Doxygen
* cppcheck
* clang-analyze

### Getting the Source

This project uses `git-lfs`, so please install it before cloning. If you cloned prior to installing `git-lfs`, simply run `git lfs pull` after installation.

This project is [hosted on GitHub][8]. You can clone the project directly using this command:

```
git clone --recursive git@github.com:embeddedartistry/libc.git
```

### Building the Framework

To build all framework components, you can run `make` at the project root. All build output will be placed in the `buildresults` folder by default.

You can specify another build output folder for use with `meson`. You can invoke the build using:

```
$ meson your-buildresult-folder [options]
$ meson arm_build --buildtype release --cross-file build/cross/gcc/arm/gcc_arm_cortex-m4.txt
```

Depending on the target architecture and its supported functionality, some items may need to be disabled. For example, this command disables threading and `std::chrono`:

```
meson buildresults --cross-file build/cross/gcc/arm/gcc_arm_cortex-m4.txt -Dlibcxx-enable-chrono=false -Denable-threading=false
```

You can enable threading support with an RTOS and the C++ standard library using the `libcxx-thread-library` and `os-header-path` options. `os-header-path` is specified relative to the `libcpp` directory.

```
meson buildresults --cross-file build/cross/gcc/arm/gcc_arm_cortex-m4.txt -Dlibcxx-thread-library=threadx -Duse-external-stdlibs=true -Dos-header-path=../../os/threadx/include
```

Once the directory has been created by `meson`, you can build all targets with `make`:

```
$ make
```

Individual targets can be built within the `buildresults` folder:

```
06:36:30 libcpp$ cd buildresults/
06:37:18 buildresults$ ninja libc++.a
[5/5] Generating install-cpp-headers with a custom command.
```

#### Listing Targets

#### Cross-Compilation

Cross compilation is managed through meson "cross-files".

Example cross file:

```
meson buildresults --cross-file build/cross/gcc/arm/gcc_arm_cortex-m4.txt
```

Note that if you are cross-compiling for ARM using the arm-none-eabi-gcc toolchain, you will need to use **version 9.0 or later**. If you cannot get this version for your platform due to package availability, you can build the most recent compiler version using the [arm-gcc-bleeding-edge](https://github.com/embeddedartistry/arm-gcc-bleeding-edge) project.

#### Using an Alternate Toolchain

Meson is going to build with a default toolchain on your system. Some developers will have multiple toolchains installed, such as GCC and clang.

You can use the alternate toolchain through one of two approaches: 

1. Creating a cross file which points to the alternate toolchain (works even if you're targeting the same architecture)
2. Using the standard GNU make environment variables on when invoking meson.

As an example of number two, you can say:

```
CC=/usr/local/opt/gcc/bin/gcc-9 CXX=/usr/local/opt/gcc/bin/g++-9 AR=/usr/local/opt/gcc/bin/gcc-ar-9 meson buildresults
```

For a full list of implicit variables, [check out the Make manual](https://www.gnu.org/software/make/manual/html_node/Implicit-Variables.html).

* AR - Archive-maintaining program; default ‘ar’.
* AS - Program for compiling assembly files; default ‘as’.
* CC - Program for compiling C programs; default ‘cc’.
* CXX - Program for compiling C++ programs; default ‘g++’.

#### Debug vs Release

By default, the project is configured to generate release builds. This means that debug symbols are not provided. The default optimization level for the framework is `-O2`.

To build a debug variant, you will need to manually configure the meson project and use the `--debug` option. You may also optionally set the `--optimization` option to `g` for an optimized debug build.

```
meson buildresults --debug --optimization g
```

### Building Documentation

To generate the Doxygen documentation, run `make docs` at the project root. Documentation will be placed into `buildresults/documentation/html`. You can open `index.html` to access the document root.

### Running Tests

The framework uses Catch for unit testing. Some dependencies use CMocka. If CMocka is not installed on your system, the tests will not be built or run.

To run the framework tests and print the results to console:

```
$ make test-framework
```

To run all the framework unit tests:

```
$ make test
1/4 libmemory / libmemory_freelist_tests    OK       0.02 s
2/4 libc / printf_tests                     OK       0.02 s
3/4 libc / libc_tests                       OK       0.07 s
4/4 Framework / framework_tests             OK       0.17 s

OK:         4
FAIL:       0
SKIP:       0
TIMEOUT:    0
```

Test results for the framework will be output into `buildresults/framework_tests.xml`. Module test results will be found in `buildresults/test`.

### Running Static Analysis

TBD

### Running Demo Applications

TBD

### Building Your Own Framework Program

TBD

## Build Configuration Options

Our meson build can be configured in a variety of the ways through.

### Debug

By default, the framework is built in release mode. To enable a debug build, you must supply the meson configuration option `--debug`.

```
meson buildresults --debug --optimization g
```

### Optimization

The optimization level for the framework defaults to `-O2`.

To modify the optimization level, you must supply the meson configuration option `--optimization`.

```
meson buildresults --debug --optimization g
meson buildresults --debug --optimization 1
```

### Vendor SDK Path

Your project may need a vendor SDK, especially for migratory purposes. You can use the `vendor-sdk-path` option to provide a path to this SDK:

```
option('vendor-sdk-path', type: 'string', value: '')
```

The vendor SDK path is relative to where the binaries are declared, or where the path is first used. This is not relative to the top level of the framework. This can be confusing.

So if you are at:

```
~/src/ea/embedded-framework
```

And the SDK is at:
```
~/Downloads/nRF5_SDK_15.3.0_59ac345
```

And the binary build folder where you use the SDK is at:

```
embedded-framework/src/applications/nrf52
```

Then you need to use this path:

```
../../../../../../Downloads/nRF5_SDK_15.3.0_59ac345
```

Here's an example invocation using the NRF52 SDK:

```
meson buildresults --cross-file=build/cross/gcc/arm/nrf52840.txt -Dvendor-sdk-path=../../../../../../Downloads/nRF5_SDK_15.3.0_59ac345
```

## Formatting

This repository enforces formatting using [`clang-format`](https://clang.llvm.org/docs/ClangFormat.html).

You can auto-format your code to match the style guidelines by issuing the following command:

```
make format
```

Formatting is enforced by the Jenkins build server which runs continuous integration for this project. Your pull request will not be accepted if the formatting check fails.

## Release Process

## Need Help?

If you need further assistance or have any questions, please [file a GitHub Issue][6] or send us an email using the [Embedded Artistry Contact Form][5].

You can also [reach out on Twitter: mbeddedartistry](https://twitter.com/mbeddedartistry/).

## Authors

* **[Phillip Johnston](https://github.com/phillipjohnston)** - original framework author - [Embedded Artistry](https://github.com/embeddedartistry)

## License

The framework is Copyright © 2018 Embedded Artistry LLC.

TBD

A full list of open source modules and their licenses can be found in the [Software Inventory](docs/development/software_inventory.xlsx).

## Acknowledgments

I would like to thank the following individuals for their direct contributions to this project:

* Rozi Harris, who has tirelessly listened to ideas, reviewed the framework architecture, and edited documentation

I would like to thank the following individuals for their inspiration on this project:

* Ruth Malan, who inspired the architecture documentation and process
* James Grenning, who taught me the value of Test-Driven Development (TDD)
* Jerry Fitzpatrick, who inspired many of the guidelines and practices that were used to build this framework

I would like to thank the following individuals for the open source libraries that have been incorporated into this framework:

* John Wellbelove for the [Embedded Template Library](http://etlcppc.com)
* Jonathan Müller for [type_safe](https://github.com/foonathan/type_safe) and [debug_assert](https://github.com/foonathan/debug_assert)

[5]: http://embeddedartistry.com/contact
[6]: https://github.com/embeddedartistry/embedded-framework/issues/new
