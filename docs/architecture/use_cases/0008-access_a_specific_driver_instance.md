# Use Case: Access a Specific Driver Instance

* Name: Access a Specific Driver Instance
* Number: 0008

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

* Developers should be able to access the generic interface of a specific driver instance
	* e.g.,, "accel0", "spi1"

## Actors

* Developer

## Preconditions

* Requested driver category is defined within the framework

## Post-conditions

N/A

## Assumptions

N/A

## Steps

1. User requests a specific driver instance
2. DriverRegistry searches registered driver objects for a matching ID
3. DriverRegistry returns a handle to the requested driver instance

## Variations

2. Driver ID not found
	* Return an invalid driver object in step 3 (indicating failure)

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
* [0009: Find All Drivers of a Specific Type](0009-find_all_drivers_of_a_specific_type.md)
