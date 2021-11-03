# Architectural Layer View

The Layer View abstracts the embedded framework into a layered architecture. The framework decomposes into three layers:

1. [Processor Layer](#processor-layer), which abstracts the underlying processor used by the target platform
2. [Platform Layer](#platform-layer), which abstracts the underlying circuit board, operating system, and C/C++ runtime
3. [Software Layer](#software-layer) sits atop the Platform Layer, which allows it to be portable across platforms (given that requirements are met by the platform implementation)

In addition, there is a cross-cutting [Utilities](#utilities) "layer" which is independent of the target platform & architecture. The Utility constructs are usable by all layers.

The primary goal of the framework is to create re-usable and portable device drivers and software constructs for embedded devices and product lines. In order to achieve this goal, the software and hardware segments must be designed to be decoupled and interchangeable at the three layering points.

**Table of Contents:**

1. [Primary Presentation](#primary-presentation)
2. [Context Diagram](#context-diagram)
3. [Secondary Presentations](#secondary-presentations)
4. [Element Catalog](#element-catalog)
5. [Variability Guide](#variability-guide)
6. [Rationale](#rationale)
	1. [Allowed-to-use](#allowed-to-use)
	2. [Dependency Problems the Framework Addresses](#dependency-problems-the-framework-addresses)
		1. [Processor Dependencies](#processor-dependencies)
		2. [Platform Dependencies](#platform-dependencies)
		3. [Component Dependencies](#component-dependencies)
		4. [RTOS Dependencies](#rtos-dependencies)
7. [Related Views](#related-views)

## Primary Presentation

<center>

| ![Layered view of the framework system](LayerView.png) |
|:--:|
| *Figure 1. Layered View for Embedded Framework.* |

</center>

## Context Diagram

The layered view above represents the blue **Embedded Framework** block in the image below. The *external device* blocks represent example pieces of hardware that the software system interacts with. The **Embedded Framework** invokes the **Client Application**, which is an abstract concept represented as part of the **Software** layer.

<center>

| ![Context for the Software System Layering](Context.jpg) |
|:--:|
| *Figure 3. Software System Context Diagram for Embedded Framework* |

</center>

## Secondary Presentations

The layers of the framework communicate in different ways:

* The Processor Layer receives and handles hardware interrupts
* The peripheral driver may communicate to a Platform Layer driver through transactions or queues
* A peripheral driver may be the same as a device driver (e.g.,, SPI bus peripheral), in which case the data is simply passed to the Software layer directly from the peripheral
* The Software Layer receives data from drivers through callbacks and event notifications

<center>

| ![LayerCommunicationMechanisms](LayerCommunicationMechanisms.jpg) |
|:--:|
| *Figure 2. The communication mechanisms used to pass data between the layers* |

</center>

## Element Catalog

The following elements comprise the layered view:

1. [Processor Layer](#processor-layer)
	2. [Architecture Interfaces](#architecture-interfaces)
	3. [Boot Handler](#boot-handler)
2. [Platform Layer](#platform-layer)
	1. [Virtual Hardware Platform](#virtual-hardware-platform)
	2. [Virtual RTOS](#virtual-rtos)
	3. [Driver Model](#driver-model)
	4. [Language Runtime](#language-runtime)
	5. [Boot Sequencer](#boot-sequencer)
	6. [Timer Manager](#timer-manager)
3. [Software Layer](#software-layer)
	1. [Client Software](#client-software)
	2. [Subsystems](#subsystems)
	3. [Libraries](#libraries)
4. [Utilities](#utilities)
	1. [C++ Extensions](#c-extensions)

The [Domain Context Model](../../domain_context.md) provides additional information about the concepts discussed in this overview.

### Processor Layer

The Processor Layer abstracts the underlying processor implementation. A [Platform](#platform-layer) contains a processor and is responsible for the user's desired configuration of the processor and its peripherals.

The Processor Layer is composed of the following components:

* [Architecture Interfaces](#architecture-interfaces)
* [Boot Handler](#boot-handler)

#### Architecture Interfaces

A processor is implemented using an underlying architecture, such as PIC, AVR, MIPS, ARMv8. Each architecture provides a set of common features and instruction sets that are available for the platform. Additionally, features such as caching and MMU support are contained within the architectural interfaces.

#### Boot Handler

When power is applied to an embedded system, the first context of control in a program is the processor. Each processor has a specific startup sequence, startup requirements, and limitations that apply. The processor-specific startup code executes before transferring control to the [Platform](#platform-layer) [Boot Sequencer](#boot-sequencer) to continue the boot process.

**Processor start-up is the only exception to the layering "allowed-to-use" rules.**

### Platform Layer

The Platform Layer abstracts the [Software Layer](#software-layer) from the underlying hardware/OS platform. The Platform Layer represents a set of platform interfaces which provide specific capabilities and responsibilities, covering both capabilities provided by the hardware and (optionally) the RTOS. The Platform Layer also provides abstract interfaces to the [Device Drivers](#driver-model) available on the platform.

The Platform Layer provides a set of abstracted interfaces. The underlying abstractions are:

1. [Virtual Hardware Platform](#virtual-hardware-platform)
2. [Virtual RTOS](#virtual-rtos)
3. [Driver Model](#driver-model)
4. [Language Runtime](#language-runtime)

Platform abstractions are not fixed. Users can define their own Platform Abstractions and reuse them on multiple platform implementations.

#### Virtual Hardware Platform (HAL)

The Virtual Hardware Platform abstracts away the underlying hardware (e.g., circuit board) and the [Processor Layer](#processor-layer). The job of the Virtual Hardware Platform is to decouple the software from the hardware details. Board-specific functionality should be encapsulated in generic drivers and generic APIs, allowing the software to be decoupled from underlying hardware changes. This is what sets the framework apart from most embedded system designs.

Externally, a Virtual Hardware Platform provides a generalized set of capabilities and functionality to the higher-level software.

Internally, the Virtual Hardware Platform implementation functions can be implemented in multiple ways and using multiple hardware components. The Virtual Hardware Platform implementation is where:

* The processor that will be used is decided upon
* GPIOs are mapped and input to platform interfaces or drivers
* Peripheral and clock settings are applied
* Drivers are created and configured

The Virtual Hardware Platform interacts with the [Processor Layer](#processor-layer), and may provide access to higher levels of software by providing generic interfaces.

#### Virtual RTOS

The Virtual RTOS is an abstraction layer which provides a standardized interface to the underlying RTOS.

An RTOS may or may not be used on a given platform implementation.

Because the RTOS is dependent (in most cases) on the underlying processor, it must be encapsulated within and initialized by the [Platform Layer](#platform-layer).

#### Driver Model

The Driver Model provides two critical system pieces:

1. Definitions for generic driver interfaces
2. A `DriverRegistry` which can be used to access drivers available on a platform without being coupled to the specific underlying device

The [Virtual Hardware Platform](#virtual-hardware-platform) is responsible for initializing the devices that can be used by the system. Drivers which should be publicly available (via their abstract interfaces) to the rest of the program should be registered in the Virtual Hardware Platform using the Driver Registry.

Hardware Abstraction Layer (HAL) Drivers are a specialization of the [Driver](#driver-model) concept. HAL Drivers work directly with the processor peripheral hardware. HAL Drivers have additional responsibilities that generic drivers lack, such as:

* Often requires a memory-mapped register address which corresponds to the device's hardware interface
* May or may not support DMA
* May or may not support interrupts
* May or may not support clock configuration
* May or may not support error conditions and recovery mechanisms

HAL drivers still work with the [Driver Model](#driver-model). HAL drivers provide generic interfaces to higher-level consumers through the [Driver Registry](#driver-registry).

The [Virtual Hardware Platform](#virtual-hardware-platform) is responsible for initializing and configuring the HAL drivers required by the platform.

#### Language Runtime

Some C and C++ features are dependent upon the underlying [Processor Layer](#processor-layer) architecture and [RTOS](#virtual-rtos) support. These features are contained and configured within the [Platform](#platform-layer) layer.

For example, `std::mutex` requires an RTOS in order to provide mutex support.

#### Boot Sequencer

The Boot Sequencer receives control from the [Processor Layer](#processor-layer)'s [Boot Handler](#boot-handler).

The Boot Sequencer is responsible for initializing the [Virtual Hardware Platform](#virtual-hardware-platform), [RTOS](#virtual-rtos), and [Language Runtime](#language-runtime).

#### Timer Manager

The Timer Manager provides a generic interface for the [Platform Layer](#platform-layer) and [Software Layer](#software-layer) to create, manage, and use timers.

### Software Layer

The Software Layer represents portable software which can be run on any [platform](#platform-layer) (assuming the platform meets the requirements and provides the necessary interfaces).

The Software Layer can be broken down into three categories:

* [Client Software](#client-software)
* [Subsystems](#subsystems)
* [Libraries](#libraries)

The code within the Software Layer must not be coupled to a specific hardware implementation, although platform dependencies can be noted. For example, applications or subsystems may require that a platform has an external storage device or that the platform is using an RTOS.

The Software Layer communicates with hardware using the [Platform](#platform-layer) interfaces and the [Driver Model](#driver-model). The Software Layer is allowed to use standard C/C++ functions and [Utilities](#utilities).

#### Client Software

Client Software is a generalization of the end-application that is run by the framework. The framework is designed to support any number of client applications in many development styles.

Client Software contains the use-case-specific logic and core business IP for each application. By developing the end application in a platform-independent manner, software becomes more robust to underlying hardware changes.

#### Subsystems

Subsystems are reusable software modules which perform standalone functional operations within the larger software system. These subsystems are typically active in nature, requiring their own threads or regular processing time.

Subsystems are decompositions of typical system functionality, encapsulating features such as:

* System power management
* Firmware updates
* Over-the-Air (OTA) updates
* Command line

Subsytems should be designed in a way that makes them reusable across software systems. Some subsystems may have requirements of the underlying platform, such as a wireless interface for OTA updates.

#### Libraries

Libraries are also reusable software modules. Libraries often provide a specific set of functionality and differ from [Subsystems](#subsystems) in that they are not typically active modules. A library is often used as a toolkit, while the higher-level software coordinates the function calls in order to accomplish a specific task.

### Utilities

Utilities represent cross-cutting constructs that can be used by all of the layers without creating specific platform dependencies. These utilities are typically small modules which provide core features. Examples include `Buffer<>`, `BufferPool<>`, `DispatchQueue`, and `StaticDispatchQueue`.

All layers are allowed to use Utility constructs.

#### C++ Extensions

Many of the C++ language features require exceptions and dynamic memory. Alternatives will be created for use in the framework core components, such as a `static_queue` and a static variant of `std::function`. These extensions will not rely on dynamic memory, RTTI (Run-time Type Information), or exceptions, ensuring that they are portable and usable across embedded systems.

These extensions will be used in the [Processor Layer](#processor-layer) & [Platform Layer](#platform-layer) to prevent dependence on an [RTOS](#virtual-rtos).

## Variability Guide

The following variability options are available during compile-time and software development:

* A [Platform](#platform-layer) can support any number of [Software Layers](#software-layer)
* The Software Layer can be ported to any Platform which provides the required interface
* Additional subsystems may be added to the [Software Layer](#software-layer)
* Additional libraries may be added to the [Software Layer](#software-layer)
* Additional RTOS implementations may be added as long as it conforms to the [Virtual RTOS](#virtual-rtos) interfaces
* A [Virtual Hardware Platform](#virtual-hardware-platform) encapsulates hardware capabilities and responsibilities, and additional platforms can be swapped in as long as the [Virtual Hardware Platform](#virtual-hardware-platform) interfaces and guarantees are satisfied
* Additional processors may be added by implementing a new [Processor Layer](#processor-layer) model
* Additional processor architectures can be added, as long as a new abstract interface for the architecture is defined
* Additional drivers may be added and used by the [Virtual Hardware Platform](#virtual-hardware-platform) and [Processor Layer](#processor-layer)
* Additional types of drivers may be supported by defining a new abstract interface for that type
* Developers can override the default System Power Management behavior to suit the needs of their platform
* Developers can override the default [Boot Sequencer](#boot-sequencer) to suit the needs of their platform

## Rationale

The most difficult aspect about keeping embedded systems software portable and re-usable is the dependence upon the underlying hardware platform. Each embedded system is unique in the combination of processor and peripherals. When developing embedded software, if an abstraction layer is not used, your program becomes coupled to the processor, the circuit board, and the RTOS. In many cases, migrating to a new processor or circuit board triggers large-scale software rewrites and schedule delays.

There are four primary ways that embedded software becomes coupled:

1. [Processor dependencies](#processor-dependencies)
2. [Platform/board dependencies](#platform-dependencies)
3. [Driver/Component dependencies](component-dependencies)
4. [RTOS dependencies](#rtos-dependencies)

By abstracting away these dependencies, our software becomes portable and layers can be interchanged as needed.

The [Software Layer](#software-layer) represents re-usable behavioral logic which is implemented in a platform-independent manner. The Software Layer interacts with hardware using the [Driver Model](#driver-model), [Virtual RTOS](#virtual-rtos), and [Virtual Hardware Platform](#virtual-hardware-platform). By interacting with these interface-based components rather than directly with the process or hardware, the software is kept hardware-independent. The [Platform](#platform-layer) enables software to be written against a virtual machine, rather than against a specific board design.

The [Software Layer](#software-layer) components may be dependent upon platform capabilities/APIs. Dependency on a platform is supported through the use of client-created [Platform](#platform-layer) interfaces and [Virtual Hardware Platform](#virtual-hardware-platform) interfaces. Multiple platforms can utilize the same interfaces, allowing client software to quickly migrate to new hardware that provides equivalent functionality.

The [Virtual RTOS](#virtual-rtos) prevents the [Software Layer](#software-layer) from becoming tightly coupled to a specific RTOS. The use of an RTOS is optional.

The Virtual Hardware Platform is the [Mediator](../../patterns/mediator.md) for the hardware components on the board. The Virtual Hardware Platform contains a [Processor Layer](#processor-layer) and decouples the system from depending on a specific processor. Hardware components are made available through the [Driver Model](#driver-model), which allows software to work with abstract interfaces rather than specific implementations.

### Allowed-to-Use

Aside from [Utilities](#utilities), each layer is allowed to use the next layer below it. This means that the [Software Layer](#software-layer) is not allowed to talk to the [Processor Layer](#processor-layer). Any layer is allowed to use the [Utilities](#utilities).

Layers cannot call up the stack, meaning that the Processor cannot call [Platform](#platform-layer) functions, and the [Platform](#platform-layer) cannot call Software Layer functions.

The allowed-to-use relationships are shown in the [Primary Presentation](#primary-presentation) of the layer view.

**The only exception to these rules** is during the boot process. The boot process begins in the Processor Layer (see [Boot Handler](#boot-handler)), and the Processor transfers control to the Platform Layer after the initial chip setup process is completed. The Platform Layer is then responsible for driving the remainder of the boot process. This exception allows each processor to initialize itself before handing control over to the system. By using a generalized boot process, the [Platform Layer](#platform-layer) remains decoupled from the [Processor](#virtual-processor). This eliminates a common [processor dependency](#processor-dependency).

### Dependency Problems the Framework Addresses

The framework is designed to address four common sources of dependencies that tightly couple embedded software to the underlying platform:

1. [Processor Dependencies](#processor-dependencies)
2. [Platform Dependencies](#platform-dependencies)
3. [Component Dependencies](#component-dependencies)
4. [RTOS Dependencies](#rtos-dependencies)

### Processor Dependencies

Processor dependencies are the most common form of coupling and arise from two major sources:

1. Using Vendor SDKs without an abstraction layer
2. Using APIs or libraries which are coupled to a target architecture (e.g.,, [CMSIS](https://developer.arm.com/embedded/cmsis))

Processor-level function calls are commonly intermixed with application logic and driver code, ensuring that the software becomes tightly coupled to the processor. De-coupling firmware from the underlying processor is one of the most important aspects when designing for portability and reusability.

In the most common cases, teams will develop software using a vendor's development kit (SDK). When the team is required to migrate to another processor or vendor (due to cost, power, end-of-life), the coupling to a specific vendor's SDK often triggers a rewrite of the majority of the system. At this point, many teams realize the need for abstraction layers and begin to implement them on their own.

In other cases, software becomes dependent upon the underlying architecture. Your embedded software may work on an ARM system, but not be readily portable to a PIC or x86 machine. This is common when utilizing libraries such as [CMSIS](https://developer.arm.com/embedded/cmsis), which provides an abstraction layer for ARM Cortex-M processors.

In other cases, abstraction layers and generic driver interfaces are developed by teams. However, these teams can create dependencies on a specific style of processor with their abstraction layer. For example, a model may require that all drivers support DMA / interrupts / have specific interaction requirements. Moving to another processor line or vendor can bring these assumptions to light.

### Platform Dependencies

Embedded software is often written specifically for the underlying hardware platform. Rather than abstracting platform-specific functionality, embedded software often interacts directly with the hardware. Whenever a new board revision is released, the software logic must be updated to support the changes. Supporting multiple board revisions often leads to conditional logic statements scattered throughout the software. Porting the software to another platform requires finding and replacing all of the board-specific calls.

By abstracting platform/board functionality behind an interface, multiple platforms & boards can be created that provide the same set of functionality and responsibilities in different ways. If the [Software Layer](#software-layer) is built upon the [Platform Interface](#virtual-platform-interface), then different Platform Layers can be used with the same software layer.

[Component Dependencies](#component-dependencies) are a specialization of the platform dependency, where software relies on the presence of a specific hardware component.

### Component Dependencies

In embedded systems, software is often written to use specific driver implementations rather than generalized interfaces. This means that instead of using a generalized accelerometer interface, software typically works directly with a [BMA280](https://www.bosch-sensortec.com/bst/products/all_products/bma280) or [LIS3DH](https://www.st.com/en/mems-and-sensors/lis3dh.html) driver. Whenever the component changes, the software must be updated to use the new driver, or conditionals are added to use the proper driver for the proper board revision.

Higher-level software can be decoupled from components by working with generic interfaces rather than specific drivers. When working with generic interfaces, underlying components can be swapped out without the higher-level software being aware. The [Virtual Hardware Platform](#virtual-hardware-platform) is responsible for handling configuration of peripherals to provide platform-specific guarantees.

### RTOS Dependencies

An RTOS's functions are commonly used directly by embedded software. When a processor change occurs, the team may find that the RTOS they were previously using is not supported on the new processor.

Migrating from one RTOS to another requires a painful porting process, as there are rarely straightforward mappings between the functionality and usage of two different RTOSes.

Providing an RTOS abstraction allows platforms to use any RTOS that they choose without coupling their application software to the RTOS implementation.

Abstracting the RTOS APIs also allows for host-machine simulation, since we can provide a [pthread](https://en.wikipedia.org/wiki/POSIX_Threads) implementation for our RTOS abstraction.

## Related Views

The context diagram for the framework software is pulled from the [Structural View](../structural/structural_view.md). The Structural View shows the system from a Systems Engineering perspective and a Software Engineering perspective.

The [Conceptual View](../conceptual_architecture/conceptual_view.md) shows the connections between the components and layers identified here.
