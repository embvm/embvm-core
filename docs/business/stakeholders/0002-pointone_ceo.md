# Stakeholder Analysis: CEO of Point One

* **Name:** Aaron Nathan
* **Company:** Point One
* **Role:** CEO

Aaron Nathan is the CEO of Point One. He is a firmware engineer by training. They are building a high-precision GPS location service which involves an embedded device.

Point One is a Bolt company.

## Background Notes

* Amina has contacted Embedded Artistry about porting efforts
* Their schedule is aggressive and their staff is limited
* Their initial set of software was tied to the chips they selected initially
* HAL work is in progress

## Related Documents

* [Amina CEO](0001-amina_ceo.md) describes another Bolt company with a similar request
* [Bolt](0007-director_engineering_bolt.md) is the investing VC firm

## Stakeholder Story

Aaron is creating a new product with limited staff and resources. He also has an aggressive schedule to push the team toward monetizing and testing the product as quickly as possible. Aaron's team has recently redesigned the product hardware, requiring use of new processors and active electrical components. Because of initial firmware implementation decisions, the firmware must be largely re-written to support the new parts. With an impending manufacturing date on the horizon, Aaron's team values being able to quickly migrate to a new platform.

## Stakeholder Analysis

Aaron is the CEO of Point One

### Responsibilities

* Running a company
* Getting investors
* Setting + meeting milestones
* Hiring engineers
* Deciding on product features
* Sales + marketing

### Business Goals

* Sell product & make money
* Provide a profitable product/service
* Keep company open
* Hire + employ talented engineers
* Pay people on time
* Avoid bad press
* Keep to the schedule

### Personal Goals

* Increase business value and exit

### Goals for Our System

* Decouple application logic from hardware/OS
* Faster turnaround on hardware changes
* Hire less specialized developers because firmware is taken care of
* Improve quality & security
* Take a headache off of his plate

### Value Proposition for Our System

* Decouple SW from hardware - resilience against future HW/chip changes
* Decouple SW from OS
* Faster bring-up time on new boards
* Focus on application logic work, not on porting + starting over
* Less struggle hiring - no need for deep FW experts

## Empathy Map

Aaron is the CEO of Point One

### What are they seeing?

* Engineer working hard, but still not able to meet the schedule
* New hardware coming in
* Delays
* Calendar with impending date coming closer
* My high consulting fees

### What are they hearing?

* "I need more time"
* "Your schedule is too tight"
* "You need to be profitable"
* "Move fast and fix it later"
* "Be agile"
* "We are booked right now, here is another consultant you can talk to"

### What are they saying?

* "I'd love to give you feedback on the SDK"
* "We need new hardware for our production run in July"
* "We can do that / meet that schedule"
* "There's just no way for our firmware engineer to get it done on his own"
* "Your rates are too expensive for our budget at this time"

### What are they thinking?

* How will I get this done?
* This consultant is out of our price range

Framework:

* Does this get me to my products faster?
* Is this too expensive?
* Will my team understand it?
* Does this help me evolve my design faster?

### What are they feeling?

* Pressure from investors re: profit
* Pressure of having employees
* Pressure from not meeting schedule
* Pressure - can't hire firmware engineers
* Like he just can't get it done on his own

### What are they doing?

* Continuing with the crazy schedule
* Committing to vendor code generator + porting code over on their own
	* Getting into another trap
* Talking to consultants
