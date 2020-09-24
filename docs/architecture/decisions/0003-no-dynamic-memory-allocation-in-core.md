# 3. No Dynamic Memory Allocation in Core

Date: 2018-07-06

## Status

Accepted

## Context

Many systems, teams, and designs require that the system will not utilize dynamic memory allocation. We should maintain a flexible design by allowing users to use dynamic memory allocation if they desire. However, we should be able to support the strictest operating model for maximum flexibility and potential use of the framework.

## Decision

No dynamic memory allocation will be utilized by framework core components

## Consequences

* Framework remains open for use by teams who will not allow dynamic memory allocation
* Constructs which are provided by C/C++ but require dynamic memory allocation will need to be re-written with a static option (e.g., static_function, static_queue)
