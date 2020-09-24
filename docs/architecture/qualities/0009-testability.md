# Quality Requirement: Testability

* Name: Testability
* Reference Number: 0009
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

Testability is the degree to which a software artifact (i.e., a software system, software module, requirements- or design document) supports testing in a given test context. If the testability of the software artifact is high, then finding faults in the system (if it has any) by means of testing is easier.

## Scenario

* A developer checks a module's implementation and finds that there are unit tests which support the implementation
* A developer runs framework unit tests and receives the results of those tests

## Test Cases

TBD

### Landing Zones

| Quality | Measurement | Minimum | Target | Oustanding |
|---------|-------------|---------|--------|------------|
| Modules with Unit Tests | % | 50 | 75 | 95 |
| Performance | avg txn time | 2s | 1s | < 1s |
| Data Quality | accuracy | 97% | 99% | > 99% |

## Context

Testing improves our confidence in the framework and its interfaces. Testing reduces the support burden of Embedded Artistry and helps ensure that our releases are the highest quality.

## Tradeoff Notes

Testability requires development time and effort, though this effort pays dividends in release quality.

## Related Qualities

Testability improves the [Resilience](0003-resilience.md), [Reliability](0002-reliability.md), [Stability](0005-stability.md), and [Robustness](0006-robustness.md) of the framework

## Notes

Further Reading:

* [Wikipedia: Software Testability](https://en.wikipedia.org/wiki/Software_testability)

Testability is not an intrinsic property of a software artifact and can not be measured directly (such as software size). Instead testability is an extrinsic property which results from interdependency of the software to be tested and the test goals, test methods used, and test resources (i.e.,, the test context).
