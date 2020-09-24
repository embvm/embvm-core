# Stakeholder Analysis

* **Name:** Dwight Crow
* **Company:** Whisper
* **Role:** CEO

Dwight is the CEO of Whisper, a startup focused on developing the best hearing aid in the world. Their primary IP is in their processing algorithms.

## Background Notes

Whisper is a multi-part embedded system with extremely tight power and latency reuqirements. They are multiple real-time systems and use a couple of RTOSes.

* nRF52
* STM32
* Qualcomm chip w/ DSP

They are writing their own RF protocol, which is painful. They are worried about latency and timing - 2ms timing window, nRF52 interrupts + messages DSP within 100usec window.

## Related Documents

N/A

## Stakeholder Story

Dwight is the CEO of Whipser. Whisper is building a complex multi-system embedded device which runs their proprietary algorithms. Due to latency requirements, Dwight is convinced that he is tightly coupled to his hardware platforms and cannot be disentangled without sacrificing latency and control. As a result, Dwight is at risk of rewriting entire parts of his system if there is a required component change. Currently, Dwight doesn't see it any other way.

## Stakeholder Analysis

### Responsibilities

* Hiring
* Setting the schedule
* Raising money
* Marketing
* Protecting his IP and keeping as much in-house as possible

### Business Goals

* Develop the best hearing aid in the world
* Build cool stuff
* Build a profitable business
	* Tap into a multi-billion dollar industry by providing the best hearing aid

### Personal Goals

* Wants his company to succeed
* Really wants to get his product out into the world
* Wants to avoid using consultants

### Goals for Our System

* Low-power, provide hooks for reducing power
* Performant: they have tight latency and timing requirements
	* 2ms timing window
	* nRF52 fires interrupt and shoots over to DSP in 100us
* Faster board bring-up

### Value Proposition for Our System

Currently, Whisper is not a target for our system. They are interested in being tied to hardware because of their perceived performance requirements, and are prepared to throw away software and start over if a new chip is needed.

However, we can still propose:

* Reusability of higher-level application logic
* Portability of code such as OTA

Can we provide performance guarantees that make our framework more interesting?

## Empathy Map

Dwight is the CEO of Whisper

### What are they seeing?

* Firmware hires take a long time - 3.5 months for first hire
* The guy writing the original firmware, who is extremely capable, is moving into the VP of hardware role
* Complex system with multiple components and vendor SDKs
* The calendar dates flying by (moving slower than expected)

### What are they hearing?

* Developer cursing at the different chips and SDKs
* Complaints about poor documentation
* "We don't believe that DSP exists" from the contract development house (re: Qualcomm)

### What are they saying?

* We need to get every bit of performance possible out of the chips
* I will have to do OTA, but I really don't want to deal with that headache
* I need to hire 2-3 more firmware engineers
* I need people who can do hardware bring-up somewhere
* I need people who can read a schematic
* Nordic SDK is good - only chip people don't complain about, datasheets are accurate, code bases are sane, documentation is good
* We hate working with Qualcomm
* We hate being forced to work through Mistral/Intrynsic

### What are they thinking?

* Our latency requirements are so tight that something like this likely won't work
* We are already tied to using our chips in a custom, low-level way. Can we really break away from the vendor SDKs?
* We have to support our own custom RF protocol
* Only a small part of our application is generic or reusable

### What are they feeling?

* Success - got 14 million dollars
* Frustration - they are "behind schedule" and the project isn't moving as fast as they want it to
* Frustration - dealing with vendors and having pieces of the system outside of their control

### What are they doing?

* Hiring
* Avoiding consultants, trying to keep IP in house
* Managing all of their source code in a single repository
	* including the fact that there are different platforms!
* Architecting a communication layer with strict guarantees that ensures they can make changes and it will work on other platforms
* Committing to STM32 as the primary controller
* Lots of prototyping
