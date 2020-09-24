# Quality Requirement: Documentation

* Name: Documentation
* Reference Number: 0010
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

Documentation includes the following materials:

* Interface documentation
* Usage guides
* Examples
* Rationale
* Recommendations
* Guidelines

## Scenario

* A developer looks up documentation for a subsystem/driver/core component, and finds the following:
	* API documentation
	* Usage guide
	* Examples showing usage of the subsystem
* A developer searches the documentation and receives a list of results matching his search

## Test Cases

N/A

### Landing Zones

| Quality | Measurement | Minimum | Target | Oustanding |
|---------|-------------|---------|--------|------------|
| Documentation Pages with Images | % | 50% | 85% | 95% |
| Documentation Pages with Examples | % | 50% | 85% | 95% |
| Documentation pages with Rationale | % | 50% | 85% | 95% |

## Context

In order for developers to work effectively with our framework, they must be able to access reference information and examples that describe how it works and how it should be used.

Quality documentation enables developers who are using the framework to be more productive. Quality documentation also reduces Embedded Artistry's support burden.

## Tradeoff Notes

Documentation takes time away from feature development and support. However, neglecting documentation increases the support burden, as users will not understand new features.

## Related Qualities

* Documentation aids [Serviceability](0012-serviceability.md)
* Documentation one aspect of [Productivity](0001-productivity.md) (and also a child quality)

## Notes

Tutorial articles and videos can help on the documentation front. A short, simple video can replace lots of reading recommendations.

Focus on visual guides and examples over wordy test.
