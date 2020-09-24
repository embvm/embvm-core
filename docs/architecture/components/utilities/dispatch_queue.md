# CRC-R: Dispatch Queue

Fire off asynchronous processing requests to a group of threads who will handle the work. Provides an alternative to explicit thread management.

**Status:** Completed

**Table of Contents:**

1. [Responsibilities](#responsibilities)
2. [Requirements](#requirements)
3. [Collaborators](#collaborators)
4. [Rationale](#rationale)
5. [Source Links](#source-links)
6. [Related Documents](#related-documents)
7. [Notes](#notes)

## Responsibilities

* Maintain a queue of callable objects to execute on a FIFO basis
* Manage a variable-sized pool of threads which pull items from the work queue and execute them
* Sleep threads when not working

## Requirements

* Provide a static dispatch queue implementation (fixed memory size)
* Provide a dynamic dispatch queue implmenetation (dynamic growth)
* Provide an asynchronous dispatch queue (call dispatched, caller maintains control)
* Provide an option to utilize a priority queue (ensure next-highest-priority is always run)

## Collaborators

* May utilize a [Static Queue](static_queue.md)
* May utilize a [Static Function](static_function.md)

## Rationale

We want to design an event-driven framework. The dispatch queue is a core concept for that framework. The queue allows users to dispatch callable functions to a generic queue of worker threads. There is no longer a need to manually create and manage threads.

Threads are a problematic programming area, fraught with bugs. Dispatch queues reduce the number of threads needed in a system.

Single-thread queues can be used to protect peripheral access, which means mutexes are not necessary.

## Source Links

* [dispatch Folder](../../../../src/utilities/dispatch/)
* Standard Dispatch Queue - uses dynamic memory
	* [dispatch.hpp](../../../../src/utilities/dispatch/dispatch.hpp)
	* [dispatch_tests.cpp](../../../../src/utilities/dispatch/dispatch_test.cpp)
* Static Dispatch Queue - sizes decided on at compile time
	* [static_dispatch.hpp](../../../../src/utilities/dispatch/static_dispatch.hpp)
	* [static_dispatch_tests.cpp](../../../../src/utilities/dispatch/static_dispatch_test.cpp)

## Related Documents

* An [Interrupt Queue](interrupt_queue.md) represents a similar concept

## Notes

* Do we need to provide a synchronous dispatch queue variant? (Blocks until call completes, sleeps calling thread until call completes?)
* Time-limit context switching (e.g., 30ms time slices), or run-to-completion?
	* Samek's framework uses Run-to-completion
	* Should we have two modes?
* We can create dispatch queues with differnet priorities - high/med/low priority queue
	* This is how you can control different priority levels
	* E.g. 5 threads, 3 different priority levels.
	* We can have 1-2 different generic threads at different priority levels we want: interrupt, high, medium, low
* We can utilize dispatch queues for event-driven functions
	* event-driven functions that just dispatch the next state onto an async dispatch queue, and those ran whenever the processor is next available
	* Your state machine traversed to the next fucntion called for that state. Then we dispatch the next state function
	* Taking that idea further - say I have a data acquisition thread and I need to kick off a read of the next sample
		* Start an i2c read on the accelerometer
		* then you have a callback function which is "interpret_sample()", called on completion
		* Then when that's done I can sleep or dispatch another instruction.
	* **Downside:** Cannot control priority of next executed object
		* But then I could use a priority queue?
	* You could even just have an opt-in system. If there's a callback, you can register optionally.
		* You could still support the normal error flow - wait for an event, function tos ay it's done, etc.
		* If you don't supply the callback, it just returns and normal operation is possible.

From Apple:

> … Dispatch queues are an easy way to perform tasks asynchronously and concurrently in your application. A task is simply some work that your application needs to perform. … A dispatch queue is an object-like structure that manages the tasks you submit to it. All dispatch queues are first-in, first-out data structures. Thus, the tasks you add to a queue are always started in the same order that they were added. GCD provides some dispatch queues for you automatically, but others you can create for specific purposes. …

> Concurrent queues (also known as a type of global dispatch queue) execute one or more tasks concurrently, but tasks are still started in the order in which they were added to the queue. The currently executing tasks run on distinct threads that are managed by the dispatch queue. The exact number of tasks executing at any given point is variable and depends on system conditions. …

Should we utilize the main queue concept?

> The main dispatch queue is a globally available serial queue that executes tasks on the application’s main thread. This queue works with the application’s run loop (if one is present) to interleave the execution of queued tasks with the execution of other event sources attached to the run loop. Because it runs on your application’s main thread, the main queue is often used as a key synchronization point for an application. …
