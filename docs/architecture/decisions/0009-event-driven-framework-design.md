# 9. Event Driven Framework Design

Date: 2018-07-06

## Status

Accepted

## Context

* Embedded systems are highly event-driven, as they are responding to external stimuli and reacting in a planned way
* Event-driven APIs reduce coupling, as the various objects don't need to know anything about other objects that they work with
* We can reduce the number of threads used by relying on event-driven behavior

## Decision

The framework will be defined with interfaces and processing models that support event-driven development (callbacks, events, register/unregister for events).

Dispatch queues will be provided to assist with the event driven model.

Platform examples will default to dispatch-based processing models.

## Consequences

* All of our APIs should support an event driven interface
* Consideration of callback functions & notification registering needs to be included at all stages
* Threading will still be allowed and usable, maintaining flexibility
* Dispatch queues will be used to handle generic callbacks without blocking high-priority operations

## Further Reading

* [Observer Pattern](../../patterns/observer.md)
