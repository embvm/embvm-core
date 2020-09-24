# Quality Requirement: Maintainability

* Name: Maintainability
* Reference Number: 0013
* Parent: N/A
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

Maintainability is the ease with which the can be maintained in order to:

* Root cause & correct an error
* Replace a faulty implementation with an improved one (with no negative effects on those using the API)
* Prevent unexpected conditions from occurring
* Meet new requirements (e.g., through extension)

Maintainability also represents the ability of our system to continuously improve and simplify over time.

## Scenario

TBD

## Test Cases

* Fixing a bug with the framework should take less than 8 hours once the bug can be reproduced

TBD Test Cases:

* Set Line of Code delta targets for existing modules (we want to remove more than we add)
* Cyclomatic Complexity (what is our limit?)

### Landing Zones

| Quality | Measurement | Minimum | Target | Oustanding |
|---------|-------------|---------|--------|------------|
| Time to fix a bug | hours | 8 | 4 | 1 |

## Context

We are positioning our framework so that we can support a large number of clients, projects, and platforms. In order to effectively support a variety of products, we must be able to maintain and improve our framework over time.

## Tradeoff Notes

N/A

## Related Qualities

* Maintainability can be considered an aspect of internal framework developer [Productivity](0001-productivity.md)
* Maintainability helps our system stay [Resilient](0003-resilience.md)
* Maintainability is improved through [Serviceability](0012-serviceability.md) mechanisms

## Notes

Further Reading:

* [Wikipedia: Maintainability](https://en.wikipedia.org/wiki/Maintainability)
