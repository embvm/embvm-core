# 11. Generic Startup Library

Date: 2018-07-06

## Status

Proposed

## Context

Systems go through a predictable set of boot behaviors, which we can commonize:

* Relocate
* zero data in the .bss section
* Call constructors
* Initialize RTOS
* Set default hardware pin states
* Create drivers
* Configure system

## Decision

We will provide a generic startup library which manages the initial boot sequence in a predictable manner.

The library will gain control from the processor startup sequence and transition the boot control process to the platform layer. It will call pre-defined platform APIs to initialize the RTOS, board, etc.

We will call the same order ever time, but allow the platform to define actions that occur at each stage.

## Consequences

* Boot process needs to be well-defined in order to apply across the board
* Proper extension points need to be added to maintain design flexibility
* Boot Sequencer must be made with a strategy pattern to allow users to override the behavior
