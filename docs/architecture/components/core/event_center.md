# CRC-R: Event Center

Provides a centralized location for subscribing to events and posting events.

**Status:** Completed, issues exist for dynamic memory allocations

**Table of Contents:**

1. [Responsibilities](#responsibilities)
2. [Requirements](#requirements)
3. [Collaborators](#collaborators)
4. [Rationale](#rationale)
5. [Source Links](#source-links)
6. [Related Documents](#related-documents)
7. [Notes](#notes)

## Responsibilities

* Provides modules with a method for registering/unregistering for system-level event notifications
* Calls subscribers when system-level events occur
* Provides interfaces for triggering system notifications

## Requirements

* Provides methods for utilizing static memory rather than dynamic memory
* Provide method for notifying with data
* Provide method for notifying without data

## Collaborators

* Notifications are [Dispatched](../utilities/dispatch_queue.md)
* [Events](../utilities/event.md) are used to interact with the dispatcher

## Rationale

Objects which are event-based can use the observer pattern, allowing consumers to register callbacks for specific events. However, for centralized events, there must be one source of truth where consumers can subscribe and post notifications.

Some events will be signals: they just notify that an event occurred, and no additional data is provided. Others will carry data. Both should be usable with the `embvm::EventCenter`.

## Source Links

* [event_center.hpp](../../../../src/core/platform/event_center.hpp)
* [Unit Tests](../../../../src/core/platform/event_tests.cpp)

## Related Documents

N/A

## Notes

Notes on decoupling with dispatcher:

> Ideally, both the dispatcher and the event should live in neither component:
> * The dispatcher should be a library completely independent of our application and therefore installed in a generic location using a dependency management system
> * The event, nevertheless, is part of our application but should live outside both components as to keep them unaware of each other. The event is shared between the components and it is part of the core of the application. Events are part of what DDD calls the Shared Kernel. This way, both components will depend on the Shared Kernel but will remain unaware of each other.

Note on Listeners Vs Subscribers:

> A common debate to have when implementing an event driven architecture is whether to use event listeners or event subscribers, so let’s clarify my take on that:
> 1. Event Listeners react to only one event and can have multiple methods reacting to it. So we should name the listener following the event name, for example if we have a “UserRegisteredEvent” we will have a “UserRegisteredEventListener” this will make it easy to know, without even looking inside the file, to what event that listener is listening to. The methods (reactions) to the event should reflect what the method actually does, for example “notifyNewUserAboutHisAccount()” and “notifyAdminThatNewUserHasRegistered()“. This should be the usual approach for most cases, as it keeps the listener small and focused on the single responsibility of reacting to a specific event. Furthermore, if we have a componentized architecture, each component (if needed) would have its own listener to an event that could be triggered from multiple locations.
> 2. Event Subscribers react to multiple events and have multiple methods reacting to them. The naming of the subscriber is more difficult as it can not be ad-hoc, nevertheless the subscriber should still comply with the Single Responsibility Principle, so the name of the subscriber needs to reflect its single intent. Using event subscribers should be a less common approach, especially in components, as it can easily break the single responsibility principle. An example of a good use case for an event subscriber is to manage transactions, more concretely we could have an event subscriber named “RequestTransactionSubscriber” reacting to events like “RequestReceivedEvent“, “ResponseSentEvent” and “KernelExceptionEvent“, and bind to them the start, commit and rollback of transactions, respectively, each in their own method like “startTransaction()“, “finishTransaction()” and “rollbackTransaction()“. This would be a subscriber reacting to multiple events but still focusing on the single responsibility of managing the request transaction.
