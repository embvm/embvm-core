# Domain Context Model

This Domain Context Model represents the framework's embedded systems concepts.

Major concepts and connections are:

* [Firmware](#firmware) is the name for software which runs on an embedded system
* A firmware [binary](#binary) is a compiled software package
	* Other names are "image" or "payload"
* A typical firmware [binary](#binary) consists of:
	* [Application](#application) logic, which is unique to each system's goals and responsibilities
	* [Libraries](#library) and [utilities](#utility), which provide reusable support functionality
	* [Subsystems](#subsystem), which encapsulate a standalone functional operation
	* [Drivers](#driver), which allow software to interact with hardware components
* An [RTOS](#rtos) may optionally be used to provide threading and minimal OS support
* A firmware binary runs on a [Hardware Platform](#hardware-platform) (aka PCB/board), which is composed of:
	* A [Processor](#processor)
	* Any number of [circuits](#circuit), which can be ICs, passive circuitry, or active circuitry
* A firmware binary is produced by a [build system](#build-system)
* A [CI server](#ci-server) uses the build system to create and store firmware images
	* A CI server often runs tests to check the output quality
* The term [virtual](#virtual) refers to an abstract interface which generalizes the underlying functionality
	* e.g., virtual hardware platform, virtual RTOS, virtual processor

## Diagram

<center>

| ![Domain Context Model UML Diagram](images/DomainConceptModel.jpg) |
|:--:|
| *Figure 1. Domain Context Model for Embedded Framework.* |

</center>

## Concept Catalogue

* [Application](#application)
* [Binary](#binary)
* [Build system](#build-system)
* [CI server](#ci-server)
* [Circuit](#circuit)
* [Driver](#driver)
* [Firmware](#firmware)
* [HAL](#hal)
* [Hardware Platform](#hardware-platform)
* [Library](#library)
* [Processor](#processor)
* [Protocol](#protocol)
* [RTOS](#rtos)
* [Subsystems](#subsystems)
* [Utilities](#utility)
* [Virtual](#virtual)

### Application

* An "application" is a software program
* The application logic controls how an embedded device operates
* The application logic is unique to each system's goals and responsibilities
* In most cases, the business's core IP is contained within the application logic

### Binary

* A firmware binary is a compiled software package that is runnable on an embedded system
* Other common terms are a firmware "image" or firmware "payload"
* A typical firmware binary consists of:
	* [Application](#application) logic
	* [Libraries](#library) and [utilities](#utility)
	* [Subsystems](#subsystem)
	* [Drivers](#driver)
* The output of a [build system's](#build-system) compilation process is a firmware binary

### Build System

* The build system contains rules for assembling a set of source code files into a [binary](#binary)
* The build system consists of the compiler/linker, rules for compiling the project, and associated tools which are used during the assembly process
* The output of the compilation process is a firmware binary
* The build system will also compile unit tests

### CI Server

* CI = Continuous Integration
* Also known as a "build server" or "integration server"
* A CI server checks out a copy of the [firmware](#firmware) source code and uses the [build system](#build-system) to create and store firmware [binaries](#binaries)
* A CI server often runs tests to check the output quality
* A CI server will often track metrics for a build over time, such as:
	* Warnings
	* Build Failures
	* Test Failures
* A CI server can be configured to report results to another server
	* Often, code management services will require a build to compile correctly and pass all tests before a merge request is approved

### Circuit

* A circuit is a set of electrical components which are interconnected to provide a specific function
* [Hardware platforms](#hardware-platform), aka circuit boards, are composed of a [processor](#processor) and corresponding circuits

Circuits can be Integrated Circuits (ICs), which are standalone components on a circuit board:

> A circuit in which all or some of the circuit elements are inseparably associated and electrically interconnected so that it is considered to be indivisible for the purposes of construction and commerce.

Circuits can also be made of discrete components, and may be classified as active or passive.

Active circuits:

> Active circuits are those which can provide gain, either voltage gain or current gain.

> An active circuit is a circuit which contains at least one of these (common) active devices: transistors (bipolar or FET types), operational amplifiers (opamps), OTAs (Operational Transconductance Amplifiers) or vacuum tubes (of the amplifying type, mostly triodes and pentodes). There are some other variants of active devices (e.g., MESFETs) but they are less common).

Passive circuits:

> Passive circuits are those which cannot or do not provide any gain.

> A circuit without an active device is a passive circuit. The active and passive terms are usually applied to filters.

Further Reading:
* [Wikipedia: Integrated Circuit](https://en.wikipedia.org/wiki/Integrated_circuit)
* [What is the Difference Between Active and Passive Circuits?](https://www.quora.com/What-is-difference-between-active-and-passive-circuits)

### Driver

A driver is a software module which encapsulates interactions with a [processor's peripheral](#processor) or a [circuit](#circuit).

A driver typically deals directly with hardware (in the case of the [HAL](#hal), or hardware abstraction layer) or with other drivers (such as an accelerometer driver which communicates to the part using a SPI bus driver).

Each electrical circuit requires its own device driver. Interfaces can be written in a generic manner, meaning that different classes of drivers are implemented to the same interface. With a generic driver interface, drivers of the same class are interchangeable (e.g., you can swap one accelerometer with another).

### Firmware

* [Firmware](#firmware) is the name for software which runs on an embedded system
* Firmware refers either to the set of source code, the class of program (i.e., software for embedded systems), or the [binary](#binary) output

### HAL

* HAL = Hardware Abstraction Layer
* A specialization of the [driver](#driver) concept
* A driver that deals directly with hardware (e.g., a [processor peripheral](#processor)) is a HAL driver
* HAL drivers contains more interfaces and responsibilites than generic drivers
	* Often requires a memory-mapped register address which corresponds to the device's hardware interface
	* May or may not have DMA support
	* May or may not have interrupt support
	* May or may not have configurable clocks
	* May or may not support error conditions and recovery mechanisms

### Hardware Platform

* Also commonly referred to as "board" or "PCB" (Printed circuit board)
* A hardware platform is the total hardware system which the firmware application is targeted board
	* Composed of:
		* A [Processor](#processor)
		* Any number of [circuits](#circuit), which can be ICs, passive circuitry, or active circuitry
	* The hardware platform may be a complete design, such as a printed circuit board
	* The hardware platform may also be a loosely connected design, involving multiple boards/components connected via wires or cabling
* The firmware [binary](#binary) runs on the hardware platform

### Library

* A software module which provides a set of functionality
* Reusable across projects
* Typically contains a cohesive responsibility or theme, such as "fixed-point mathematics", "logging", or "version"
* Differ from [Subsystems](#subsystems) in that they are not typically active modules that handle a specific system responsibility

Further Reading:

* [Wikipedia: Library (Computing)](https://en.wikipedia.org/wiki/Library_(computing))

### Processor

* A processor is an integrated circuit that contains all the functions of a central processing unit of a computer
* A processor has an architecture, which is the underlying abstract model that the processor implements. The architecture includes:
	* Machine language (assembly)
	* Register mapping, addressing, and functionality
	* Memory locations and offsets
	* Complex instructions, such as DMA, caching, floating-point arithemetic
* A processor also has a vendor, which is the company that designed and implemented the processor
	* Each vendor has particular style of implementation, feature sets, and peripheral implementations
* Each vendor organizes processors into families, which are processor lines that contain similar functionality at various
* A processor may or may not offer a set of hardware **peripherals**, which are integrated into the chip and usable through Special Function Register (SFR) interfaces
	* These hardware devices contain memory-mapped structures which are used to interact with the peripheral hardware
	* These may be sensors, communication busses, timers, etc.
* Other common terms: microprocessor, microcontroller, system on a chip (SoC)
	* Microprocessor typically has fewer (or no) peripheral devices
	* SoCs typically have a wide array of integrated peripherals
* A [firmware](#firmware) [binary](#binary) runs on the processor

Further Reading:

* [Wikipedia: Microprocessor](https://en.wikipedia.org/wiki/Microprocessor)
* [Wikipedia: CPU](https://en.wikipedia.org/wiki/Central_processing_unit)
* [Wikipedia: Instruction Set Architecture](https://en.wikipedia.org/wiki/Instruction_set_architecture)
* [Wikipedia: Microarchitecture](https://en.wikipedia.org/wiki/Microarchitecture)

### Protocol

TBD

### RTOS

* RTOS = Real-time Operating System
* An [RTOS](#rtos) may optionally be used to provide threading and minimal OS support
* Many RTOSes offer preemptive multitasking support, to ensure that the highest priority task is always executing
* RTOSes often provide stricter timing and latency guarantees compared to more traditional operating systems

### Subsystem

* A subsystem encapsulates a standalone functional operation within a larger software system
* Subsystems are decompositions of systems
* Each subsystem should have a singular (albeit complex) purpose
* Subsystems are typically active in nature, requiring their own threads or regular processing time
* Example subsystems include: command line, system power management, over-the-air (OTA) updater, firmware updater

### Utility

* Utilities represent cross-cutting constructs that can be used without creating specific platform dependencies
* A utility is a specialization of a [library](#library)
* The scope is typically much smaller: a single class or single file which provides a re-usable piece of functionality
* Example utilities include static queues, buffer pools, dispatch queues, or static `std::function` alternative

### Virtual

* The term [virtual](#virtual) refers to an abstract interface which generalizes the underlying functionality
	* e.g., virtual hardware platform, virtual RTOS, virtual processor
* Higher-level software is implemented to virtual interfaces in order to decouple the software from the underlying hardware
