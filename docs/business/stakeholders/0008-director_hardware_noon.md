# Stakeholder Analysis: Director of Harware at Noon Home

* **Name:** Saket Vora
* **Company:** Noon Home
* **Role:** Director of Hardware

Saket is an EE with experience shipping hardware products and product ecosystems. Saket is the director of hardware at Noon Home, manging hardware and firmware activities.

## Background Notes

* Saket manages the hardware and firmware teams at Noon Home
* Saket has worked on multiple hardware products in the past
* Saket has asked us for support on Noon Home products with regard to bring-up/firmware porting

## Related Documents

N/A

## Stakeholder Story

Saket is the Director of Hardware at Noon Home. Saket is responsible for coordinating hardware and firmware efforts, and is constantly trying to encourage communication and design across teams. Saket notices that firmware engineers have often painted themselves into corners, requiring re-architecture efforts to support use cases required by the software and product teams. Saket is interested in ways to leverage external experts to reduce internal headcount. Saket is also focused on business value: how can the team directly increase the value of the busienss and product? This means focusing on product features, not on firmware and hardware low-level details. As such, Saket is interested in getting more computer-science focused developers working on embedded projects.

## Stakeholder Analysis

### Responsibilities

* Managing hardware team
* Managing firwmare team
* Communicating and planning activities with the larger team (product, software)
* Planning builds
* Coordinating & encouraging architecture efforts

### Business Goals

* Ship quality products on time and on budget
* Support software/service efforts through firmware support
* Identifying high-value ways to apply team member efforts (e.g., work on the things that provide the highest value for the company)
* Prevent his team members from being overworked
* Prevent his team members from being jerked around by other teams

### Personal Goals

* Conveying information in a way that other stakeholders will listen/receive
* Leveraging his experience on past products to make the current/future products more succssful

### Goals for Our System

* Provide abstractins between software layer and platform/processor layer
* Reduce the effort level given to hardware support and driver bringup
* Enable firmware team to focus on higher-value activities, such as service/app APIs and integrations with other hardware platforms
* Enable experienced programming generalists to work on firmware, rather than just low-level hardware engineers

### Value Proposition for Our System

* Reduce total number of firmware engineers that need to be dedicated to bring-up/hardware support
* Prevent inexperienced firmware engineers without a CS focus from making design decisions which will be catastrophic later in the project
* Save time by using tested constructs out of the box for things like logging, system configuration, parametric data collection
* Experts take care of the stable firmware base, while firmware team can focus on the ways to add value to the business/product

## Empathy Map

Saket is the Director of Hardware at Noon Home

### What are they seeing?

* Firmware being re-written and refactored after ship date
* Features being added in late in the project, when they weren't architected for
* Teams not communicating effectively
* Schedule slips
* Money running out and sales not doing well

### What are they hearing?

* Firmware engineers stating assumptions about how something would be handled.
	* App/service engineers stating different assumptions about how something would be handled
* "We didn't plan for that"
* "That is going to take two months to do"

### What are they saying?

* "Do you prefer a quick hack with a 10min update time, or two month schedule slip with 30s update time?"
* "We need to talk together more and earlier"
* "On X product, we did Y and Z happened" (sharing earlier experience & examples)
* "You need to find the leaders or engineering heads who have the battle scars, they will understand the value of the framework"
* "Inexperienced teams think that they won't make the same mistakes, and then they walk right into the trap"

### What are they thinking?

* Product managers don't handle the tradeoffs and consequences well
* Potentially dealing with an inexperiencd team who seems to be committed to making similar mistakes to other teams
* These issues could be prevented with up front architecture
* What is the right amount of architecture?
* How can I effectively communicate the value and my messages?

### What are they feeling?

* Frustration at trying to communicate difficulties with inexperienced stakeholders and failing
* Frustration at the lack of empathy among the teams
* Thoughtful about how to make the situation better

### What are they doing?

* Talking to outside consultants about helping his team address the issue
* using his experience on other products and sharing the learnings
* Reflecting on how they got to this state and how he can prevent it in the future
* Learning more about architecture and business and firmware

## Notes

* What if you did have this framework model?
    * 1-2 people on firmware
    * FOcus more energy on a rock-solid application layer and having more of teh senior people work on connectivity and user facing issues
    * Have less people who are there needing to grind through a bring-up process
    * Would have steered people to more higher level tasks
