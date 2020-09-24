# Quality Requirement: Stability

* Name: Stability
* Reference Number: 0005
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

In order for developers to productively work with the framework, the objects and interfaces should remain stable over time. New releases, features, and API changes should not impact other parts of the system.

> Stability characterizes the sensitivity to change of a given system that is the negative impact that may be caused by system changes.

## Scenario

TBD

## Test Cases

* Number of changes to an API after it is released

### Landing Zones

<center>

| Quality | Measurement | Minimum | Target | Oustanding |
|---------|-------------|---------|--------|------------|
| API changes after a release | # changes | 2 | 1 | 0 |
| API changes per year | # changes | 8 | 2 | 0 |

</center>

## Context

**The public interface of a black box is more important than its implementation.** In most cases, if its interface isn't changed, its implementation can be changed without affecting client code.

Because applications are so dependent on the framework for their design, they are particularly sensitive to changes in framework interfaces. As a framework evolves, applications have to evolve with it. Once clients start using an API, the costs and risks can be substantial, so it's best to avoid changes that affect client code.

## Tradeoff Notes

A focus on Stability can be at odds with making the framework [Resilient](0003-resilience.md).

To quote *Thinking in Systems: A Primer* by Donna Meadows (p. 77):

```
conversely, systems that are constant over time can be unresilient. This distinction between static stability and resilience is important. Static stability is something you can see; it’s measured by variation in the condition of a system week by week or year by year. Resilience is something that may be very hard to see, unless you exceed its limits, overwhelm and damage the balancing loops, and the system structure breaks down. Because resilience may not be obvious without a whole-system view, people often sacrifice resilience for stability, or for productivity, or for some other more immediately recognizable system property.
```

## Related Qualities

* [Productivity](0001-productivity.md) is the parent quality
* Stable APIs are [well-documented](0010-documentation.md)
* Stability promotes [reusability](0011-reusability.md)

## Notes

How can we promote stability under changing requirements?

Further reading:

* [Wikipedia: Stability Model](https://en.wikipedia.org/wiki/Stability_Model)
