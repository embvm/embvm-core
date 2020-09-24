# Using the Framework

The purpose of a framework is to improve the efficiency of creating new software. Frameworks can improve developer productivity and improve the quality, reliability and robustness of new software. Developer productivity is improved by allowing developers to focus on the unique requirements of their application instead of spending time on application infrastructure (“plumbing”).


I am building a C++14 (or 17) SDK which has well-defined HALs, and I'll be implementing many of the language features using an RTOS or two (std::mutex, std::thread, etc.).

That will enable me to simulate an application and drivers on a host PC. Unit testing becomes possible on a host machine - I can use mock objects for the lower-level hardware interfaces

I will also write drivers for some USB-to-X adapters (Aardvark I2C/SPI, CAN, etc.). With those you could truly write a driver on your host machine and test it while talking to real hardware. When it works you can run it on the target hardware, and if your I2C driver on that chip is good, you have no extra work to do. Developers can finally test code without having the final boards in, and you get a much faster code-compile-test loop.

* make it easier to write multi-threaded software
* make it easier to write safer software - eliminate whole categories of common programming problems


## Sub-documents

* [Developing a Device Driver](driver.md)
* [Interrupt Guidelines](interrupt_guidelines.md)
* [Memory Tuning Guide](memory_tuning.md)

## Threads vs Dispatching Functions

Pure threads should be used when:

* You need to wait on data and then perform a COMPLEX SERIES of operations (e.g.,, not just one function in response)
* Streaming data
* Your thread is sleeping until a state change occurs, and then it is a primary actor running a lot of logic

Dispatch events and callbacks
Dispatch system events


* Dispatch Q as the core operating mode
    * threads also supported but not encouraged
    * e.g.,, no need for a thread - dispatch a callback to the queue

## Selecting Interrupt Priorities

* How fast does this need to be?
* What are the latency requirements?
* WHat is the ranking of operations that need to be performed if two interrupts trigger at once?

Mantra of priority: Keep the hardware happy

## Selecting Thread Priorities

* How close to the hardware is it?
* How fast does this need to be?
* What are the latency requirements?

Mantra of priority: Keep the hardware happy
Keep the data flowing

# Clean Up

* David Parnas proposed a decoupling strategy known as information hiding, originally described as follows:
    * We propose instead that one begins with a list of difficult design decisions or design decision which are likely to change. Each module is then designed to hide such a decision from the others.
* Decoupling
    * David Parnas proposed a decoupling strategy known as information hiding, originally described as follows:
        * We propose instead that one begins with a list of difficult design decisions or design decision which are likely to change. Each module is then designed to hide such a decision from the others.
        * EA Design giudelines
        * Principle!
        * Add to the architecture checklist
        * Add to glossary
    * Read: On the criteria to be used in decomposing systems into modules, Communications of the ACM, 1972, December, 1053-1058
    * Client modules should not be able to discover or corrupt the way a service module works
    * The information hiding strategy leads to three decoupling tactics:
        * Keep related parts together
        * Reduce potential coupling by using visibility options
        * Create a barrier to isolate or protect parts
        * EA design guide
        * Design principles
* Abstractions
    * An abstraction is a simplified or idealized representation of something. Program abstractions normally reflect entities and behaviors that exist in the problem domain, making the program easier to develop, understand, and maintain
    * Program design is a problem-solving venture. Most designs are based on the time-honored approach of dividing the program into subprograms (modules). Decomposition normally continues until each module is straightforward enough to understand and implement.
    * This modular appraoch improves productivity and flexibility. Specifically, modules:
        * Are easier to understand and modify
        * Let multiple developrs work concurrently
        * Support direct reuse rather than reimplementation
        * Can be arranged or combined as needed, improving flexibility
        * Help limit the effects of program errors
    * Modules usually consist of one or more abstractions
    * There are three kinds of abstractions: functional abstractions, data abstractions, and object-oriented abstractions
    * []Add these notes to desing guide
    *
Functional abstractions
        * Functional abstractions model behavior, and they are implemented as procedures
        * The purpose of a procedure is more important than its specific implementation
        * What we desire from an abstraction is a mechanism which permits the expression of relevant details and the suppression of irrelevant details. - Liskov and Zilles
            * Share on SQ
            * Programming with abstract data types. ACM SIGPLAN Notices. Read
        * Ieally, each procedure should:
            * Have a single, well-defined purpose
            * Have explicit inputs and outputs
            * Have verifiable run-time behavior
            * Be separable and potentially re-usale
                * Add these to design guide
                * Principles
                * Self-review checklist
                * Well, ode review c hecklist in general
        * Procedures are the only abstractions that contain executable statements.
            * They can also call other procedures, permitting a hierarchy of operations.
            * They also create or access instances to manipulate data.
            * Procedures are potent tools for decomposition and reuse
    * Data abstractions
        * Data abstractions model stateful entities, often known as "abstract data types" or "user-defined types."
        * A data abstraction is effectively a blueprint that defines a set of data members to be used as a unit. For example, i the C language, data abstractions are defined using the struct and union keywords.
        * A data abstraction should always have a well-defined and cohesive puprose, and should usually relate to an entitty in the problem domain
    * Also for design guide: threads, runloop, eent machine, actor-observer models, dispatch queue
        * What other common design patterns for embedded?
    * Object-oriented abstractions
        * A hybrid between data abstractions nad functional abstractions.
        * Model stateful entitites plus the behavior associated with them
        * Provide two major benefits:
            * by packaging data and procedures togehter, the abstraction is cohesive, complete, and understandable.
            * The visibility of its members can be set individually, providing detailed control over scope
        * OO abstraction is a blueprint for creating instances
        * Can define complex hierarchies of OO abstractions.
* Use black boxes
    * A black box is an abstraction that has a distinct boundary, inputs, and outputs, but whose implementation is hidden. Black box analysis has proven valuable in many scientific and engineering fields.
    * Principle, design guide
    * With OO languages, a class designed as a black box provides several benefits:
        * A larger amount of reusable functionality
        * Greater control over member visibility
        * Its implementation can be changed without affecting client code
    * Encapsulatin
        * Hallmark of OO development!
        * Definitions:
            * The process of hiding all of the details of an object that don't contribute to its essential characteristics; typically, the structure of the object is hidden, as well as the implementaiton of its methods -- Grady Booch
            * A modeling and implementaiton technique that separates the external aspects of an object from the internal, implementation details of the object (also called information hiding) - James Rumbaugh
            * The behavior and information are encapsulated int he object. The only way to affect the object is to perform operations on it. Objects thus support the concept of information hiding, that is, they hide their internal structure from their surroundings. - Ivar Jaconbson
            * The ability to preent certain aspects of a class from being accessible to its clients. -- Bertrand Meyer
        * PJ: Definitions, design guide, section on encapsulation. Reference Scott Meyers as well
        * Essays on Object Oriented Software Engineering, Vol 1.
        * Grady Booch: Object oriented design with applications
        * Bertrand Meyer: Object oriented software construction
        * Data hiding is just one kind of information hiding.
        * Though you've made data members cprivate, client code can still set the member values usin the public setter methods. Therefore, the calss is not encapsulated. No information is truly hidden. You've simply created a more complicated and less efficient way to get or set the data members.
            * PJ: great note. cleanup in my own code
            * Design guide point, review checklist, cleanup checklist
    * The focus of an abstraction is outward,defining services used by the client code. The focus of encapsulation is inward, hiding an abstractions implementation from client code. In other words, encapsulation is a form of Parnas' information hiding.
        * Smarter queue?
    * The public interface of a black box is more important than its implementation. In most cases, if its interface isn't changed, its implementation can be changed without affecting client code.
        * This is an important detail and selling point for the framework.
    * An API design is no big deal only if you ignore the costs and risks of change. Once clients start using an API, the costs and risks can be substantial, so it's best to avoid changes that affect client code.
        * Design guide / principle
    * An API designed with stability in mind will usually be better than one designed willy-nilly. It's easy to design an API without forethought, but frequent changes undermine the stability of black box design.
        * Smarterqueue
        * design guide / principle
## Prefer Cohesive abstractions
    * Cohesion refers to the logical unity of an abstraction - its singleness of purpose
        * Several kinds of cohesion have been identified, but functional cohesion (purpose) is widely considered the most important
    * Cohesiveness is the degree to which the  parts of an abstraction are related logically. Cohesiveness is an ordinal measure, so items are ranked from low to high. We prefer highly cohesive abstractions because they're sensible and stable.
    * PJ: above are defintions for glossary, and design guide
    * The ButtonList member implies that each button instance will contain a list of other Button instances, resulting in an overall tree strcutre. I can't understand why an application would have buttons within buttons, so the Button class is less cohesive than it could be
        * Good example
    * Simplify procedures
        * Review checklist, design guide, style guide
        * Small procedures are more cohesive and easier to understand. It's harder for bugs to hide in them, and they have a greater potential for reuse
        * Most of the time, large procedures should be decomposed into several smaller procedures.
        * Bob Martin has proposed a technique he calls "extract till you drop". The idea is to extract new procedures from existing ones recursively until it becomes impossible to continue. The process increases the number of procedures, but each one is much simplier.
            * Cleanup checklist idea
            * Bob Martin - one thing, extract till you drop. sites.google.com/site/unclebobconsultingllc/one-thing-extract-till-you-drop
        * In general, a procedure shouldn't be extracted unless it's functionally cohesive
            * Code cleanup node
            * Code review
        * Mixed metaphors
            * OO developers often ruin cohesion by defining classes that are based on mixed real-world metaphores
            * The most common mistakes hinge on difference between products, producers, and processors
            * A product is an abstraction whose instances are created for use by client code. Imagine a real-world manufacturer that creates bolts, radios, cars, or other consumer products. A client must obtain the product to use it.
            * A producer is an abstraction responsible for creating a product. It's equivalent to a manufactuerer or sipplier in the real world. A producer may create multiple products, but a prodcut never creates itself or destroys itself
            * A processor is an abstraction that manipulates a product in some way. A processor might repair the prouct, rearrange it, store it, or adjust it in various ways. A processor never creats a prodcut or vice versa. Likewise, a processor never creates a producer or vice versa.
            * Products, producers, and processors are used togehter, but each abstraction must be independent to be cohesive. Developers go wrong by mixing them in some way.
            * []Design guide, review checklist, architecture checklist?
            * In most cases, client code may act as a producer by creating products directly. At other times, client code may use a separate prodcuer abstraction to create products. For example, the Factory Method and Abstract Factory design patterns represent producers
            * Example:
                * Message class, containing serialization and deserialization procedures
                * In operation, a blank message was instantiated. Then its deserialization procedure was called, filling the message with values from the mainframe. Later, i the object had been modified, its serializatin procedure was called to send all the message values to the mainframe.
                * The design mixes all three metaphors. The message class is a prodcut, prodcuer, and processor.
                * Message class hould be only a product
                * A producer should be created: MessageDeserializer, should create a Message instance
                * A processor class, such as a messageSerializer, should send the message values to the mainframe.
                * These abstractions would be more cohesive and re-usable.
            * PJ: get rozi in on esign, share principles with her, then walk through designs.
        * Cohesive abstractions are important. They reduce excess coupling and make the program easier to understand.
        * The Single Responsibility Principle (SRP) states that a calss should have a signle responsibility (a single reason to change). The SRP provides another way to judge the cohesiveness of OO abstractions.
        * Good designs have highly cohesive abstractions with low coupling between them. For this reason, coupling and cohesion are normally viewed as contrasting concepts. Howver, the contrast is misleading. Conhesion is actually a sepcial case of coupling.
            * The members of a cohesive abstraction are related logically and syntactically. In other words, a cohesive abstraction is a region of tight, local coupling.
            * By encapsulating tightly coupled parts within each abstraction, the coupling between abstractions is automatically reduced.
            * The true cohesiveness of an abstraction depends on the problem's context, as well as subjective judgement. However, an abstraction may *not* be cohesive if:
                * It's alrge. Cohesiveness normally decreases as size increases.
                * Its name includes the words "And" or "Or"
                * Its name describes its algorithm or structure instead of its purpose
            * Not every design needs to use a product, producer, or processor abstractions. Even so, abstractions should always be designed carefully to avoid mixed metaphors and overlapping functionality.
