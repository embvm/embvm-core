# Use Case: Add a New Driver

* Name: Add a New Driver
* Number: 0012

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

* List of actors involved in use case

## Preconditions

* Conditions that must be true when the use case is initiated

## Post-conditions

* State of the system when the use case terminates

## Assumptions

* Optional entry
* Use if the use case depends on any other assumptions (than pre/post conditions)
* E.g.: assumptions about resources, about the environment

## Steps

* Interactions between actors and system that are necessary to achieve goal
* This section represents the "happy case" i.e., default actions
* A step completes when all its component interactions have completed.
* Steps are in the form:
	* <sequence number> <interaction>
* If there are multiple steps, then each step must have an integer sequence number showing its position in the list of steps.
* Steps are initiated in order, in accordance with their sequence number.
* The default assumption is that each step is completed before the next is started
* Steps may be represented visually in UML activity or sequence diagrams

1. interaction a
2. interaction b

## Variations

* Any variations in the steps of a use case
* Capture all alternative paths here
* Capture all exceptions here (e.g., error cases)
* The alternative paths make use of the numbering of the primary steps to show at which point they differ from the basic scenario, and if appropriate, where they rejoin. Don't repeat information unnecessarily.

1. The system recognizes cookie on user's machine.
2. Go to step 4 (main path)

Or:

1. The system does not recognize the user's login information
2. Go to step 1 (main path)

## Non-functionals

* List any non-functional requirements that the use case must meet
* Link to relevant qualities, constraints, and principles

## Business Rules

* Optional entry
* If the business rule applies to multiple use cases, you can add a use case name field to the business rules catalogue

## Architectural Mechanisms

* What architectural mechanisms are associated with the use case?

## Issues

* List of issues that remain to be solved

## Related Use Cases

* Link to any related use cases
