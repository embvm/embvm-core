# Use Case: Find All Drivers of a Specific Type

* Name: Find All Drivers of a Specific Type
* Number: 0009

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

* Developers should be able to access a list which contains all generic interfaces of a specific driver category
	* e.g., "accel" -> { "accel0", "accel1" }

## Actors

* Developer

## Preconditions

* Requested driver category is defined within the framework

## Post-conditions

* A list of all drivers matching that category is returned

## Assumptions

N/A

## Steps

1. User requests a list of drivers matching a specific category
2. DriverRegistry searches registered driver objects matching the category
3. DriverRegistry returns a list of generic driver interfaces matching the category

## Variations

2. Driver category not found
	* Return an empty list in Step 3

## Non-functionals

* [Portability](../qualities/0008-portability.md):
	* The framework should return a generic interface regardless of the specific underlying hardware

## Business Rules

N/A

## Architectural Mechanisms

* [Driver Registry](../components/core/driver_registry.md)

## Issues

N/A

## Related Use Cases

* [Developer User Stories](0001-user_stories_developer.md)
* [0008: Access a Specific Driver Instance](0008_access_a_specific_driver_instance.md)
