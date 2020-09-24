# 5. Provide Non-Blocking Interfaces by Default

Date: 2018-07-06

## Status

Accepted

## Context

Blocking interfaces are those that only return when an operation has been completed.

Non-blocking interfaces initiate a request and return when the request is submitted. Completion is annouced via a callback or by checking the status through another interface.

Non-blocking interfaces provide a more flexible implementation route.

Blocking implementations are typically avoided on embedded systems. Typically, a thread should sleep while waiting for an action to complete, rather than hogging processor resources.

## Decision

Interfaces provided by the framework will be non-blocking. Users can write their own blocking wrappers if blocking code is needed.

## Consequences

* Enables high flexibility in implementations
* Prevents wasted cycles within an operation
* Users can still create blocking interfaces if they need them. But they *must* create blocking interfaces.
* We can potentially supply decorators to create blocking functions if this is a major pain point
