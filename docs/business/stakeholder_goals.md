# Stakeholder Goals

**Table of Contents:**
1. [Stakeholder List](#stakeholder-list)
2. [Goals](#goals)
	1. [Functional](#functional-goals)
	2. [Documentation](#documentation-goals)
	3. [Quality](#quality-goals)
	4. [Architectural](#architectural)
	5. [Business](#business-goals)
3. [Goals by Milestone](#goals-by-milestone)
	1. [Functional](#functional-goal-milestones)
	2. [Documentation](#documentation-goal-milestones)
	3. [Quality](#quality-goal-milestones)
	4. [Architectural](#architectural-goal-milestones)
	5. [Business](#business-goal-milestones)

## Stakeholder List

* Embedded Artistry
	* Phillip, as [CEO](0010-EA_CEO.md) and [Developer](0009-EA_developer.md)
	* Rozi, [Project Manager](0011-EA_project_manager.md)
* External
	* CEOs
		* Aaron, [CEO of Point One](0002-pointone_ceo.md)
		* James, [CEO of Amina](0001-amina_ceo.md)
		* Dwight, [CEO of Whisper](0004-ceo_whisper.md)
	* Directors
		* Dan, [Director of Software at Motiv](0005-director_sw_motiv.md)
		* Tyler, [Director of Engineering at Bolt](0007-director_engineering_bolt.md)
		* Saket, [Director of Hardware at Noon Home](0008-director_hardware_noon.md)
	* Firmware Engineers
		* Brian, [engineer at Core Wellness](0006-firmware_engineer_corewellness.md)
		* Wynn, [engineer at Morrison Weighing](0003-morrison_weighing_engineer.md)

## Goals

The following goal categories are used:

* [Functional Goals](#functional-goals): 7
* [Documentation Goals](#documentation-goals): 5
* [Quality Goals](#quality-goals):
* [Architectural Goals](#architectural-goals): 9
* [Business Goals](#business-goals): 11

### Functional Goals

* **Unit Test Support:** The framework should enable developers to write and run unit tests
	* Stakeholders: Dan, Phillip
* **Built-in Static Analysis:** The framework should ship with static analysis support, enabling users to test with al ow barrier to entry
	* Stakeholders: Dan, Phillip, Brian
* **Panic Support:** The framework should provide a panic/assert handler with support for printing a backtrace and processor registers for debugging purposes
	* Stakeholders: Brian, Phillip
* **Logging Support:**
	* Stakeholders: Saket, Phillip
* **Parametric Data Collection:**
	* Stakeholders: Saket
* **Command Line Interface:** The firmware should work with a command line interface for development + debugging
	* Stakeholders: Phillip, Brian
* **Threading/RTOS Support:** The framework should support threaded development models using an RTOS
	* Stakeholders: Dan, Phillip, Brian
* **Single Thread Support:** The framework should support applications which do not need threading or an RTOS
	* Stakeholders: Phillip
* **Host Simulation Support:** Developers should be able to use the framework to write drivers, libraries, and test code on their host machine
	* Drivers can be written for debug adapters, allowing driver development against samples / breakout boards
	* Stakeholders: Phillip, Dan

#### Documentation Goals

* **Targeted Documentation:** Documentation should be targeted to different stakeholders so that they will be able to find and understand the parts of the framework they need to know about
	* Target your documentation to the specific audiences
	* Stakeholders: Phillip, Rozi, Saket
* **Single Point of Management:** Documentation, issues, templates, and source should all be available in a single location
	* Stakeholders: Phillip, Rozi
* **Searchable Documentation:** The documentation should be searchable
	* Stakeholders: Rozi, Phillip
* **Concept Explanations:** Subsystems and concepts should have clear explanations
	* Documentations hould support firmware engineers in helping their team understand complex concepts (multi-stage boot, bootloaders, OTA)
	* Stakeholders: Brian, Rozi
* **Document Rationale:** The rationale for decisions and structure should be made available, so others can understand WHY decisions were made
	* Stakeholders: Phillip, Rozi

### Quality Goals

* **Usability:** Client teams should be able to understand and operate effectively using the framework
	* "How long will this take me to learn?"
	* "How much C++ do I need to learn to be useful on this project?"
	* Stakeholders: All
	* AKA **Productivity**
* **Reliability:** The framework should work reliably and be free of errors
	* The framework should be well-tested to ensure there are not failures on delivery
	* Framework should utilize static analysis and unit testing
	* Stakeholders: James, Wynn, Brian, Phillip, Dan

### Architectural Goals

* **Low-Power:** The framework should provide hooks for reducing power consumption & controlling the system power state
	* Stakeholders: Phillip, Dwight, Dan
* **Low-latency:** The framework should not add significant latency to interrupts, tasks, etc.
	* Some systems require tight timing and latency requirements
		* nRF52 fires interrupt and shoots over to DSP in 100us
	* "We need to get every bit of performance possible out of the chips"
	* "Our latency requirements are so tight that something like this likely won't work"
	* Stakeholders: Phillip, Dwight
* **Portable:** Decouple application, libraries, and utilities from the underlying hardware & OS
	* Port higher-level software across different platforms by providing equivalent APIs on a new platform
	* Make software robust to underlying hardware changes
	* Stakeholders: Phillip, Brian, Wynn, Tyler, Saket, Aaron, Dan
* **Reusable:** Develop generic abstractions, subsystems, and utliities so that they can be re-used in a variety of systems
	* Reuse of software improves reliability and confidence
	* Reusable components speed up development
	* Framework core should be reusable across a variety of systems as a project skeleton
	* Stakeholders: Saket, Phillip, Dwight, Brian, Rozi, Tyler
* **Suitable for Small Memory Systems:** The framework core components should be small enough that they can be used on systems with limited memory (flash or RAM)
	* Stakeholders: Dan Post, Phillip, Brian, Saket
* **Multi-Product Support:** The framework should enable users to support multiple products within a single repository
	* Stakeholders: Phillip, Dwight, Dan, Wynn
* **Event-driven:** The framework should be structured so that an event driven model can be used
	* Stakeholders: Brian, Phillip, Dan
* **Configurability:** Settings and options can be modified by the user/implementor and specified per-application or per-platform
	* Experienced should be able to override default behavior and customize the system to meet their needs
	* Stakeholders: Wynn, Dwight, Phillip
	* Example: Wynn wants to control the algorithms used in software depending on the client's specific requirements
* **Extensibility:**
	* Users should be able to add their own drivers, platforms, processors, libraries, and applications
	* Users should be able to extend our abstractions to meet their program needs (e.g., adding custom platform APIs)
	* Stakeholders: All

### Business Goals

EA-related:

* **Better Sales:** The framework and its lessons should result in improved sales and negotiations with clients
	* Stakeholders: Phillip, Rozi
* **Better Projects:** The framework should result in better projects for EA
	* De-risks projects
	* Projects completed faster
	* Higher value for clients
	* Not re-implementing work that was previously done
	* Isolate from client not having requirements: We do bring-up, they do logic
	* Stakeholders: Phillip, Rozi
* **Profitability:** The framework should generate profit for Embedded Artistry
	* Stakeholders: Phillip, Rozi
* **Customize for the Client:** The framework should be delivered to the client fully-customized and ready for their use
	* Stakeholders: Phillip, Rozi
* **Support our clients:** We focus on fixing framework issues and bringing up new drivers/processors for our clients
	* Stakeholders: Phillip, Rozi

Customer-related:

* **Capture Best Practices:** The framework is built on learnings and best practices from the industry
	* We capture the important details and put them to good use
	* Prevent repeating the same mistakes over-and-over again
	* Guard your system against inexperienced engineers making bad design choices
	* Leverage quality features that have been thoroughly tested
	* Stakeholders: Phillip, Saket
* **Affordable Cost:** The framework should be available at a price point which other companies can afford
	* This holds for the early-stage companies
	* Stakeholders: James, Aaron, Dan Post
* **Faster bring-up:**
	* Board bring-up time reduced: develop drivers while waiting for boards, re-use existing drivers
	* Faster turnaround on hardware changes due to application portability
	* Stakeholders: Brian, Phillip, Saket
* **Shorter development schedules:**
	* Save 3-4 months while waiting to hire firmware engineers
	* Save 3+ months typically spent recreating firmware features used on previous projects
	* Enable companies to start developing product features faster
	* Enable companies to start testing and prototyping faster
	* Stakeholders: Dwight, Dan, Aaron, Tyler, Brian, James, Rozi, Saket
* **Reduce firmware headcount:**
	* Reduces the effort level typically given to hardware support and driver bringup
	* Only need 1 person to support low-level firmware activities
		* EA can support via framework while hiring takes place
		* EA can train the person so they are effective on the project out of the gate
	* Focus on generalists who can develop the application logic, API design, and integration with external systems
	* Stakeholders: Saket, Dwight, Dan, Aaron, Tyler
* **Focus on Application, Differentiation, & Product Value:** The framework should enable client companies to spend more time focused on their core IP and developing new features, rather than bring-up and low-level firmware
	* Experts take care of the stable firmware base, while firmware team can focus on the ways to add value to the business/product
	* Enable firmware team to focus on higher-value activities, such as service/app APIs and integrations with other hardware platforms
	* Stakeholders: Saket, Dwight, Brian, Aaron, Tyler, James

## Goals by Milestones

### Functional Goal Milestones

<center>

| Goal Name | Stakeholders | Core + Phase1 | Panic | CLI | Logging & Parametrics |
| --------- | ------------ | -------------- | ----- | --- | --------------------- |
| Unit Testing | Dan, Phillip | x |  |  | |
| Static Analysis | Dan, Phillip, Brian | x |  |  | |
| Panic Logs | Brian, Phillip |  | x |  | |
| Logging | Saket, Phillip, Brian | x |  |  | x |
| Parametric Data Collection | Saket |  |  | | x |
| Command Line Interface | Brian, Phillip |  | | x | |
| Single Thread | Phillip | x |  |  |
| Threading/RTOS | Dan, Phillip, Brian, Saket | x |  |  | |
| Host Simulation | Phillip, Dan | x |  |  | |

</center>

### Documentation Goal Milestones

<center>

| Goal Name | Stakeholders | Core + Phase1  | Panic | CLI | Logging & Parametrics |
| --------- | ------------ | -------------- | ----- | --- | --------------------- |
| Targeted Documentation | Rozi, Phillip, Saket | x |  |  |  |
| Single Point of Management | Rozi, Phillip | x |  |  |  |
| Searchable Documentation | Rozi, Phillip | x |  |  |  |
| Concept Explanations | Brian, Rozi | x | x | x | x |
| Document Rationale | Phillip, Rozi | x | x | x | x |

</center>

### Quality Goal Milestones

<center>

| Goal Name | Stakeholders | Core + Phase1 | Panic | CLI | Logging & Parametrics |
| --------- | ------------ | -------------- | ----- | --- | --------------------- |
| Usability (Productivity) | All | x | x | x | x |
| Reliability | James, Wynn, Brian, Phillip, Dan | x |  |  |  |

</center>

### Architectural Goal Milestones

<center>

| Goal Name | Stakeholders | Core + Phase1 | Panic | CLI | Logging & Parametrics |
| --------- | ------------ | -------------- | ----- | --- | --------------------- |
| Low-Power Capable | Dwight, Phillip, Dan | x |  |  |  |
| Low-latency | Phillip, Dwight | x |  |  |  |
| Portable | Phillip, Brian, Wynn, Tyler, Saket, Aaron, Dan | x |  |  |  |
| Reusable | Saket, Phillip, Dwight, Brian, Rozi, Tyler | x | x | x | x |
| Suitable for Small Memory Systems | Dan, Phillip, Brian, Saket | x |  |  |  |
| Multi-Product Support | Phillip, Dwight, Dan, Wynn | x |  |  |  |
| Event-Driven | Phillip, Brian, Dan | x |  |  |  |
| Configuration | Wynn, Dwight, Phillip | x |  |  |  |
| Extensibile | All | x |  |  |  |

</center>

### Business Goal Milestones

<center>

**EA Goals**

| Goal Name | Stakeholders | Core + Phase1 | Panic | CLI | Logging & Parametrics |
| --------- | ------------ | -------------- | ----- | --- | --------------------- |
| Better Sales | Phillip, Rozi | ? | ? | ? | ? |
| Better Projects | Phillip, Rozi | x | x | x | x |
| Profitability | Phillip, Rozi | ? | ? | ? | ? |
| Customize for the Client | Phillip, Rozi | x |  |  |  |
| Support Our Clients | Phillip, Rozi | x |  |  |  |

</center>

<center>

**Customer Goals**

| Goal Name | Stakeholders | Core + Phase1 | Panic | CLI | Logging & Parametrics |
| --------- | ------------ | -------------- | ----- | --- | --------------------- |
| Capture Best Practices | Phillip, Saket | x | x | x | x |
| Affordable Cost | James, Aaron, Dan | ? | ? | ? | ? |
| Faster Bring-up | Brian, Phillip, Saket, Aaron | ? | x | x | x |
| Shorter Development Schedules | All | ? | x | x | x |
| Reduce Firmware Headcount | Saket, Dwight, Dan, Aaron, Tyler | x | x | x | x |
| Focus on Application, Differentiation, and Product Value | Saket, Dwight, Brian, James, Aaron, Tyler | x | x | x | x |

</center>
