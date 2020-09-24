# 15. Use embvm top level namespace for core interfaces

Date: 2019-02-20

## Status

Proposed

## Context

All of the framework code was placed into the global namespace. We need to find a home to group our code in.

## Decision

The `embvm` (Embedded Virtual Machine) namespace will be used for framework constructs.

## Consequences

Framework core will need to be updated so they are grouped in the `embvm` top-level namespace. This includes everything in the `src/core/` directory.

There are likely second-level namespaces used, so developers will be burdened with remembering these details.


