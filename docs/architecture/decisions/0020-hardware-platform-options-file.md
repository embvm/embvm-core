# 20. Hardware Platform Options File

Date: 2020-10-20

## Status

Accepted

Caused by [18. Driver Registration in HW Platform](0018-driver-registration-in-hw-platform.md)

## Context

The issue motivating this decision, and any context that influences or constrains the decision.

## Decision

We will create a new required `hw_platform_options.hpp` file. This file is defined in the platform level, and can be used to configure any necessary compile-time parameters without the use of templates.

## Consequences

- We cannot (easily) build the hardware platform into a library independently of the platform, so any includes that are used in the hardware platform can still be used at the platform level (but not the application level)
    + This may be solvable with some build magic.
