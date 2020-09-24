# Stakeholder Analysis

* **Name:** Dan Post
* **Company:** Motiv
* **Role:** Director of Software

Dan Post is the director of software at Motiv, who builds a tiny fitness ring for health monitoring. He is previously a firmware engineer and is an adept software person. He's focused on more of a manager.

## Background Notes

Motiv's product is a multi-chip embedded system that his highly memory constrained and may operate for multiple days without connecting to a phone. They are building a second product with improved capabilities. New features are being added, but SRAM is currently at its limit.

They will need help porting the existing software to the new hardware platform, as well as splitting it into threads.

Testing and HALs are not yet in place.

Currently 1 firmare engineer and 1 EE (cofounder).

## Related Documents

N/A

## Stakeholder Story

Dan is the director of software at Motiv. His company has shipped their first product, and they have a second product that is starting design & development. Dan's staff is extremely limited, though he is looking to add developers in the future. Dan needs to be able to support multiple platforms, migrate toward a HAL & threading model, and bring-up new systems quickly while supporting existing systems.

## Stakeholder Analysis

### Responsibilities

* Hiring
* Managing all software efforts
* Software development
* Monitoring returns and identifying issues

### Business Goals

* Build a successful product (V1)
* Build a second generation product
* Add new features to the launched product
* Improve features & fix bugs driving RMAs

### Personal Goals

* Wants to build something bigger than he can build on his own
* Wants to grow a team and build the kind of environment that he never had

### Goals for Our System

* Provide a HAL
* Enable unit-testing and improved reliability
* Support threading / RTOS model
* Quickly port the system to new hardware
* Develop + test software on the host system (He previously did this at square and is really in love with the idea)

### Value Proposition for Our System

* Host simulation
* HAL
* Quick porting
* Testing hooks - they don't have much testing so we can add even more

## Empathy Map

Dan is the Director of Software at Motiv

### What are they seeing?

* Memory shrinking
* Flash storage shrinking
* Team is being pulled in different directions - not enough time
* Firmware engineer he hired not showing up for the job

### What are they hearing?

* need to add another feature (no space!)
* Need to develop a new product
* Need to finalize designs ASAP. Apple bought our remaining capacity. We can't delay your production any longer.
* "I'm not available for 2-3 months"

### What are they saying?

* "We haven't gotten around to that yet"
	* HAL
	* Testing / regression tests / cycle tests
* "We probably have to throw about half of that out"
	* Regarding bring-up done so far
* "We started that but haven't had time to finish"

### What are they thinking?

* How can I support an existing product and develop a new one with my current resource?
* How can I fit these features into the remaining space I have left?
* Will I get the budget?
* Will I be able ot hire enough quality people?

### What are they feeling?

* Unsure at the budget
* That hiring will be hard and will take some time
* Firmware engineers are hard to find

### What are they doing?

* Hiring
* Managing a team
* Supporting firmware, app, backend, etc.
* Supporting new product development efforts and sustaining product efforts
* Reaching out to consultants
* Planning efforts months in advance (good!)
