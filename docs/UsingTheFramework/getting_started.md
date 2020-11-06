# Getting Started with the Embedded Virtual Machine

**Table of Contents:**

1. [What is the Embedded Virtual Machine?](#what-is-the-embedded-virtual-machine)
    1. [Project Goals](#project-goals)
    2. [Anatomy of an Embedded Application](#anatomy-of-an-embedded-application)
2. [Detailed Documentation](#detailed-documentation)
3. [The Embedded VM Ecosystem](#the-embedded-vm-ecosystem)
    1. [Example Projects](#example-projects)
4. [Building an Example](#building-an-example)
5. [Ready to Dive In?](#ready-to-dive-in)

## What is the Embedded Virtual Machine?

The Embedded Virtual Machine (Embedded VM or `embvm` for short) is an embedded software framework that helps embedded systems software developers create portable and reusable embedded software components. The Embedded VM core project provides the core abstractions, boot-related code, utilities, and supporting libraries that are used to build Embedded VM applications. These abstractions are used to quickly create and port embedded systems software from one platform to another.

### Project Goals

The ultimate goals of the Embedded Virtual Machine project are:

1. Increasing the ability to reuse embedded systems software across products
1. Minimizing the cost of hardware changes in an embedded system by constraining existing code modifications to the hardware platform layer, as well as writing implementations for any new devices.
1. Reduce time-to-market by enabling developers to build embedded systems software by composing 
1. Provide developers with tools and constructs that enable them to develop, test, and debug embedded software off target (we often use this capability to develop and test drivers on our personal computers, moving them to the target hardware once they're working as expected)

We carefully selected these goals because we were continually tired of repeatedly facing the [same challenges over the past ten years](https://embeddedartistry.com/blog/2018/08/06/musings-on-tight-coupling-between-firmware-and-hardware/). We're tired of being stuck in the software development dark ages. We want to do our part to improve the state of the art in embedded systems software development, enabling users like you to more quickly develop reliable embedded systems software.

### Anatomy of an Embedded Application

We view an embedded systems software application as the sum of four distinct types of software, which we have organized into [*layers*](../architecture/views/layer/layer_view.md) in the Embedded VM.

- The **processor layer**, which contains the non-portable processor code such as interrupt vectors, startup code, processor-specific APIs, and processor peripherals (e.g., SPI and I2C).
- The **hardware platform layer**, which represents the software that supports a given circuit board design. The hardware platform layer contains driver declarations, board-specific configuration code, and any hardware-level APIs that you want to provide to platforms.
- The **platform layer**, which represents the combination of an OS, hardware platform, 
- The **application**, which can run on any **platform** that provides the APIs and functionality required by the application.

In addition to these layers, there are also a set of common **utilities** and **supporting libraries** that can be used by all layers.

Because the Embedded VM is designed for portability and reusability, we attempt to use common types whenever possible. We provide a singular OS API that can be implemented for any number of operating systems

> **Note:** If you're unfamiliar with any concepts or terms used in the documentation, we recommend reviewing our [Domain Context Guide](../architecture/domain_context.md) and [Glossary](../glossary.md).

## Detailed Documentation

We've included all of our architecture, design, and development documentation inside of this repository. Our goal is to provide developers with the background information and design rationale necessary to understand how and why each Embedded VM component was developed.

If you want a deeper look into the Embedded VM ecosystem, we recommend reviewing these architecture documents:

* [System Overview](../architecture/system_overview.md)
* [Conceptual Architecture View](../architecture/views/conceptual_architecture/conceptual_view.md), which showcases significant components and abstractions that make up the Embedded VM framework.
* [Layer View](../architecture/views/layer/layer_view.md), which describes how the Embedded VM framework is organized to support portability, particularly by restricting access to low-level details as you move up the stack.

We also provide documentation landing pads, which allow you to quickly find documentation that you're looking for:

* [Architecture Documentation](../architecture/architecture_documentation.md)
* [Developer Documentation](../development/developer_documentation.md)
* [Using the Framework](using_the_framework.md)

Significant architecture decisions are documented as [Architecture Decision Records](../architecture/decisions) (ADRs), and we also note major real-world events that correspond with the project's development in our [Pottery Project History](../project-history).

## The Embedded VM Ecosystem 

Embedded VM ecosystem source code can be found in two GitHub organizations:

* [embvm](https://github.com/embvm), which contains the core project, modules that extend the core support, and example code.
* [embvm-drivers](https://github.com/embvm-drivers), which contains reusable device drivers that are written for use with the Embedded VM.

The primary Embedded VM project is [`embvm-core`](https://github.com/embvm-core). This project contains the core abstractions, utilities, subsystems, and documentation for the project. All of the other projects in the Embedded VM ecosystem reference `embvm-core`.

In addition to the core project, we provide [modules that extend the Embedded VM](embvm_modules.md), such as adding support for specific processor families or RTOSes. We also release reusable [device drivers](drivers.md) that are designed for use with the Embedded VM ecosystem.

### Example Projects

We provide a number of example projects within the [`embvm` GitHub organization](https://github.com/embvm):

- [embvm/embvm-demo-platforms](https://github.com/embvm/embvm-demo-platforms) contains example platforms and hardware platforms that are shared by our [blinky](https://github.com/embvm/blinky) and [embvm-demo](https://github.com/embvm/embvm-demo) projects
- [embvm/blinky](https://github.com/embvm/blinky) contains the application layer code for a simple `blinky` example. This project runs on a personal computer using an Aardvark adapter hooked up to an LED, an nRF52840 development kit, an nRF USB Dongle, and an STM32 NUCLEO-L4R5ZI development board.
- [embvm/embvm-demo](https://github.com/embvm/embvm-demo) contains a more complicated example application involving LEDs, a time-of-flight sensor, and an OLED display. This application runs on a personal computer using an Aardvark adapter, an nRF52840 development kit, and an STM32L4R5ZI-P Nucleo development kit.
- [embvm/stm32l4](https://github.com/embvm/stm32l4) provides an example hardware platform, platform, and blinky application that will run on an STM32 NUCLEO-L4R5ZI development kit.

## Building an Example

The core project that we've built to demonstrate the capabilities of the Embedded VM ecosystem is the [`embvm-demo`](https://github.com/embvm/embvm-demo) project. As described above, this application runs on multiple platforms that are defined in [`embvm-demo-platforms`](https://github.com/embvm/embvm-demo-platforms).

To build the project for your personal computer, you can clone the `embvm-demo` project and run:

```
$ make
```

> **Note:** Of course, you will need to have the [required dependencies installed](https://embeddedartistry.com/fieldatlas/embedded-artistrys-standardized-meson-build-system/#dependencies).

This will build the simulator program using your build machine's processor architecture. While it is a "simulator" build, real hardware is required in the form of an Aardvark I2C adapter and the time-of-flight sensor and OLED display.

Demos can be cross-compiled using our build system's cross compilation support. The currently supported platforms use Cortex-M4, so you can enable these builds by creating a cross-compilation build instead:

```
$ make CROSS=arm:cortex-m4_hardfloat
```

This will build the host applications that are defined for ARM Cortex-M4 processors, and it will also build the simulator application for your host machine as well.

For more information on our build system, please see [our build system guide](https://embeddedartistry.com/fieldatlas/embedded-artistrys-standardized-meson-build-system/).

## Ready to Dive In?

If you're ready to dive in, we recommend starting with our [New Project Guide](new_project_guide), which will walk you through all of the steps needed to get your own Embedded VM project up and running.
