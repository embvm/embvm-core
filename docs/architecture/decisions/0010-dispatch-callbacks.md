# 10. Dispatch Callbacks

Date: 2018-07-06

This relates to decision [0009: Event Driven Framework Design](0009-event-driven-framework-design.md)

## Status

Proposed

## Context

Because we are building an [event-driven framework], we need to think about how to handle callback functions. We want to use them without causing threads/functions to block unexpectedly while executing callbacks.

## Decision

We will dispatch callbacks to a global dispatch queue which will execute them as processing time is available.

## Consequences

* A system will always need a dispatch queue to handle callbacks
* Callbacks will not cause threads/functions to block unexpectedly while executing the callbacks
* High-priority threads can still execute ahead of the callbacks, ensuring that we meet our real-time guarantees
* We need to make sure the dispatched functions have operating time

## Issues

* A system will always need a dispatch queue to handle callbacks
	* TBD: How can we enforce this?

## Further Reading

* [Decision 0009: Event Driven Framework Design](0009-event-driven-framework-design.md)
