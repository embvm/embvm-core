# Use Case: Compile in Simulator Mode

* Name: Compile in Simulator Mode
* Number: 0010

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

Developers should be able to select a subset of software to run in simulator mode on their host machine

## Actors

* Developer

## Preconditions

TBD

## Post-conditions

* A binary is produced which will run the framework software on a host machine

## Assumptions

TBD

## Steps

TBD

## Variations

TBD

## Non-functionals

* [Extensibility](../qualities/0004-extensibility.md):
	* Developers should be able to add or remove software components that are executed on the host machine

## Business Rules

N/A

## Architectural Mechanisms

* [Build System](../components/build_system.md)
* [Simulator Processor](../components/core/simulator_processor.md)

## Issues

TBD

## Related Use Cases

TBD
