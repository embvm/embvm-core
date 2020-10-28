# 22. Unified GPIO base class

Date: 2020-10-28

## Status

Accepted

## Context

The issue motivating this decision, and any context that influences or constrains the decision.

## Decision

To simplify the implementation for drivers, we will create a "unified" GPIO base class, which provides the standard interfaces that can be used to set the mode (input, output, special function). This will provide a single class to implement for GPIO support, rather than one class for each mode.

Ideally, we will find a way to zero-overhead create wrapper classes that constrain the interface appropriately, so we can mark pins as `GPIOInput` or `GPIOOutput` and have the compiler report an error if we try to do an invalid operation for the given type.

## Consequences

- Simplified implementation for porting purposes - we only need to implement one class.
- We lose type safety with the default base class; there's no way to generate a compiler error if `set()` is being called on an input GPIO pin.
