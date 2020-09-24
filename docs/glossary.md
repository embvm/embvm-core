# Project Glossary

[0-9](#0-9) | [A](#a) | [B](#b) | [C](#c) | [D](#d) | [E](#e) | [F](#f) | [G](#g) | [H](#h) | [I](#i) | [J](#j) | [K](#k) | [L](#l) | [M](#m) | [N](#n) | [O](#o) | [P](#p) | [Q](#q) | [R](#r) | [S](#s) | [T](#t) | [U](#u) | [V](#v) | [W](#w) | [X](#x) | [Y](#y) | [Z](#z)

# 0-9

# A

* **Abstract class:** a class that cannot be directly instantiated
* **Abstract data type:** a data type defined for a specific domain or application
* **Abstract interface:** a set of public procedures that are defined for an abstraction but are not implemented
* **Abstraction:** a simplified or idealized representation of a module or system
* **ADR:** Architecture decision record
* **Assertion:** a special conditional expression used to verify program values or states at points of execution

# B

* **Black box:** an abstraction with defined inputs and outputs but an unknown implementation
* **Brittle (program)** a program that is difficult to change because it abstractions are too tightly coupled

# C

* **Class:** an abstraction consisting of procedures and data members
* **Cohesion:** a property of an abstraction that represents singleness of purpose
* **Cohesiveness:** the degree of cohesion exhibited by an abstraction
* **Component:**
	* Driver/Electrical context: a standalone electrical part, typically an IC
	* Software context: A standalone software executable which can run independently on a node (e.g., a program)
		* a physically distinct module
* **Concrete class:** a class that can be directly instantiated
* **Concurrency:** multiple procedures that execute, or appear to execute, in parallel with one another
* **Context:** a known environment or state that establishes the conditions in which one or more procedures execute
* **Coupling:** a connection between program elements

# D

* **Data member:** a data element belonging to a type
* **DbC:** Design by contract
* **DDD:** Domain driven design
* **Decomposition:** the process of dividing a program into sub-programs
* **Domain:** a specialized area of study, interest, or work (e.g., chemistry)
* **Domain expert:** a person having deep knowledge about a specific domain

# E

* **Efficiency:** the extent to which a program minimizes the computing resources it uses
* **Encapsulation:** the process of hiding the internal representation of an abstraction

# F

* **Flexibility:** the ease with which a program can be modified or reconfigured
* **Framework:**
	* A set of cooperating classes that make up a reusable design pattern for a specific class of software
	* A specific arrangement of abstractions that can be used, customized, or extended to solve a specific problem
	* You customize a framework to a particular application by creating application-specific subclasses of abstract classes from the framework
	* The framework captures the design decisions that are common to its application domain
	* An application designer/implementer can concentrate on the specifics of the application rather than plumbing
	* Frameworks emphasize design reuse over code reuse, though a framework will usually include concrete subclasses you can put to work immediately

# G

* **Generalization:** analyzing the similarities between specialized but related abstractions to devise a general-purpose abstraction
* **Getter:**  a method that returns the value of a data member
* **GPIO:** General purpose input/output

# H

* **HAL:** Hardware Abstraction Layer

# I

* **Immutable:** an instance whose state cannot be changed after creation
* **Information hiding:**  a strategy for program decomposition in which key design information is intentionally hidden to increase flexibility
* **Inheritance:** the ability of a programming language to allow one type to be defined as an extension of another type
* **Instance:** a distinct entity stored in computer memory during program execution whose representation is based on a type definition
* **Instantiation:** the creation of an instance
* **Interface:** the set of public procedures implemented by an abstraction
	* An interface is a boundary across which two elements meet and interact or communicate with each other

# J

# K

* **KV:** Key-value

# L

* **Lifetime:** the time during which an identifier is bound to a memory location during program execution

# M

* **Method:** a procedure that belongs to a class or structure
* **Module:** a distinct set of program instructions and definitions
	*  A module is an implementation unit of software that provides a coherent set of responsibilities
		* A responsibility is a general statement about an architecture element and what it is expected to contribute to the architecture
		* This includes:
			* the actions that it performs
			* the knowledge it maintains
			* the decisions it makes
			* the role it plays in achieving the system’s overall quality attributes or functionality
* **Monolithic:** a program or module that has not been decomposed into sub-modules
* **Mutable:** an instance whose state can be changed after initialization
	* The opposite of immutable

# N

* **NFC:** Near-field communication

# O

* **Optimization:** actions taken to improve the responsiveness, efficiency, or performance of a module

# P

* **Parameter:** a variable defined for a procedure, whose value is supplied by the caller
* **Performance:** the actual or apparent speed of an operation
* **Persistence:** the ability to store the state of program instances while the program is inactive
* **Polymorphism:** the ability of a derived class to define a method that supersedes a method defined in the base class
* **Postcondition:** a condition that a procedure should satisfy just prior to its completion
* **Precondition:** a condition that should be satisfied just before a procedure is executed
* **Private:** visibility designation indicating that an element is not visible outside of the abstraction in which it is defined
* **Procedure:** general term for a sequence of computer instructions that provides a specific service and that has a distinct boundary and identify
	* May also be known as a routine, subroutine, function, operation, or method
* **Project glossary:** a glossary of domain-specific project terms
* **Protected:** visibility designation indicating that an element is visible to the abstraction in which it is defined and any abstraction derived via inheritance
* **Public:** visibility designation indicating an element is visible inside and outside of the abstraction in which it is defined

# Q

# R

* **RAII:** Resource acquisition is initialization
* **Real-time:** responsive data processing
* **Realization:** the act of implementing an abstract interface
* **Recursive:** a sequence of instructions that execute themselves again in a different context
	* For instance, a procedure is recursive if it can call itself during execution
* **Reentrant:** program code that permits simultaneous, interleaved, or nested invocations that are independent of one another
* **Refactor:** an action to improve the structure of an existing program without altering its public behavior
* **Robust:** the ability of a program or module to behave reasonably during unusual or unexpected conditions

# S

* **SBRM (Scope-bound resource management):** Managing a resource based on the scope it's contained within
	* When the object goes out of scope, the resource is automatically freed or released
* **Scope:** the region of source code in which a specific identifier (name) can be used
	* Visibility determines scope
* **SDK**: Software development kit
* **Setter:** a method that sets the value of a data member
* **Sponsor:** a person or organization that authorizes the development of a product
* **Stakeholder:** any person or or invested in the success of a program/product/business
	* A stakeholder can be a user, a purchaser, or an internal team member
* **State:** the collective data values of an instance, module, or program at a specific time
* **Style (Architecture):** An architecture style is a specialization of element and relation types, together with a set of constraints on how they can be used.

# T

* **Technical debt:** a future cost incurred by using inadequate short-term planning or activities
* **Type:** a classification of data that denotes specific storage and behavioral characteristics

# U

* **Uses:** Uses is a form of dependency that can exist between two modules
	* A uses B if the correctness of A depends on the presence of a correct implementation of B

# V

* **View (Architecture):** a representation of a set of system elements and the relationships associated with them
* **Virtual Machine:** A virtual machine is a collection of modules that form an isolated, cohesive set of services that can execute programs
	* It’s sometimes called an abstract machine
	* Early use of the term referred to a more abstract stand-in for a real computer, but current use includes virtual machines that have no direct correspondence to any real machine
	* Interpreters are good examples of virtual machines
	* An operating system itself is a virtual machine that allows the execution of native code on the underlying hardware
	* Thus, a virtual machine is a software layer that can execute “programs,” which can be sequences of calls to facilities of the virtual machine’s interface
		* Hence some authors regard layers and virtual machines as synonyms.
* **Visibility:** an implicit or explicit designation that governs the scope of an identifier
	* Controls who can access a data, when, and for how long

# W

# X

# Y

# Z
