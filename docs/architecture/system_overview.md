# System Overview

This document provides an overview for the Embedded Virtual Machine (VM) frameworks.

**Table of Contents:**
1. [Motivation](#motivation)
2. [Scope Summary](#scope-summary)
    1. [Embedded VM Core Scope](#embedded-vm-core-scope)
    2. [Phase 1 Platform Bring-up](#phase-1-platform-bring-up)
    2. [Phase 2 Subsystem Scope](#phase-2-subsystem-scope)
    3. [Ongoing Platform Expansion](#ongoing-platform-expansion)
3. [Functionality](#functionality)
    1. [Key Capabilities](#key-capabilities)
    2. [High-level Use Cases](#high-level-use-cases)
4. [Architectural Challenges](#architectural-challenges)
5. [Quality Attribute Requirements](#quality-attribute-requirements)
    1. [Key Quality Attribute Scenarios](#key-quality-attribute-scenarios)

## Motivation

As of July 2018, the embedded systems world lacks a common design language and set of common abstractions. Every embedded system design is bespoke, and companies continually reinvent the wheel. Many companies shipping embedded products build upon vendor-provided SDKs. Few companies are building their software on top of abstraction layers or generalized interfaces.

The lack of abstraction layers in firmware applications results in programs that are tightly coupled to the underlying processors, boards, and electrical components. When companies need to update their platforms to use new processors or electrical components, the resulting firmware changes can be catastrophic. Entire applications often need to be rewritten and ported to a new vendor SDK and new RTOS. With chips regularly reaching end-of-life and the increasing supply chain volatility around our systems, we must invest resources into keeping our firmware decoupled from the underlying hardware.

The tight coupling between hardware and software manifests itself in other damaging ways:

* Developers rarely write modular code, reducing the potential for reuse on other platforms & products.
* Unit tests are rarely utilized on embedded products, since drivers and modules cannot be decoupled from the hardware dependencies, underlying RTOS, or vendor SDKs
* Due to the typical tight coupling between firmware and hardware, development efforts often do not begin until hardware is in house (as a result, firmware is _already late_)

By decoupling firmware from the underlying hardware, a brave new world is open to embedded teams:

* Unit tests can be written and run on a host machine or CI server
* Host-machine drivers can be written to interact with USB protocol adapters (e.g., Aaarvark I2C/SPI adapter)
    * Drivers can be developed on a host machine by using the protocol adapter to talk directly to development kits or breakout boards
    * These drivers can be ported to any firmware platform that supports the underlying communication protocol
    * Developing drivers on a host machine allows for a much shorter code-compile-test loop

There is no need for each company to recreate an embedded systems framework in order to ship their product. Instead, companies and developers would benefit from a portable and reusable framework. We believe a reusable design solution is beneficial because:

* By designing our systems to limit platform dependencies, we reduce the schedule impact of underlying hardware changes
* Quality designs enhance agility and efficiency, rather than hindering it
* Frameworks improve developer productivity and improve the quality, reliability, and robustness of new software
* Hierarchical design gives a system stability and resilience, and also reduces the amount of information that developers need to keep in mind
* Companies building embedded products can focus on their unique application features and core business value rather than spending time rebuilding infrastructure for a new platform
* Companies and developers both benefit from improved productivity, shorter code-compile-test cycles, and the ability to develop firmware and drivers asynchronously from hardware schedules

## Scope Summary

Version 1 of the framework consists of the [Framework Core](#framework-core-scope) and [Phase 1 Platform Bring-up](#phase-1-platform-bring-up). After these two phases of development are complete, the framework can be used on client projects.

As the framework development project is open-ended, a variety of commonly required features will be produced during [Phase 2](#phase-2-subsystem-scope). Additional drivers, interfaces, processors, and utilities will be developed on an [ongoing basis](#ongoing-platform-expansion).

### Embedded VM Core Scope

A [build system](components/build_system.md) will be created to:

* Compile the framework and client applications
* Compile unit tests
* Run unit tests
* Perform static analysis
* Run clang-format

The embedded framework core scope includes:

* [Virtual RTOS Abstraction](components/core/virtual_rtos.md)
* [Virtual Platform Abstraction](components/core/virtual_platform.md)
* [Virtual Hardware Platform Abstraction](components/core/virtual_hardware_platform.md)
* [Virtual Processor Abstraction](components/core/virtual_processor.md)
    * [Virtual Architecture Abstractions](components/core/virtual_processor_architecture.md)
    * [Register Abstraction](components/core/register.md)
* [Driver](components/core/driver.md) Model, including:
    * [Driver Registry](components/core/driver_registry.md)
    * Generic [Driver](components/core/driver.md) Interfaces for:
        * UART/USART
        * GPIO
        * I2C
        * ADC
        * SPI
        * DMA
        * Timer
    * [HAL Driver](components/core/hal_driver.md)
* [Timer Manager](components/core/timer_manager.md)
* [Boot Sequencer](components/core/boot_sequencer.md)
* [Event Center](components/core/event_center.md)

The embedded framework will also provide core utilities for developers:

* [Interrupt Queue](components/utilties/interrupt_queue.md) (Bottom Half Handler)
* [Dispatch Queue](components/utilities/dispatch_queue.md)
    * Asynchronous
    * Synchronous
    * Static (no dynamic allocation)
* [Static `std::function` equivalent](components/utilities/static_function.md)
* [Static `std::queue` equivalent](components/utilities/static_queue.md)
* [Templated Buffer container](components/utilities/templated_buffer.md)
* [Templated Buffer Pool](components/utilities/templated_buffer_pool.md)
* [Templated Queue](components/utilities/templated_queue.md)

The core will also ship with select libraries to support initial development efforts, such as a reusable[Logging](components/subsystems/logger.md) subsystem.

The framework will be structured to automatically sequence the boot process, with hooks for clients to customize platform-specific behavior.

The framework will automatically invoke the client application as the final step in the boot process.

## Functionality

The framework provides a re-usable design that can be applied to a wide variety of embedded systems. The framework skeleton can be customized and extended by users to support the needs of their platform. The framework dictates the architecture of the application, defining the overall structure, its partitioning into classes and objects, the key responsibilities thereof, and how the key classes and objects collaborate.

A build system is provided which can compile framework code.

A processor skeleton with a selection of host adapter drivers is provided to enable developers to test code on their host machine.

All framework documentation will be maintained alongside source code. Full documentation which includes in-source documentation can be generated by the build system.

### Key Capabilities

* C++ support
    * Runtime available to developers if desired
    * C++ extensions provide STL equivalents which utilize static memory allocation and no exceptions
* C runtime support (limited scope for embedded)
    * C interface shims
* Core components utilize only static memory allocation
* Decoupling between software and the underlying hardware
* Extensible abstractions for platforms, boards, processors, and RTOSes
* Extensible and reusable driver model
    * Centralized management of system drivers
    * Generic interfaces for categories of drivers (USART, SPI, accelerometer, temperature sensor)
* Event-driven interfaces and development model
* Support for single-threaded development (bare metal, no RTOS)
* Support for multi-threaded development styles:
    * RTOS abstraction layer, allowing a variety of RTOSes to be used as drop-in replacements
    * Dispatch queue support
    * C++ std::thread support
    * Coroutine support (Future)
* Support for dynamic memory allocation if configured
* System Power Management scheme
    * Default scheme provided
    * Override-able by end applications for custom implementations
* Automatic boot sequencing
    * Default scheme provided
    * Override-able by end applications for custom implementations
* Fault (panic) handling with backtraces and processor information
* Centralized management of generic timer resources
* Logging
* Parametric data collection
* Command-line interface
* Built-in memory test capabilities

### Feature Tree

<center>

| ![Feature tree for the embedded framework.](images/FeatureModel.png) |
|:--:|
| *Figure 1. Feature tree for the embedded framework.* |

</center>

### High-level Use Cases

The framework must meet the needs of the following actor categories (shown in *Figure 2*):

1. Developers
    * Internal developers, who will extend and support the framework
    * Client developers, who will build their applications using the framework
2. Project managers
    * Will use framework documentation to create and track project schedules
3. CI Servers
    * Will interact with the framework to generate images, run unit tests, and perform static analysis

<center>

| ![Key actors in framework use cases](images/KeyActors.jpg) |
|:--:|
| *Figure 2. Key actors participating in the Embedded Framework ecosystem.* |

</center>

*Figure 3* shows high-level framework core use cases. Detailed user stories are available for each category of actor:

* [General User Stories](use_cases/0004-user_stories_general.md)
* [Developer User Stories](use_cases/0001-user_stories_developer.md)
* [Project Manager User Stories](use_cases/0002-user_stories_project_manager.md)
* [CI Server User Stories](use_cases/0003-user_stories_ci_server.md)

<center>

| ![High-level framework use cases](images/HighLevelUseCases.jpg) |
|:--:|
| *Figure 3. View of high-level use cases for the Embedded Framework.* |

</center>

## Architectural Challenges

The major architectural challenges of the framework are:

1. Picking the Right Abstractions
	1. Processor & Processor Architecture Abstractions
	2. HAL Driver Abstractions
	3. RTOS Abstractions
	4. Platform Abstractions
	5. Driver (Generics) Abstractions
2. Creating a flexible System Power Manager which can be overriden to provide user behavior
3. Properly constructing all APIs to support event-driven programs
4. Keeping the framework low-latency, allowing it to be used in devices with tight timing constraints
5. Keeping the framework abstractions and usages simple in order to make it accessible to a wide audience of developers

These areas should be given careful consideration and top design priority.

## Quality Attribute Requirements

The framework’s main contribution to the end application is the architecture we provide. Therefore, it’s imperative to design the framework to meet the following quality requirements.

The framework's qualities can be divided into two categories:

1. Evolution Qualities, which are embodied in the static structure of the framework
2. Execution Qualities, which are observable during framework operation.

The framework's key evolution quality requirements are:

* [Resilience](qualities/0003-resilience.md)
    * [Portability](qualities/0008-portability.md)
        * OSes
        * Processors
        * Drivers
    * [Extensibility](qualities/0004-extensibility.md)
* [Productivity](qualities/0001-productivity.md)
    * [Stability](qualities/0005-stability.md)
    * [Reusability](qualities/0011-reusability.md)
    * [Documentation](qualities/0010-documentation.md)
* [Maintainability](qualities/0013-maintainability.md)
* [Reliability](qualities/0002-reliability.md)
* [Testability](qualities/0009-testability.md)

The framework's key execution quality requirements are:

* [Robustness](qualities/0006-robustness.md)
* [Security](qualities/0007-security.md)
* [Serviceability](qualities/0012-serviceability.md)

<center>

| ![Taxonomy of Embedded Framework Quality Attributes](images/NonFunctionalRequirementRelationships.jpg) |
|:--:|
| *Figure 3. Taxonomy of Embedded Framework Quality Attributes.* |

</center>

### Key Quality Attribute Scenarios

**TBD: Define Quality Attribute Scenarios**

* What are the important quality attributes and Quality Attribute Scenarios (QAS)?

 Example:

 ```
 The quality attribute scenarios (QAS) are listed below, separated by quality attribute.
Modifiability
* QAS1. A new business partner (airline, lodging, or activity provider) that uses its own web services interface is added to the system in no more than 10 person-days of effort for the implementation. The business goal is easy integration with new business partners.
Performance
* QAS2. A user places an order for an adventure travel package to the Consumer Web site. The user is notified on screen that the order has been successfully submitted and is being processed in less than five seconds.
* QAS3. Up to 500 users click to see the catalog of adventure packages following a random distribution over 1 minute; the system is under normal operating conditions; the maximal latency to serve the first page of content is under 5 seconds; average latency for same is less than 2 seconds.
Reliability
* QAS4. The Consumer Web site sent a purchase order request to the order processing center (OPC). The OPC processed that request but didn’t reply to Consumer Web site within five seconds, so the Consumer Web site resends the request to the OPC. The OPC receives the duplicate request, but the consumer is not double-charged, data remains in a consistent state, and the Consumer Web site is notified that the original request was successful in one hundred percent of the times.
Security
* QAS5. Credit approval and payment processing are requested for a new order. In one hundred percent of the cases, the transaction is completed securely and cannot be repudiated by either party. The business goals are to provide customers and business partners confidence in security and to meet contractual, legal, and regulatory obligations for secure credit transactions.
* QAS6. The OPC experiences a flood of calls through the Web Service Broker endpoint that do not correspond to any current orders. In one hundred percent of the times, the system detects the abnormal level of activity, notifies the system administrator, and continues to service requests in a degraded mode.
Availability
* QAS7. The Consumer Web site is available to the user 24x7. If an instance of OPC application fails, the fault is detected and the system administrator is notified in 30 seconds; the system continues taking order requests; another OPC instance is created; and data remains in consistent state.
```
