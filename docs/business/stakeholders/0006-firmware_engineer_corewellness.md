# Stakeholder Analysis

* **Name:** Brian Sutton
* **Company:** Core Wellness
* **Role:** Firmware Engineer (The only one)

Brian is the sole firmware engineer at Core Wellness. He transitioned their initial firmware from an EE-written runloop to a RTOS-based system.

## Background Notes

* Brian is a solo firmware engineer
* He must leverage existing tools and code in order to meet schedules and ship their design quickly
* He is re-writing many features we have used on existing projects before, which are also being considered in the framework
* Core is shipping an embedded system

## Related Documents

N/A

## Stakeholder Story

Brian is the only firmware engineer on staff at Core Wellness. Brian has spent three out of his current four months at Core Wellness re-implementing features which he has used on other projects. These features include logging, a driver model, dispatch queues, lock guards, and command lines. Brian is impressed by the amount of work that can be done by a single engineer given a quality toolkit. Brian is interested in finding a toolkit which supports all of the structures he needs out of the box, rather than continually re-implementing them himself.

## Stakeholder Analysis

### Responsibilities

* Bring-up of hardware and new drivers
* Communicating with his team about firmware, schedules, risk, and impact
* Developing firmware features
* Debugging
* Insuring firmware builds are qualified before releasing updates (uses checklists)

### Business Goals

* Ship a good product, free of bugs
* Don't introduce a firmware build which causes a catastrophic failure
* Meet schedules & ship quality features

### Personal Goals

* Minimize stress
* Minimize long-hours and weekend work
* Leverage existing work to be more effective

### Goals for Our System

* Minimize the amount of time to be spend re-writing structures that have been previously used on other projects
* Focus more time on developing features
* Reliability
* Quality API design
* Future-proof against portability problems

### Value Proposition for Our System

* Decouple your software from a specific vendor SDK
* Provide features which talented embedded developers expect
* Having our SDK would have saved 3+ months of development time. He spent 3 months recreating the features that we are going to create
	* This time was spent before he could even focus on the feature development for the product
* Low cost of entry to techniques like static analysis, testing, host-side development
* Modular software
* Dispatch support w/ events

## Empathy Map

### What are they seeing?

* People who don't understand how firmware works
* Tight schedules
* Long feature lists
* SDK with good documentation, but still shitty design

### What are they hearing?

* "You need to meet this crazy deadline"
* "We need to release firmware on a regular cadence"

### What are they saying?

* We need to cut that feature
* That's a bad idea…
* I could have saved 3 months if I had all of this at the start
* I really wish my command line interface was better
* Nordic's SDK seems to have an RTOS tacked on - interrupt handlers are too heavy

### What are they thinking?

* My coworkers don't know anything about firmware
* The expectations they have are unreasonable and unrealistic
* I need to keep redoing things I've done before

### What are they feeling?

* Overwhelmed by schedule push
* As if he needs to convince his team of his knowledge (he's one of two people who have shipped a product)
* As if others aren't listening to him
* As if he's reinventing the wheel unnecessarily

### What are they doing?

* Cutting features to meet deadlines
* Trying to explain to the team to help them understand firmware concepts (multi-stage boot, bootloaders, OTA, etc.)
* Re-writing features and constructs that he used at other companies/projects
