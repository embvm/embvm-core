# Pattern: Strategy

* Encapsultes an algorithm inside of a class
* Define a family of algorithms, encapsulate each one, and make them interchangeable. Strategy lets the algorithm vary independently from the clients that use it.
* Capture the abstraction in an interface, bury implementation details in derived classes.


Talk about achieving the strategy pattern using CRTP

## Diagrams

* Include / link to visual representations of this pattern

## Context

* How does this pattern help to complete larger patterns?
* What is the intent of this pattern?

## Problem

```
//TODO: BREAK THIS SECTION UP INTO PROBLEM, SOLUTION, CONSEQUENCES
```

One of the dominant strategies of object-oriented design is the "open-closed principle".
Figure demonstrates how this is routinely achieved - encapsulate interface details in a base class, and bury implementation details in derived classes. Clients can then couple themselves to an interface, and not have to experience the upheaval associated with change: no impact when the number of derived classes changes, and no impact when the implementation of a derived class changes.

A generic value of the software community for years has been, "maximize cohesion and minimize coupling". The object-oriented design approach shown in figure is all about minimizing coupling. Since the client is coupled only to an abstraction (i.e., a useful fiction), and not a particular realization of that abstraction, the client could be said to be practicing "abstract coupling" . an object-oriented variant of the more generic exhortation "minimize coupling".
A more popular characterization of this "abstract coupling" principle is "Program to an interface, not an implementation".
Clients should prefer the "additional level of indirection" that an interface (or an abstract base class) affords. The interface captures the abstraction (i.e., the "useful fiction") the client wants to exercise, and the implementations of that interface are effectively hidden.

Structure

The Interface entity could represent either an abstract base class, or the method signature expectations by the client. In the former case, the inheritance hierarchy represents dynamic polymorphism. In the latter case, the Interface entity represents template code in the client and the inheritance hierarchy represents static polymorphism.

## Solution

1. Identify an algorithm (i.e., a behavior) that the client would prefer to access through a "flex point".
2. Specify the signature for that algorithm in an interface.
3. Bury the alternative implementation details in derived classes.
4. Clients of the algorithm couple themselves to the interface.

## Consequences

* What are the benefits of using this pattern?
* What are the liabilities with using this pattern?

## Known Uses

* Describe known applications for this pattern
* Link to uses of the pattern within the source code

## Variants

Link to any patterns that are varients of this

## Related Patterns

* Strategy is like Template Method except in its granularity.
* State is like Strategy except in its intent.
* Strategy lets you change the guts of an object. Decorator lets you change the skin.
* State, Strategy, Bridge (and to some degree Adapter) have similar solution structures. They all share elements of the 'handle/body' idiom. They differ in intent - that is, they solve different problems.
* Strategy has 2 different implementations, the first is similar to State. The difference is in binding times (Strategy is a bind-once pattern, whereas State is more dynamic).
* Strategy objects often make good Flyweights.

# References

* [Sourcemaking: Strategy Design Pattern](https://sourcemaking.com/design_patterns/strategy)
