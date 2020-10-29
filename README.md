# Embedded Virtual Machine Core Project

This repository contains the designs, documentations, and source files for the Embedded Virtual Machine (`embvm`) project's core interfaces, libraries, and subsystems. This project is required to build Embedded VM applications and reusable modules (including drivers, OS interfaces, subsystems, and more).

Note that this project is dual-licensed. The open-source version of this project is released under GPLv3. [A commercial license](https://embeddedartistry.com/product/embedded-virtual-machine-commercial-license), which provides holders with the ability to create and distribute software without open-source obligations, can be purchased in the [Embedded Artistry store](https://embeddedartistry.com/store).

## Table of Contents

1. [About the Project](#about-the-project)
2. [Project Status](#project-status)
3. [Getting Started](#getting-started)
    1. [Requirements](#requirements)
        1. [git-lfs](#git-lfs)
        1. [Meson Build System](#meson-build-system)
    2. [Getting the Source](#getting-the-source)
    3. [Building](#building)
    4. [Testing](#testing)
4. [Configuration Options](#configuration-options)
5. [Documentation](#documentation)
6. [Need Help?](#need-help)
7. [Contributing](#contributing)
9. [Authors](#authors)
10. [License](#license)
11. [Acknowledgments](#acknowledgements)

# About the Project

The Embedded Virtual Machine (Embedded VM or `embvm` for short) is an embedded software framework that helps developers create portable and reusable embedded software components. The Embedded VM core project provides the core abstractions, boot-related code, utilities, and supporting libraries that are used to build Embedded VM applications. These abstractions are used to quickly create and port embedded systems software from one platform to another.

For a comprehensive introduction to the project, please see the [Getting Started Guide](docs/UsingTheFramework/getting_started.md). You can also find a detailed overview of the project and its design goals in the [System Overview](docs/architecture/system_overview.md).

**[Back to top](#table-of-contents)**

# Project Status

This project is under active development. Reusable programs, platforms, and drivers have been successfully written using the Embedded VM, and they run on ARM Cortex-M4 embedded platforms. Supporting utilities are continually being tested, tuned, and optimized. Interfaces are evolving, and many interfaces still need to be created. A number of reusable subsystems are planned, but not yet implemented.

**[Back to top](#table-of-contents)**

## Getting Started

The instructions below will help you download and build the project.

Detailed documentation on the design and use of the Embedded VM can be found in the [docs/](docs/) folder.

If this is your first encounter with the Embedded VM project, we recommend beginning with our [Getting Started Guide](docs/UsingTheFramework/getting_started.md). This guide includes links to demo projects that are built using the Embedded VM.

### Requirements

This project uses [Embedded Artistry's standard Meson build system](https://embeddedartistry.com/fieldatlas/embedded-artistrys-standardized-meson-build-system/), and dependencies are described in detail [on our website](https://embeddedartistry.com/fieldatlas/embedded-artistrys-standardized-meson-build-system/).

At a minimum you will need:

* [`git-lfs`](https://git-lfs.github.com), which is used to store binary files in this repository
* [Meson](#meson-build-system) is the build system
* Some kind of compiler for your target system.
    - This repository has been tested with:
        - gcc-7, gcc-8, gcc-9
        - arm-none-eabi-gcc
        - Apple clang
        - Mainline clang

#### git-lfs

This project stores some files using [`git-lfs`](https://git-lfs.github.com).

To install `git-lfs` on Linux:

```
sudo apt install git-lfs
```

To install `git-lfs` on OS X:

```
brew install git-lfs
```

Additional installation instructions can be found on the [`git-lfs` website](https://git-lfs.github.com).

#### Meson Build System

The [Meson](https://mesonbuild.com) build system depends on `python3` and `ninja-build`.

To install on Linux:

```
sudo apt-get install python3 python3-pip ninja-build
```

To install on OSX:

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

**[Back to top](#table-of-contents)**

### Getting the Source

This project uses [`git-lfs`](https://git-lfs.github.com), so please install it before cloning. If you cloned prior to installing `git-lfs`, simply run `git lfs pull` after installation.

This project is hosted on GitHub. You can clone the project directly using this command:

```
git clone --recursive https://github.com/embvm/embvm-core
```

If you don't clone recursively, be sure to run the following command in the repository or your build will fail:

```
git submodule update --init --recursive
```

**[Back to top](#table-of-contents)**

### Building

If Make is installed, the project can be built by issuing the following command:

```
make
```

This will build all targets for your current architecture.

You can clean builds using:

```
make clean
```

You can eliminate the generated `buildresults` folder using:

```
make distclean
```

You can also use  `meson` directly for compiling.

Create a build output folder:

```
meson buildresults
```

And build all targets by running

```
ninja -C buildresults
```

Cross-compilation is handled using `meson` cross files. Example files are included in the [`build/cross`](build/cross/) folder. You can write your own cross files for your specific processor by defining the toolchain, compilation flags, and linker flags. These settings will be used to compile the project.

Cross-compilation must be configured using the meson command when creating the build output folder. For files stored within `build/cross`, we provide a Makefile `CROSS` to simplify the process. This variable will automatically supply the proper Meson argument, `build/cross/` prefix, and `.txt` filename extension.

You can use a single file, or you can layer multiple files by separating the names with a colon.

```
make CROSS=arm:cortex-m4_hardfloat
```

You can also do this manually with the Meson interface. Note, however, that you will need to include a special `--cross-file=build/cross/embvm.txt` cross file to ensure that the required Embedded VM settings are applied.

```
meson buildresults --cross-file build/cross/arm.txt --cross-file build/cross/cortex-m4_hardfloat.txt --cross-file=build/cross/embvm.txt
```

Following that, you can run `make` (at the project root) or `ninja -C buildresults` to build the project.

> **Note:** Tests will not be cross-compiled. They will only be built for the native platform.

**Full instructions for working with the build system, including topics like using alternate toolchains and running supporting tooling, are documented in [Embedded Artistry's Standardized Meson Build System](https://embeddedartistry.com/fieldatlas/embedded-artistrys-standardized-meson-build-system/) on our website.**

**[Back to top](#table-of-contents)**

### Testing

The tests for this library are written with Catch2, which is included as a subproject and does not need to be installed on your system. You can run the tests by issuing the following command:

```
make test
```

By default, test results are generated for use by the CI server and are formatted in JUnit XML. The test results XML files can be found in `buildresults/test/`. You can manually run the test programs if needed. 

For more information on testing, see [Unit Testing with Catch2](docs/UsingTheFramework/unit_testing_with_catch2.md).

**[Back to top](#table-of-contents)**

## Configuration Options

Build configuration is covered in depth in the [Configuration and Tuning](docs/UsingTheFramework/configuration_and_tuning.md) guide.

**[Back to top](#table-of-contents)**

## Documentation

We provide a large amount of documentation for the Embedded VM project.

* The [Glossary](docs/glossary.md) contains definitions for concepts used in the documentation and code
* The [Software Inventory](docs/software_inventory.xlsx) contains a list of all open source projects included in this framework, with version numbers and licensing information
* The [Architecture and Design Documentation](docs/architecture/) can be used to understand the current Embedded VM design, as well as the rationale behind many of the decisions made in this project.
* Our [Developer Documentation](docs/development/developer_documentation) folder provides guidelines and standards for developers who are building the Embedded VM or related components.
* The [Project History](docs/project-history) folder contains a lot of major events that have occurred throughout this project.


### Building the Documentation

Full documentation can be built locally by running the following command:

```
make docs
```

Documentation can be found in `buildresults/docs`, and the root page is `index.html`.

**[Back to top](#table-of-contents)**

## Need help?

If you need further assistance or have any questions, please file a GitHub issue or send us an email using the [Embedded Artistry Contact Form](http://embeddedartistry.com/contact).

You can also [reach out on Twitter: mbeddedartistry](https://twitter.com/mbeddedartistry/).

## Contributing

If you are interested in contributing to this project, please read our [contributing guidelines](docs/CONTRIBUTING.md).

## Authors

* **[Phillip Johnston](https://github.com/phillipjohnston)**

## License

Copyright © 2020 Embedded Artistry LLC

Note that this project is dual-licensed. The open-source version of this project is released under GPLv3. [A commercial license](https://embeddedartistry.com/product/embedded-virtual-machine-commercial-license), which provides holders with the ability to create and distribute software without open-source obligations, can be purchased in the [Embedded Artistry store](https://embeddedartistry.com/store).

See the [LICENSE](LICENSE) file for further licensing details.

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

**[Back to top](#table-of-contents)**

