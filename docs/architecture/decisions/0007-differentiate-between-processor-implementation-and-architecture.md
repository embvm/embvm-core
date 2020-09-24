# 7. Differentiate Between Processor Implementation and Architecture

Date: 2018-07-06

## Status

Accepted

## Context

We want to support a wide variety of chips. However, there are many chips that utilize the same architecture, and we dont' want to duplicate information across those chips.

We can abstract out architecture-related behavior from the vendor-specific processor implementations.

## Decision

A processor will be represented by two orthogonal abstract models:

1. Processor architecture (e.g., ARM Cortex-M0, x86_64, PIC32)
2. Processor implementation (e.g., a Nordic nRF52 has a unique architecture & set of peripherals)

A specific processor will be repersented by inheriting from an architecture and a processor family. The peripherals, definitions, and other details for that processor which are unique will be contained within the specific processor implementation.

This means that an nRF52822 inherits from nRF52 and Cortex-M4F.

The processor architecture tree with contain all architecture-specific behavior (e.g., how to turn on/off MMU on Cortex-M0, M4, A5, etc.).

The processor implementation tree will contain all vendor-specific behaviors and constructs.

## Consequences

* We need to carefully abstract out the architecture-related behavior and encapsulate that.
	* What is common for ARM?
	* What is common for PIC?
	* What is common for AVR?
	* What is common for x86?
* We need to make sure that there is no overlap between the functionality in the arch tree and the processor tree
	* In order to keep our multiple inheritence model straightforward, the functionality must be kept orthogonal
* Developers need to have clear indication of this model when created a new chip implementation
