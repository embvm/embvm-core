# Pattern: Event Sourcing

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


Event Sourcing

Let’s assume an Entity in its initial state. Being an Entity, it has its own identity, it’s a specific thing in the real world, which the application is modelling. Along its lifetime, the Entity data changes and, traditionally, the current state of the entity is simply stored as a row in a DB.
Transaction log

This is fine for most cases, but what happens if we need to know how the Entity reached that state (i.e., we want to know the credits and debits of our bank account)? It’s not possible because we only store the current state!
Using event sourcing, instead of storing the Entity state, we focus on storing the Entity state changes and computing the Entity state from those changes. Each state change is an event, stored in an event stream (i.e., a table in an RDBMS). When we need the current state of an Entity, we calculate it from all of its events in the event stream.
The event store becomes the principal source of truth, and the system state is purely derived from it. For programmers, the best example of this is a version-control system. The log of all the commits is the event store and the working copy of the source tree is the system state.
Greg Young 2010, CQRS Documents
Deletions

If we have a state change (event) that is a mistake, we can not simply delete that event because that would change the state change history, and it would go against the whole idea of doing event sourcing. Instead, we create an event, in the event stream, that reverses the event that we would like to delete. This process is called a Reversal Transaction, and not only brings the Entity back to the desired state but also leaves a trail that shows that the object had been in that state at a given point in time.
There are also architectural benefits to not deleting data. The storage system becomes an additive only architecture, it is well known that append-only architectures distribute more easily than updating architectures because there are far fewer locks to deal with.
Greg Young 2010, CQRS Documents
Snapshots

However, when we have many events in an event stream, computing an Entity state will be costly, it will not be performant. To solve this, every X amount of events we will create a snapshot of the Entity state at that point in time. This way, when we need the entity state, we only need to calculate it up to the last snapshot. Hell, we can even keep a permanently updated snapshot of the Entity, that way we have the best of both worlds.

Projections

In event sourcing, we also have the concept of a projection, which is the computation of the events in an event stream, from and to specific moments. This means that a snapshot, or the current state of an entity, fits the definition of a projection. But the most valuable idea in the concept of projections, is that we can analyse the “behaviour” of Entities during specific periods of time, which allows us to make educated guesses about the future (i.e., if in the past 5 years an Entity had increased activity during August, it’s likely that next August the same will happen), and this can be an extremely valuable capability for the business.
Pros and cons

Event sourcing can be useful for both the business and the development process:
* we query these events, useful both for business and for development to understand the users and system behaviour (debugging);
* we can also use the event log to reconstruct past states, again useful both for business and for development;
* automatically adjust the state to cope with retroactive changes, great for business;
* explore alternative histories by injecting hypothetical events when replaying, awesome for business.
But not everything is good news, be aware of hidden problems:
*
External updates

When our events trigger updates in external systems, we do not want to retrigger those events when we are replaying the events in order to create a projection. At this point, we can simply disable the external updates when we are in “replay mode”, maybe encapsulating that logic in a gateway.
Another solution, depending on the actual problem, might be to buffer the updates to external systems, performing them after a certain amount of time, when it is safe to assume that the events will not be replayed.
*
External Queries

When our events use a query to an external system, i.e., getting stock bonds ratings, what happens when we are replaying the events in order to create a projection? We might want to get the same ratings that were used when the events were run for the first time, maybe years ago. So either the remote application can give us those values or we need to store them in our system so we can simulate the remote query, again, by encapsulating that logic in the gateway.
*
Code Changes

Martin Fowler identifies 3 types of code changes: new features, bug fixes, and temporal logic. The real problem comes when replaying events which should be played with different business logic rules at different moments in time, i.e., last year tax calculations are different than this year. As usual, conditional logic can be used but it will become messy, so the advice it to use a strategy pattern instead.


So, I advise caution, and I follow these rules whenever possible:
* Keep events dumb, knowing only about the state change and not how it was decided. That way we can safely replay any event and expect the result to be the same even if the business rules have changed in the meantime (although we will need to keep the legacy business rules so we can apply them when replaying past events);
* Interactions with external systems should not depend on these events, this way we can safely replay events without the danger of retriggering external logic and we don’t need to ensure the reply from the external system is the same as when the event was played originally.
And, of course, like any other pattern, we don’t need to use it everywhere, we should use it where it makes sense, where it brings us an advantage and solves more problems than it creates.
