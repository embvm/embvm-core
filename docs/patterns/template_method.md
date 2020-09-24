# Pattern: Template Method

* **Classification:** TBD

Template Method (325) Define the skeleton of an algorithm in an operation, deferring some steps to subclasses. Template Method lets subclasses redefine certain steps of an algorithm without changing the algorithm’s structure.

Gamma, Erich. Design Patterns: Elements of Reusable Object-Oriented Software (Addison-Wesley Professional Computing Series) (Kindle Locations 470-472). Pearson Education. Kindle Edition.

Template = algorithm flexibility, enforcement of a functional pattern on subclasses

template = inheritence, function enforcement


Learn. This solves the subclass behavior problem i wrotw bout in the notebook. Use this.



5. Algorithmic dependencies. Algorithms are often extended, optimized, and replaced during development and reuse. Objects that depend on an algorithm will have to change when the algorithm changes. Therefore algorithms that are likely to change should be isolated. Design patterns: Builder (97), Iterator (257), Strategy (315), Template Method (325), Visitor (331).



Consequences Template methods are a fundamental technique for code reuse. They are particularly important in class libraries, because they are the means for factoring out common behavior in library classes. Template methods lead to an inverted control structure that’s sometimes referred to as “the Hollywood principle,” that is, “Don’t call us, we’ll call you” [Swe85]. This refers to how a parent class calls the operations of a subclass and not the other way around. Template methods call the following kinds of operations: • concrete operations (either on the ConcreteClass or on client classes); • concrete AbstractClass operations (i.e., operations that are generally useful to subclasses); • primitive operations (i.e., abstract operations); • factory methods (see Factory Method (107)); and • hook operations, which provide default behavior that subclasses can extend if necessary. A hook operation often does nothing by default. It’s important for template methods to specify which operations are hooks (may be overridden) and which are abstract operations (must be overridden). To reuse an abstract class effectively, subclass writers must understand which operations are designed for overriding.

Gamma, Erich. Design Patterns: Elements of Reusable Object-Oriented Software (Addison-Wesley Professional Computing Series) (Kindle Locations 5136-5148). Pearson Education. Kindle Edition.


Use as hooks for my driver design

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
