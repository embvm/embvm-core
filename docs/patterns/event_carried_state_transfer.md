# Pattern: Event-Carried State Transfer

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

* [Event Notification](event_notification.md)
* [Event Sourcing](event_sourcing.md)

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


Event-Carried State Transfer

Let’s consider again the previous example of an application core with clearly defined components. This time, for some of their functionality, they need data from other components. The most natural way of getting that data is to ask the other components for it, but that means the querying component will know about the queried component: the components will be coupled to each other!
Another way of sharing this data is by using events that are triggered when the component that owns the data, changes it. The event will carry the whole new version of the data with them. The Components interested in that data will be listening to those events and will react to them by storing a local copy of that data. This way, when they need that external data, they will have it locally, they will not need to query the other component for it.
*
Advantages

    * Greater resilience, since the querying components can function if the queried component becomes unavailable (either because there is a bug or the remote server is unreachable);
    * Reduced latency, as there’s no remote call (when the queried component is remote) required to access the data;
    * We don’t have to worry about load on the queried component to satisfy queries from all the querying components (especially if it’s a remote component);
*
Disadvantages

    * There will be several copies of the same data, although they will be read-only copies and data storage is not a problem nowadays;
    * Higher complexity of the querying component, as it will need the logic to maintain a local copy of the external data although this is pretty standard logic.
Maybe this pattern is not necessary if both components execute in the same process, which makes for fast communication between components, but even then it might be interesting to use it for decoupling and maintainability sake or as a preparation for decoupling those components into different microservices, sometime in the future. It all depends on what are our current needs, future needs and how far we want/need to go with decoupling.
