# 12. Switch to Catch for Unit Testing

Date: 2018-09-08

## Status

Proposed

## Context

We started unit testing with doctest. However, when trying to compile with exceptions disabled, it seems to be a problem.

Catch provides support for compiling with -fno-exceptions, and even lets us configure the termination handler.

## Decision

We're proposing to use Catch for unit testing. But we need to actually try it out firs.t

## Consequences

Tests cannot be written in headers when using Catch. This is minor - we'll keep tests in a separate file anyway.
