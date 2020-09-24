# User Story: Developer Group

* Name: Developer User Story Group
* Category: Group
* Reference Number: 0001

**Table of Contents:**
* [Story List](#story-list)
* [Acceptance Criteria](#acceptance-criteria)
* [Comments](#comments)

## Story List

The following major story categories have been identified:

1. [Development and System Building](#development-and-system-building)
	1. [Multiple Applications](#multiple-applications)
2. [Runtime](#runtime)
3. [Debugging](#debugging)
4. [Documentation](#documentation)

The following are general stories which did not fit in the above categories:

* As a developer, I want a user-friendly interface for configuring the framework
* As a developer, I want to write and test software on my host machine while boards are still being built
* As a developer, I want to write and run unit tests for my code
* As a developer, I want to reuse as much code as possible across projects

### Development and System Building

* As a developer, I want to define my own power management scheme for my product
* As a developer, I want to support multiple board revisions with the same program
* As a developer, I want to write a program which does not use an RTOS/threads
* As a developer, I want to select an RTOS for my platform
* As a developer, I want to add support for a new RTOS
* As a developer, I want to add support for a new platform
* As a developer, I want to add support for a new processor
* As a developer, I want to add support for a new chip family (vendor)
* As a developer, I want to add support for a new processor architecture
* As a developer, I want to write a new device driver for an existing type
* As a developer, I want to write a new device driver for a new category of driver
	* i.e., create a new driver abstract type
* As a developer, I want to add a device driver for my debug adapter (e.g., USB-to-SPI)
* As a developer, I want to swap out one component with another of the same category
	* Platform changes type of accelerometer
* As a developer, I want to configure interrupt priorities
* As a developer, I want to configure thread priorities
* As a developer, I want to enable and disable interrupts
* As a developer, I want to update the tuning options & configuration values for the framework

#### Multiple Applications

* As a developer, I want to compile the same application for two different platforms
* As a developer, I want to write multiple applications for one platform
* As a developer, I want to manage multiple projects (builds) within one repository
* As a developer, I want to create recovery firmware that can update itself ("recovery mode") so that devices aren't bricked after a failed update
* As a developer, I want to support both manufacturing and customer firmware for the same platform so that I can reuse as much code as possible
* As a developer, I want to implement support for a multi-stage boot process
	* Required by some machines & application sizes
	* Secure boot is possible
	* Hypervisor
	* Initialize DRAM then load into DRAM

### Runtime

* As a developer, I want the system to automatically boot up and start running my application
* As a developer, I want to be able to change the system power state at runtime
* As a developer, I want a way to access a specific device driver of a given type (spi, acel, etc.)
* As a developer, I want a way to access all device drivers of a given type (spi, acel, etc.)
* As a developer, I want the option of reconfiguring my drivers and timers during runtime so that I can support complex behaviors

### Debugging

* As a developer, I want to log messages to a buffer so that I can trace my program's execution
* As a developer, I want to selectively turn on logging so that I can debug
* As a developer, I want to selectively control the level of different logging messages
* As a developer, I want to receive panic logs
* As a developer, I want a tool to symbolicate my panic logs so that I can understand what they mean

### Documentation

* As a developer, I want a way to learn about the extension/variability points in the framework
* As a developer, I want to understand what the tuning options & configuration values are for the framework
* As a developer, I want to plan/know the order of module implementation and supporting systems for the feature I am working on
	* e.g., architecture dependency view, work assignment views
* As a developer, I want to review example code so that I can understand the capabilities of the system and how the system should be used
* As an EA developer, I want a command to auto-generate framework documentation so that I can:
	* test documentation changes
	* have access to the documentation on my machine
* As a client developer, I want to generate documentation either with or without the framework documentation included

## Acceptance Criteria

TBD

## Associated Mechanisms

* System Power Management
* Configuration management (framework)
* Build system
* Unit test framework
* Host simulation
* Board revision context control
* RTOS/Threads (priority control)
* Driver model
* Procesor model
* Platform model
* Interrupts (priority control, enable/disable)
* Cross-compilation
* Flexible boot procedures
* Document generation
* Panic/assert
* Symbolication
* Logging
* Timer model
* Searchable documentation

## Comments

> As a developer, I want to support both manufacturing and customer firmware for the same platform so that I can reuse as much code as possible

* Can we ship with manufacturing firmware support as a free subsystem with command line support? You just define the platform per our documented rules and boot the manufacturing firmware?
* How can we enable the client to add their own tests?

> As a developer, I want to write a program which does not use an RTOS/threads

* This means we can't use synchronous queues to protect the hardware in the HAL
* What do we do for a mutex with no RTOS? implement our own lightweight lock?
