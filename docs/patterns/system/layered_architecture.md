# Pattern: Layered Architecture

* **Classification:** System Structuring Pattern

A layered architecture helps structure applications that can be decomposed into groups of subtasks where each group of subtasks is on a particular level of abstraction.

## Diagrams

TBD

## Context

* Dependency management: The key to Layered Architecture is dependency management.
	* Components of one layer can only interact with peers in the same level or components from lower levels.
	* This helps reduce dependencies between components on different levels.
* Two general applications of layering: strictly layered and relaxed layered
* In a layered architecture, layers form the first cut at partitioning the system into large garnularity substructures
* Layering involves an ordered structuring of computational services into levels

## Problem

* Designing a system whose dominating characteristic is a mix of low-level issues (like sensor input) and high-level issues (like user visible functionality)
* Requirements include:
	* Isolation for later source code changes
	* Parts of the system should be exchangeable (e.g., for portability) or reusable
	* Foster understanding and maintainability by grouping similar responsibilities
	* Work must be distributed across a team of develoeprs along clear boundaries

## Solution

* Structure the system into a number of layers, where all components in a layer are at the same level of abstraction

## Consequences

TBD

## Known Uses

TBD

## Variants

1. **Strongest form:** a given layer provides services only to the layers immediately above it, and utilizes only the services provided by layers immediately below it (e.g.,. each layer acts as a server to the layer above and client to the layer below).
2. Other, **weaker**, forms are possible in which some of these restructions on service provision are relaxed. Often, the lowest software layer in an application communicates more or less directly with file systems or other data storage facilities, while the highest software layer communicates fairly directly with human users through a windowing system
3. Layering can occur along several different dimensions; when it does, two layers in different dimensions will be such that neither is higher (or lower) than the other.
4. Individual layers are subdivided into components or subsystems, and may themselves be internally layered.

## Related Patterns

N/A

## References

* Buschmann, et al. 1996
* Shaw and Garlan, 1996
* Bass, et al., 1998

## Notes

N/A
