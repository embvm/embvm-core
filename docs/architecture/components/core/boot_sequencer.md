# CRC-R: Boot Sequencer

The Boot Sequencer handles the initial system setup process in a deterministic manner.

**Status:** Completed, with multiple default boot strategies provided to users

## Responsibilities

* Boot Sequencer invokes the [C/C++ runtime startup process](c_cpp_runtime.md)
* Boot Sequencer calls Virtual Platform functions to initialize the processor, platform, and OS
* Boot Sequencer invokes `main()` after the boot process is complete

## Requirements

* Boot Sequencer must provide hooks for running early-stage platform code to enable users to configure DRAM before relocations & zeroing
* Boot Sequencer should provide a default boot strategy
* Boot Sequencer boot strategy should be extensible so that users can develop their own boot sequencing

## Collaborators

* The Boot Sequencer is invoked by the [Virtual Processor](virtual_processor.md) after the processor's initial setup requirements are completed
* The Boot Sequencer invokes [C/C++ Runtime Startup](c_cpp_runtime.md)
* The Boot Sequencer calls functions in the [Virtual Platform](virtual_platform.md) throughout the boot process.
	* The Boot Sequencer calls Virtual Platform functions rather than working with the processor, RTOS, or hardware layer directly
	* This strategy promotes decoupling
* The Boot Sequencer finishes its process by invoking `main()`, which gives control to the [Client Application](../client_application.md)

## Sequence Diagram

<center>

| ![Boot Sequence Diagram](boot_sequence_diagram.png) |
|:--:|
| *Figure 1. Boot Sequence Diagram for Embedded Framework.* |

</center>

## Rationale

Systems generally require a fixed pattern for booting the application. C and C++ applications require memory relocations, zero-initialization, and invoking static constructors prior to entering the `main()` function. Additionally, the platform should be properly initialized before `main()`, allowing users to implement software logic with the system in a usable state.

The Boot Sequencer should utilize the [Strategy pattern](../../../patterns/strategy.md) to allow users to redefine the boot process. This is achieved with a templated parameter in `embvm::BootSequencer`.

The processor invokes the Boot Sequencer because the processor is the first item to gain control after a reset or power-on condition.

## Source Links

* [Boot Sequencer](../../../../src/core/boot/)
    * [boot_sequencer.hpp](../../../../src/core/boot/boot_sequencer.hpp)
    * [boot_sequencer_tests.cpp](../../../../src/core/boot/boot_sequencer_tests.cpp)
* [Simulator Platform boot.cpp file](../../../src/platforms/simulator/boot.cpp)

## Notes

Each platform's specific boot strategy should be defined in `boot.cpp`.
