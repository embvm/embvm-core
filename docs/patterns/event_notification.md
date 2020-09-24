# Pattern: Event Notification

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

* [Event Sourcing](event_sourcing.md)
* [Event Carried State Transfer](event_carried_state_transfer.md)

## References

* [Event Driven Architecture](https://herbertograca.com/2017/10/05/event-driven-architecture/)

## Notes

Martin Fowler identifies three different types of event patterns:
* Event Notification
* Event-Carried State Transfer
* Event-Sourcing
All of these patterns share the same key concepts:
1. Events communicate that something has happened (they occur after something);
2. Events are broadcasted to any code that is listening (several code units can react to an event).

Event Notification

Let’s suppose we have an application core with clearly defined components. Ideally, those components are completely decoupled from each other but, some of their functionality requires some logic in other components to be executed.
This is the most typical case, which was described earlier: When component A performs the logic that needs to trigger the component B logic, instead of calling it directly, it triggers an event sending it to an event dispatcher. Component B will be listening to that specific event in the dispatcher and will act whenever the event occurs.
It is important to note that, a characteristic of this pattern is that the event carries minimal data. It carries only enough data for the listener to know what happened and carry out their code, usually just entity ID(s) and maybe the date and time that the event was created.
*
Advantages

    * Greater resilience, if the events are queued the origin component can perform its logic even if the secondary logic can not be performed at that moment because of a bug (since they are queued, they can be executed later, when the bug is fixed);
    * Reduced latency, if the event is queued the user does not need to wait for that logic to be executed;
    * Teams can evolve the components independently, making their work easier, faster, less prone to problems, and more organic;
*
Disadvantages

    * If used without criteria, it has the potential to transform the codebase in a pile of spaghetti code.
