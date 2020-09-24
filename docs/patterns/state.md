# Pattern: State

Template for documenting architectural patterns used in this repository. Replace "Template" in the title with the pattern name

* Include a summary description of this pattern
* Include a brief classification of this pattern (Structural, Decoupling, etc.)

## Diagrams

* Include / link to visual representations of this pattern

## Context

* How does this pattern help to complete larger patterns?
* What is the intent of this pattern?

## Problem

* Two sentence description
* Include example/motivation
* Include the forces at play (applicability)
* Follow with empirical background for the pattern
* Evidence for validity
* Range of valididty

## Solution

* Stated as an instruction
* Describe the structure, dynamics, and implementation heuristics

## Consequences

* What are the benefits of using this pattern?
* What are the liabilities with using this pattern?

## Known Uses

* Describe known applications for this pattern
* Link to uses of the pattern within the source code

## Variants

Link to any patterns that are varients of this

## Related Patterns

Link to any related patterns


## notes

* State pattern for dirver/power state management?
    * network connections tate
    * definitely seems like an applicable pattern. then identify the applicable problem.
    * Reduce conditionals by putting each conditional branch into a separate class
    * New states are easy: new class!
    * but you want to avoid object explosion
