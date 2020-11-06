# Embedded Virtual Machine Architecture & Design Documentation

All of the Embedded VM architecture and design documentation is tracked within the `embvm-core` repository. Our goal is to provide developers with the background information and design rationale necessary to understand how and why each Embedded VM component was developed.

## Architecture Documentation Overview

* [System Overview](system_overview.md)
    - Provides a high-level overview of the system and its goals.
* [Architectural Views](views/)
    - The `views/` directory contains architectural views, which look at the architectural design of the Embedded VM from different perspectives.
    * [Conceptual Architecture View](views/conceptual_architecture/conceptual_view.md) showcases significant components and abstractions that make up the Embedded VM framework.
    * [Layer View](views/layer/layer_view.md) describes how the Embedded VM framework is organized to support portability, particularly by restricting access to low-level details as you move up the stack.
    * [System Structural View](views/system_structural/system_structural_view.md) shows how the Embedded VM in context with external systems and components.
* [Architecture Decision Records](decisions/) (ADRs)
    - Major architectural decisions, especially decisions that have arisen in the course of developing the Embedded VM, are found in the `decisions/` folder. This folder is mapped to the [`adr-tools`](https://embeddedartistry.com/blog/2018/04/05/documenting-architectural-decisions-within-our-repositories/) software, which we use to document, track, and relate ADRs.
* [Domain Context Guide](domain_context.md)
    - Describes the major concepts considered in the Embedded VM, as well as the relationships between concepts.
* [Component Specifications](components/)
    - Major framework component specifications are kept in the `components/` directory. These specifications describe the roles, responsibilities, and collaborators for each component.
* [Design Constraints](constraints/)
    - Significant constraints placed on the design of the Embedded VM core, such as [not requiring dynamic memory](constraints/0001-core_no_dynamic_memory.md), can be found in the `constraints/` directory.
* [Architecture Principles](principles/)
    - The `principles/` directory contains the primary principles that guide the design and development of the Embedded VM.
* [Qualities](qualities/)
    - The `qualities/` directory contains the primary design qualities we want to emphasize with the design and behavior of the Embedded VM.
* [Use Cases](use_cases/)
    - The `use_cases/` directory contains use cases that we considered while designing the Embedded VM.
- [Architectural Scratch Pad](scratchpad.md)
    + Contains architectural notes that have not found another home yet. Useful as an inbox that can be processed at a later date.
