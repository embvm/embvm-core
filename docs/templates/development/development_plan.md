Create a development plan

The project specification describes what features are needed. By contrast, the development plan describes how the features will be implmeented. It sets developer goals and priorities, and describes the development tools and environment.

Development plan documents are especially helpful to new team members because they can become productive without taking much time from other developers.

Though it's best to start with a basic document, all the following information is helpful:

* Architecture
* Workstation setup
* Server setup (optional)
* Glossary of project terms
* Coding style guidelines (optional)

If the project is small enough, a single documentacan capture all of the above details. Otherwise, I recommend separate documents so team members can more easily find the information they need.

If not stated int he project spec, include the following items in the development plan:

* expected user background or skill level
* target hardware platforms, operating systems, browsers, etc.
* Minimum harware, operating system, monitor resolution

Also include:

* Programming languages to be used
* Tools to be used for source control, builds, integration, testing, and deployment
* High-level physical architecture: projects, components, file locations, and naming conventions
* High-level logical architecture: major sub-systems and frameworks
* List of development priorities
* Uniform strategy for bug prevention, detection, and repair
* Uniform strategy for program robustness
* Development timeline and milestones

Software architecture

* High level design of the program
* Design implies intentional planning as opposed to haphazard structure
* I know immediately that a proejct is troubled if I can't locate a high-level design document, or if the developers can't describe the design to me. This usually implies that coding began before an architecture was developed. The design is accidental, so no one knows what it is.
    * Consulting principe
    * Red flag
* Without an architecture, there's no blueprint for software development. You have incremental micro-designs based on micro-requirements, but not the benefit of broader goals and coordination. Developers are likely to get lost, leading to wasted time and rework.
* An architecture can be improvised, but it requires considerable skill. Even for experts, it's a risky approach. Your time is better spent upfront, preparing a good architecture, instead of patching or reworking an accidental or improvised one.
    * Sub-principle.
* It's usually best to describe the architecture sing both diagrams and text, especially the logical hierarchy/layering of the system. Beware: activity diagrams, flowcharts, and sequence diagrams describe operation, not architecture

Project glossary

* Every domain uses distinct words and phrases that are unfamiliar to developers
* The program source code normally reflects the domain, so it's helpful to define domain-specific terms in a project glossary
* Helps ensure consistency of names used in the source code
* PJ: Build this for framework and libraries

Document workstation & server setup. If using CI, document the build server setup guide.

Fault handling

* If a module is not defined before implementation, how can you tell if it works correctly or will integrate with other modules?
* Unit tests can determine if a module works the way the implementer expects, but not if it meets the expectations of others
* Unit tests don't guarantee that the module will integrate properly with other modules
* Strategies fo rhandling software bugs:
    * Ignore faults
    * Fault prevention
    * Fault detection
    * Fault tolerance
    * Fault correction
* A fault-tolerant product is well-behaved when an unexpected event occurs. We usually call fault-tolerant software robust. If an app can't perform the requested op, it shouldn't crash, corrupt our database, or format your hard drive
* For your proejct, the fault strategies and implementation should be documented so every developer uses the same approach. Left undocumented, the program will use a mix of (possibly redundant) styles and strategies that may not work well together.

Priorities

* Most teams don't prioritize the internal attributes of a progrma, such as maintainability and flexibility
* To avoid miunderstandings, priorities should be established before coding begins. He recommends the following priorities for most projects:
    * 1. Complete - fulfills the entire requirement
    * 2. Correct - does't have bugs
    * 3. Flexible/maintainable - easy to understand and modify
    * 4. Stable - continues working despite changes in other areas of the program
    * 5. Timely - completed within the agreed time estimate, while fulfilling the preceding conditions

Coding style: A small set of rules is usually best

Practical advice: Documentation is the only way to transfer knowledge without describing things in person.

Well-written source code is valuable, but code alone doesn't explain domain terminology, development priorities, and other important aspects of a proejct.

Development plan docs should be kept in a shared location, preferably using source control. [PJ: keep them in your repo?]

Principles:
Projects can go astray due to too little or too much planning. Too little planning leads to ambiguity, confusion, and unnecessary work. Too much planning may produce details that become obsolete due to requirements or design changes.
