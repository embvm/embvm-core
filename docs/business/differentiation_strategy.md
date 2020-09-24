# Differentiation Strategy

Focus: EA Embedded Framework

## How We Will Compete

* We will enable faster bring-up of new hardware designs, providing fully-complete drivers and chip support
* We will provide on-going customer support
	* Many other free frameworks and vendor SDKs do not come with *any* support when things go wrong
* We will offer services to write new drivers & tune the system after initial delivery
* We will ensure that applications are as portable as possible, rather than tying your program directly to the hardware and OS
* We will have a library of parts that are already supported, enabling faster development, reduced bugs, and improved re-use
* We will provide a fully-customized solution for our clients
	* Bring-up board and drivers
	* Make initial connections
	* Perform initial configuration requirements
	* Optionally Set up with build server (i.e., you can get a Jenkins server setup that works with the framework)
	* Enable customers to start using their system on day one, rather than figuring out how to get their board supported
* We will create and provide documentation which explains *how* to use the system and *why* it was designed this way
	* We will include examples and tutorials that will help introduce developers to new concepts
		* Tie our examples/tutorials with the use cases we identify during architectural stage
	* We will create short videos explaining our system and make them available on youtube (private, send links? public?)

* Provide support for standard C and C++ library functions (improved over newlib, focus on bare metal)
* We will provide add-on modules for complex design areas, such as OTA support, two-stage boot, backup firmware images, lwIP, command line, KV-store, product configuration, etc.

## Strategic Advantages of Our Architecture

We will differentiate ourselves from other frameworks and vendor SDKs by:

* We will make key architectural decisions for our clients:
	* to prevent common bugs and sources of errors
	* to prevent inefficient designs
	* to enable portability across operating systems and architectures
* We will have a library of interfaces that have been re-used across clients
* Enabling simulation and testing on a host PC using USB adapters and mock objects
* Providing high-value add-ons for common & error-prone features on embedded systems


We can adopt the following external standards and technologies to reduce our workload:

* Interfaces/Libraries:
	* libc
	* libcpp
	* foonathan/type_safe
	* SaferC++
* Documentation:
	* Doxygen
	* PlantUML
	* Markdown formatting
* Testing frameworks:
	* Doctest
	* GMock
	* metal.test
* Source analysis:
	* CPPCheck
	* Clang Analyzer
	* Clang Tidy
	* Clang Sanitizer

Under consideration for adoption:

* MCAPI
* MTAPI
* MRAPI

## Where Will We Focus Effort and Investment?

* We will focus our effort on initial architecture
	* Keep the design simple so others can easily use it
	* Keep the design as flexible as possible to support a variety of development styles
	* Ensure that our interfaces are solid so we can provide reliable frameworks to our clients
* We will focus our effort on documentation and training materials
	* Give others the tools they need to understand the system
	* Reduce our support burden through our documentation
* We will focus on bare metal systems, not Embedded Linux or Android systems
* We will focus on the most popular chipsets currently used:
	* Nordic NRF
	* STM32
	* EFM32
* We will focus on expanding our driver library with common parts

We do not need to spend time:

* Focusing on what libraries need to exist for embedded systems
	* Our clients can pick their own libraries
* Tuning our system for complex boards, such as those using Embedded Linux
	* Focus on Bare Metal

