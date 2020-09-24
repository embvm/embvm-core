# 2. Layering Scheme

Date: 2018-07-06

## Status

Proposed

## Context

The framework is being designed to provide portability for embedded systems applications. We also want to maximize reuse of software across embedded products to minimize bring-up time and to increase our confidence in the software.

In order to accomplish this, we must think about how to isolate the various platform-specific forces of change that occur within embedded products:

* The processor
* The layout/connections on a given board
* The memory availability
* The component ICs that are included on the board

By isolating these various forces of change and platform dependence, we can improve the possibility of re-use on embedded platforms.

## Decision

The architecture is structured in three layers:

1. Virtual Processor Layer (Base)
2. Virtual Platform Layer
3. Software Layer

The virtual processor layer is a standalone layer which manages the chip and HAL.

The virtual platform layer contains platform-specific implementations for the C/C++ runtime, RTOS, and board/platforms.

The software layer contains standalone software which is built to use the virtual platform abstractions.

## Virtual Processor Layer

The virtual processor layer provides the abstractions for a specific processor. It comprises of chip-specific startup code, HAL drivers, and a virtual platform abstraction. The virtual platform abstracts a board and processor combination.

The virtual processor layer cannot call into layers above it. It must operate standalone.

The virtual processor layer is responsible for managing the chip hardware devices and keeping them happy.

TBD: C/C++ language usage is allowed after chip initialization.
TBD: Use of some utilities is allowed after initialization (static_function, static_queue, interrupt queue, serial dispatch queue)
TBD: How do we identify these constructs?

## Virtual Platform Layer

The virtual platform layer contains platform-dependent abstractions:

* Platform (mediates between processor, drivers, and software layer)
* RTOS
* C/C++ Runtime setup

The virtual platform layer also contains the device driver definitions.

The virtual platform layer constructs can reference each other, and can call funtions in the virtual processor.

The virtual platform layer provides abstractions to the overall platform (board + chip combination). The platform creates the generic device drivers which can be used by the software application.

The virtual platform layer should not make any calls into the software layer. If interaction is needed, user-specific platform functions should be defined. The software layer should call these functions in the platform layer. Callbacks can be used if necessary.

## Software Layer

The software layer contains software abstractions and subsystems which are not dependent on the specific underlying software.

From the client side, this is the layer where the client application logic and client libraries are integrated.

Embedded Artistry reusable Subsystems and Utilities are also included in the software layer.

The software layer is allowed to talk to other software components as well as the virtual platform layer. The software layer should not talk directly to the virtual processor layer, as that introduces coupling between the software and the processor.

## Consequences

If this layering scheme is followed, we maintain a flexible and reusable design:

* Chip code can be re-used by differnet platforms
* Software is written to a set of virtual platform abstractions
	* As long as those abstractions are maintained and supported on alternative platforms, software can be ported
* Software uses generic driver interfaces which are not tied to specific parts or vendors
* Software and platform use constructs which are portable across architectures

Careful design consideration needs to be given to the implementation of the virtual processor abstractions. How can we create proper generic interfaces covering a variety of chip families and processor implementations? How can we keep the HAL encapsulated in a sane away?

Layering needs to be enforced or else the software will devolve into a tightly coupled ball of mud. By allowing the software layer to use the processor layer, the software regains dependency on the specific processor.

We don't want to add unnecessary overhead: consider how to utilize inline calls and constexpr rather than adding additional indirection to every function.
