# Quality Requirement: Reusability

* Name: Reusability
* Reference Number: 0011
* Parent: [Productivity](0001-productivity.md)
* Children: N/A
* Category: Evolution Qualities

**Table of Contents:**

1. [Description](#description)
2. [Scenario](#scenario)
3. [Test Cases](#test-cases)
4. [Landing Zones](#landing-zones)
5. [Context](#context)
6. [Tradeoff Notes](#tradeoff-notes)
7. [Related Qualities](#related-qualities)

## Description

Reusability is the use of existing assets within the software product development process. Reusable assets include code, software components, test suites, designs, and documentation.

Reusability implies some explicit management of build, packaging, distribution, installation, configuration, deployment, maintenance, and upgrade issues.

## Scenario

TBD

## Test Cases

* Subsystems run on two separate embedded platforms as well as the host simulator before being declared reusable
* Generic Drivers run on two separate embedded platforms as well as the host simulator before being declared reusable
* Utilities run on all platforms without modification

### Landing Zones

N/A

## Context

The framework represents a reusable software design for embedded systems applications. There are many common utilities and design structures which are regularly re-implemented (at varying levels of quality) across embedded projects. Additionally, drivers are a prime candidate for re-use, though in practice it rarely occurs due to tight coupling to underlying processor hardware.

## Tradeoff Notes

Implementing a potentially reusable module requires a little extra effort, but that effort is worthwhile if the module is reused.

If management of build/packaging/distribution/installation/configuration/deployment/maintenance issues are not addressed or considred, the design will not be reused in practice.

## Related Qualities

Reusable software improves [Productivity](0001-productivity.md). Productivity is the parent quality of.

Reusability is boosted by [Extensibility](0004-extensibility.md). Reusability together with extensibility allows a technology to be transferred to another project with less development and maintenance time, as well as enhanced reliability and consistency.

## Notes

Futher Reading:

* [Wikipedia: Stability Model](https://en.wikipedia.org/wiki/Stability_Model)
* [Wikipedia: Reusability](https://en.wikipedia.org/wiki/Reusability)

**A reusable module is a module that has been successfully reused.**

Ways to achieve reusability:

* A procedure becomes more reusable by adding parameters
* A class can be designed for reuse via inheritance
* A procedure or abstract data type can become more reusable by using a type qualifier (generalization)

Candidate design features for software reuse include:

* Adaptable
* Small Size
* Consistency
* Correctness
* Extensibility
* Fast
* Flexible
* Generic
* Localization of volatile (changeable) design assumptions (David Parnas)
* Modularity
* Orthogonality
* Parameterization
* Simple: low complexity
* Stability under changing requirements
