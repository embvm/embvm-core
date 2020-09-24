# Quality Requirement: Resilience

* Name: Resilience
* Reference Number: 0003
* Parent: N/A
* Children:
    * [Portability](0008-portability.md)
    * [Extensibility](0004-extensibility.md)
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

Resilience is a measure of a system’s ability to survive and persist within a variable environment. The opposite of resilience is brittleness or rigidity. Resilience can also be described as "elasticity" in a system.

To quote Donna Meadows (*Thinking in Systems: A Primer*, p. 78):

> I think of resilience as a plateau upon which the system can play, performing its normal functions in safety. A resilient system has a big plateau, a lot of space over which it can wander, with gentle, elastic walls that will bounce it back, if it comes near a dangerous edge. As a system loses its resilience, its plateau shrinks, and its protective walls become lower and more rigid, until the system is operating on a knife edge, likely to fall off in one direction or another whenever it makes a move. Loss of resilience can come as a surprise, because the system usually is paying much more attention to its play than to its playing space. One day it does something it has done a hundred times before and crashes.

## Scenario

TBD

## Test Cases

TBD

### Landing Zones

N/A

## Context

The framework’s main contribution to the end application is the architecture we define. Therefore, it’s imperative to design the framework to be as flexible and extensible as possible.

Loses resilience - plateau shrinks, walls lower, become rigid, until the system opeates on a knife edge

## Tradeoff Notes

* Resilience can be at odds with [Stability](0005-stability.md) and [Productivity](0001-productivity.md)
* Resilience can be hard to see unless you exceed its limits + break down the system

People may try to beat the rules that keep the system resilient. Donna Meadows recommends a way out of rule beating:

> The way out of the trap, the opportunity, is to understand rule beating as useful feedback, and to revise, improve, rescind, or better explain the rules. Designing rules better means foreseeing as far as possible the effects of the rules on the subsystems, including any rule beating they might engage in, and structuring the rules to turn the self-organizing capabilities of the system in a positive direction.

## Related Qualities

* The child qualities of Resilience are:
    * [Portability](0008-portability.md)
    * [Extensibility](0004-extensibility.md)
* [Security](0007-security.md) improves Resilience
* [Maintainability](0013-maintainability.md) improves Resilience

## Notes

Quote regarding system resilience:

> In the aspect of engineering and construction, resilience is a objective of design, maintenance and restoration for buildings and infrastructure, as well as communities. It is the ability to absorb or avoid damage without suffering complete failure. A more comprehensive definition is that it is the ability to respond, absorb, and adapt to, as well as recover in a disruptive event. **A resilient structure/system/community is expected to be able to resist to an extreme event with minimal damages and functionality disruptions during the event; after the event, it should be able to rapidly recovery its functionality similar to or event better than the pre-event level.**

Quotes from *Thinking in Systems: A Primer* by Donna Meadows:

> If you are the intervenor, work in such a way as to restore or enhance the system’s own ability to solve its problems, then remove yourself.

> To be a highly functional system, hierarchy must balance the welfare, freedoms, and responsibilities of the subsystems and total system—there must be enough central control to achieve coordination toward the large system goal, and enough autonomy to keep all subsystems flourishing, functioning, and self-organizing. Resilience, self-organization, and hierarchy are three of the reasons dynamic systems can work so well. Promoting or managing for these properties of a system can improve its ability to function well over the long term—to be sustainable.
