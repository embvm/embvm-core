# CRC-R: Register

**Status:** Completed, needs improvement

**Table of Contents:**

1. [Responsibilities](#responsibilities)
2. [Requirements](#requirements)
3. [Collaborators](#collaborators)
4. [Rationale](#rationale)
5. [Source Links](#source-links)
6. [Related Documents](#related-documents)
7. [Notes](#notes)

## Responsibilities

* Abstract raw register access behind classes/structures
* Detect invalid settings and erroneous settings at compile-time

## Requirements

TBD

## Collaborators

* A [Virtual Processor](virtual_processor.md) contains registers
* [Drivers](driver.md) (including [HAL Drivers](hal_driver.md)) may utilize a register model

## Rationale

Embedded developers frequently interact with memory-mapped peripheral devices through Special Function Registers (SFRs). Incorrect use of SFRs leads to bugs which are difficult to detect.

Rather than work directly with registers, the code should be easily be easlily understandable. Errors should be detectable at compile-time.

## Source Links

* [Register Folder](../../../../src/utilities/register)
* Register
	* [register_tests.cpp](../../../../src/utilities/register/register_tests.cpp)
	* [register.hpp](../../../../src/utilities/register/register.hpp)

## Related Documents

N/A

## Notes

Potential for adopting Kvasir's register code after Odin rewrites it (starting December 2018).

* Research KVasir Register: https://github.com/kvasir-io/Kvasir/tree/master/Lib/Register

Need to meet 100ms, but calling all constructors on boot meant they couldn't do that!
Lifecycle concept - functionality is grouped into so-called systems: CanSystem, EthernetSystem…
Systems are organized into run-levels (like unix. Run level 0 .. 5)
Systems depend on each other
Systems run through several states
Boot -> Init -> Run -> Shutdown -> Init… etc.
Can start up the system and get the most important things running in 100ms or less (by using run levels)
Run levels also support putting unnecessary things to sleep - tell RL5 to go to sleep, etc. When you wake up, then you need to initialize back up the stack to the target run level (instead of the full stack)
Mostly he doesn't have destructors in his code - they use up ROM, and mostly peple are just turning the car off - how can I leverage a similar thing when needed?
Low-level APIs
- APIs should be easy to use but hard to misuse
- Foundational aspect of embeded systems
//write registers
bool write8(uint8-t page, uint8_t addr, uint8_t v)
Approaching the challenge: normal flow is to
read the user manual
find the register section
describe the bits in the fields (tend to have meaningful names)
How can C++ help us turn that into a type-safe API?
Register description:
[Insert Image]
unions can't have base classes… why?
unions can have constructors?
use to make sure there isn't uninitialized memory
Templates can help us! Look at "Template Dispatch"
- keep your raw read/write functions private - unsafe, don't want developers using them
Then you can make generic write/read functions:
template<class Reg>
bool write(const Reg& reg);
Refer back to slides for final register description
https://meetingcpp.com/index.php/tv15/items/5.html
Information:
need to know the location, size of register, type of the raw value, size and underlying data to be sent/received
Need a struct for each register… or each size?
base case:
template<size_t RegSize>
struct RegisterDispatch;
Need to implement 1 byte, 2 byte, 4 byte cases then.
Compiler can inline dispatching - no overhead!
Then you can say:
```
Control r;
device.read(r);
```
But he uses bitfields which are implementationn defined… so bad.
So he created a portable bitfield solution using templates
specify starting bits and number of bits
has an assignment operator to handle shifting/manipulation
Conversion operator to get bits out of it.
DUDE THIS BITFIELD REGISTER SOLUTION IS FUCKING RAD
templates are required to write the driver but not to use it
No STL in automotive
- no dynamic memory, not even a heap
exceptions are mostly prohibited
different implementations
"embedded STL"
- no dynamic memory allocation
- Doesn't require exceptions
- Pre-defined container sizes, defined at compile-time
We know how many network connections we can actually support, how many messages we can support at any given time, etc. Since we know these things, we can pre-determine sizes and will alwyas know how much memory we need.
ESTL iterators are compatible with STL
Keep APIs almost identical
Remove some methods
Embedded applications define limits in code
#define MAX_CONNS 5
Connection conns[MAX_CONNS];
How to make work with templates? add a size parameter to the template
review his notes on container improvements - I got lost. templates + inheritence to make a nice interface
we have one derived class for each size of vector, but underlying class is still the same
Usage looks identical to STL usage
Forbidden APIs:
- Any API that can cause underlying data to grow
push_back on a full container
resize too big
insert too many elements
They utilized a pluggable error handler API
- can be changed at compile or runtime
- Can use exceptions in testing
- ECU rollback or reset in production
Most undefined behavior will call its error handler
e.g., front() on an empty vector
Embedded C++ Working group - research
- defining how C++ works in embedded systems
requires modenr C++ compilers
how can I get involved?
# Embedded Framework Register Model
* Foundational Principle: APIs should be easy to use but hard to misuse
* Approaching the challenge - normal flow is to
 	1. read the user manual
 	2. find the register section
 	3. describe the bits in the fields (tend to have meaningful names)
## Register Description
Information:
need to know the location, size of register, type of the raw value, size and underlying data to be sent/received
### No Dispatching
One type of approach (no dispatching):
```
struct Registers {
 	union Control {
 		enum {
 			PAGE = 0x16, ADDRESS = 0x17
 		};
 		struct OPM {
 			enum { NO_CHANGE=0, AS_STANDBY=1 };
 		};
 		struct Bits {
 			uint16_t OPM :2;
 			uint16_t CTRL_BR1: 2;
 			uint16_t CTRL_BR2: 2;
 			//…
 		} bits;
 		uint16_t raw;
 		// union contstructor
 		Control(uint16_t v = 0x00) : raw(v) {}
 	};
};
```
### Template Dispatching
* Requires a well-defined set of properties (e.g., register size!)
* Create a struct for each size of register (1 byte, 2 bytes, 4 bytes)
* Device class uses the API of each register to dispatch to the size-specific methods
* Use generic read/write functions (`template<class Reg> bool write(const Reg& reg)`)
* Compiler can inline dispatching - no overhead!
* how can you encode things like "Clear on read" into the register definition?
 	* You could make a read only register class, doesn't have the write operations.
* templates are required to write the driver but not to use it
* On Bitfields
 	* So he created a portable bitfield solution using templates
 	* specify starting bits and number of bits
 	* has an assignment operator to handle shifting/manipulation
 	* Conversion operator to get bits out of it.
#### Portable Bitfields
Specify starting bit and number of bits
```
template<class T, uint8_t START, uint8_t SIZE>
struct Bits {
 	Bits& operator=(T v) { … }
 	operator T() const { … }
private:
 	T _raw;
}
```
Code relates to spec:
```
union Control {
 	//bit 11:10 in spec
 	struct OPM {
 		typedef Bits<uint16_t, 10, 2> Bits;
 		enum { NO_CHANGE = 0, AS_STANDBY = 1};
 	};
 	// just a value definition here
 	struct CTRL_BR {
 		enum { NO_CHANGE=0, BRANCH_NORMAL=1 };
 	};
 	// bits 9:8
 	struct CTRL_BR1 : CTRL_BR {
 		typedef Bits<uint16_t, 8, 2> Bits;
 	};
 	// bits 7:6
 	struct CTRL_BR2 : CTRL_BR {
 		typedef Bits<uint16_t, 6, 2> Bits;
 	};
 	// …
 	union Bits {
 		Control::OPM::Bits OPM;
 		Control::CTRL_BR1::Bits CTRL_BR1;
 		Control::CTRL_BR2::Bits CTRL_BR2;
 	} bits;
 	uint16_t raw;
}
```
#### Example
```
class Device {
public:
 	//generic write function
 	template<class Reg>
 	bool write(const Reg& reg);
private:
 	bool write8(uint8_t p, uint8_t a, uint8_t v);
 	bool read8(uint8_t p, uint8_t a, uint8_t& v);
};
// New register description
union Control {
 	//bit 11:10 in spec
 	struct OPM {
 		typedef Bits<uint16_t, 10, 2> Bits;
 		enum { NO_CHANGE = 0, AS_STANDBY = 1};
 	};
 	// just a value definition here
 	struct CTRL_BR {
 		enum { NO_CHANGE=0, BRANCH_NORMAL=1 };
 	};
 	// bits 9:8
 	struct CTRL_BR1 : CTRL_BR {
 		typedef Bits<uint16_t, 8, 2> Bits;
 	};
 	// bits 7:6
 	struct CTRL_BR2 : CTRL_BR {
 		typedef Bits<uint16_t, 6, 2> Bits;
 	};
 	// …
 	union Bits {
 		Control::OPM::Bits OPM;
 		Control::CTRL_BR1::Bits CTRL_BR1;
 		Control::CTRL_BR2::Bits CTRL_BR2;
 	} bits;
 	uint16_t raw;
}
// Need a struct for each size of register
template<size_t RegSize>
struct RegisterDispatch;
template<> struct RegisterDispatch<1>
{
 	static
 	bool write(Device& d, uint8_t p, uint8_t a, uint8_t v)
 	{
 		return d.write8(p, a, v);
 	}
};
template<> struct RegisterDispatch<2>
{
 	static
 	bool write(Device& d, uint8_t p, uint8_t a, uint16_t v)
 	{
 		return d.write16(p, a, v);
 	}
};
// Use the API of each register to dispatch to the size-specific methods
template<class Reg> inline
bool Device::write(const Reg& r)
{
 	return RegisterDispatch<Reg::LENGTH>::write(*this, Reg::PAGE, Reg::ADDRESS, r.raw);
}
```
#### Example Usage
```
// Create a register
Control r;
device.read(r); // No template syntax here!
if(r.bits.OPM == Control::OPM::AS_STANDBY)
{
 	//…
}
```
## References
* Deeply Embedded C++
 	* [Notes](https://www.evernote.com/shard/s1/nl/19041/bda993fa-2528-4489-86b8-9515e55a86a9/)
 	* [Slides](https://meetingcpp.com/tl_files/mcpp/2015/talks/JohnHinke%20-%20Deeply%20embedded%20c++%20-%20Meeting%20C++%202015.pdf)
 	* [Presentation](https://www.youtube.com/watch?v=TYqbgvHfxjM)
how can you encode things like "Clear on read" into the register definition?
You could make a read only register class, doesn't have the write register.
https://meetingcpp.com/tl_files/mcpp/2015/talks/JohnHinke%20-%20Deeply%20embedded%20c++%20-%20Meeting%20C++%202015.pdf
https://www.youtube.com/watch?v=TYqbgvHfxjM
