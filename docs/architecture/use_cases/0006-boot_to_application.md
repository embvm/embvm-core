# Use Case: Boot to Application

* Name: Boot to Application
* Number: 0006

**Table of Contents:**
1. [Description](#description)
2. [Actors](#actors)
3. [Preconditions](#preconditions)
4. [Postconditions](#postconditions)
5. [Assumptions](#assumptions)
6. [Steps](#steps)
7. [Variations](#variations)
8. [Non-functionals](#non-functionals)
9. [Business Rules](#business-rules)
10. [Architectural Mechanisms](#architectural-mechanisms)
11. [Issues](#issues)
12. [Related Use Cases](#related-use-cases)

## Description

When the system is turned on or reset, the framework will set up the processor, board, (optional) RTOS, and call `main()`.

## Actors

* Developer
* User

In general, this use case is automatically executed when a system is turned on or reset.

## Preconditions

* Power is supplied to the system
* The processor code implements the initial boot sequencing and setup requirements
* The system receives control at the processor reset vector (power cycle, reset)

## Post-conditions

* The `main()` function is called

## Assumptions

* Optional entry
* Use if the use case depends on any other assumptions (than pre/post conditions)
* E.g.: assumptions about resources, about the environment

## Steps

1. User applies power to the system (or resets the system)
2. Processor code executes initial boot-up requirements
3. Processor transfers control to [Boot Sequencer](../components/core/boot_sequencer.md)
4. Boot Sequencer invokes (optional) platform early-stage setup
5. Boot Sequencer performs binary setup,
	* relocate
	* zero-initialize BSS section
6. Boot Sequencer executes optional RTOS initialization
7. Boot Sequencer calls static constructors
8. Boot Sequencer invokes platform initialization
9. Boot Sequencer invokes `main()`

## Variations

N/A

## Non-functionals

* [Portability](../qualities/0008-portability.md):
	* Underlying hardware dependencies should be enapsulated; the boot sequence should not be platform dependent
* [Reusability](../qualities/0011-reusability.md)
	* The boot process must be generic and work for all platforms
* [Extensibility](../qualities/0004-extensibility.md):
	* Developers should be able to extend or reconfigure the boot process to suit their needs

## Business Rules

N/A

## Architectural Mechanisms

* [Boot Sequencer](../components/core/boot_sequencer.md)

## Issues

N/A

## Related Use Cases

* [Developer User Stories](0001-user_stories_developer.md)
