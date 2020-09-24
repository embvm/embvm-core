# Use Case: Change System Power State

* Name: Change System Power State
* Number: 0007

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

* Goal to be achieved by use case and sources for requirement

## Actors

* Developer

## Preconditions

* Current power state is valid
* Drivers have been paired with power management states

## Post-conditions

* Current system power state matches the requested state
* Driver on/off states match the requirements for the current system power state

## Assumptions

* User has created a [System Power Manager](../components/subsystems/system_power_manager.md) with a valid power management strategy
* Drivers controlled by the System Power Manager exist on the system and implement on/off functionality

## Steps

1. User requests a system power state change
2. System Power Manager identifies driver state changes that are necessary for the power state transition
3. System Power Manager calls `stop()` on each driver which must be disabled for the requested power state
4. System Power Manager calls `start()` on each driver which must be enabled for the requested power state
5. System Power Manager updates its current power state to match the requested state
6. System Power Manager notifies subscribers of a power state change and reports the current power state

## Variations

1. Invalid power state requested
	1. Do not execute steps 2-6
	2. Assert an error

## Non-functionals

* [Portability](../qualities/0008-portability.md):
	* Underlying hardware dependencies should be enapsulated
* [Reusability](../qualities/0011-reusability.md)
	* The power management process must be generic and work for all platforms
* [Extensibility](../qualities/0004-extensibility.md):
	* Developers should be able to extend or reconfigure the power management process to suit their needs

## Business Rules

N/A

## Architectural Mechanisms

* [System Power Manager](../components/subsystems/system_power_manager.md)

## Issues

N/A

## Related Use Cases

* [Developer User Stories](0001-user_stories_developer.md)
