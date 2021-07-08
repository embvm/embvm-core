// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#ifndef VIRTUAL_RTOS_DEFINITIONS_HPP_
#define VIRTUAL_RTOS_DEFINITIONS_HPP_

#include <chrono>
#include <cstdint>

namespace embvm
{
// We forward declare these classes for libcpp
class VirtualMutex;
class VirtualConditionVariable;
class VirtualThread;

/// @addtogroup FrameworkOS
/// @{

/// Thread Local Storage types
namespace tls
{
/// Generic storage for Thread Local Storage handles
using handle_t = uintptr_t;
} // namespace tls

namespace once
{
/// Generic storage for Thread Once handles
using handle_t = uint32_t;
} // namespace once

#pragma mark - Condition Variable -

/// Timeout type used for OS-related functions
using os_timeout_t = std::chrono::duration<uint64_t, std::nano>;
/// Create a WAIT_FOREVER alias which can be used
constexpr os_timeout_t OS_WAIT_FOREVER = os_timeout_t{UINT64_C(-1)};

/// Condition Variable types and definitions
namespace cv
{
/// Generic storage for CV handles
using handle_t = uintptr_t;

} // namespace cv

#pragma mark - Event Flag -

/// Event flag types and definitions
namespace eventflag
{
/// Generic storage for Event Group handles
using handle_t = uintptr_t;

/// Underlying storage type for the event flag group
using flag_t = uint32_t;

/**  Option specification for VirtualEventFlag get().
 *
 * embvm::eventflag::option::OR will cause the get() operation to return once any of the bits
 * specified in bits_wait are set in the event flag group.
 *
 * embvm::eventflag::option::AND will cause the get() operation to return only once ALL of the bits
 * specified in bits_wait are set in the event flag group.
 */
enum class option : uint8_t
{
	/// Wait until all requested bits are set
	AND = 0,
	/// Wait until any of the requested bits are set
	OR = 1
};
} // namespace eventflag

#pragma mark - Mutex -

/// Mutex types and definitions
namespace mutex
{
/// Generic type for mutex handles
using handle_t = uintptr_t;

/** Mutex types
 *
 * Mutexes come in multiple flavors. The two variants that we utilize in the framework
 * are "normal" mutexes and recursive mutexes.
 *
 * Normal mutexes can only be locked once, even by the same thread. There is potential
 * for a deadlock to occur if a thread holding a lock tries to lock again.
 *
 * Recursive mutexes solve this deadlocking potential by allowing a mutex to be locked
 * multiple times by the same thread. To relinquish a recursive mutex, it must be unlocked
 * the same number of times that it was locked.
 */
enum class type : std::uint8_t
{
	/// A normal mutex can only be locked once, even by the same thread.
	normal = 0,
	/// A recursive mutex can be locked multiple times by the same thread,
	/// but it must be unlocked the same number of times.
	recursive,
	/// The default operational type for the Virtual RTOS mutexes is recursive.
	defaultType = recursive,
};

/** Mutex operational modes.
 *
 * Mutexes can operate in various modes which affect thread scheduling. The Virtual RTOS utilizes
 *	three mode types:
 *		- none: scheduling is not impacted by a thread owning the mutex
 *		- priority inheritance: owning the mutex elevates the owning thread to the highest priority
 *			of all threads waiting on the mutex
 *		- protect: A thread owning a mutex will execute at the highest priority among all mutexes
 *	owned by a thread
 *
 * The default operational mode is priority inheritance. Note that not all RTOSes may support the
 * full range of modes. The program should fail to compile or run if an invalid mode is used.
 *
 * Survey of OSes:
 * - ThreadX uses "Inherit/No Inherit" mode
 * - Pthread uses "protect, inherit, none" mode
 */
enum class mode : std::uint8_t
{
	none = 0, /// scheduling not impacted by owning the mutex
	priorityInherit, /// owning the mutex elevates you to the highest priority of all threads
					 /// waiting on mutex
	protect, /// execute at highest priority among all mutexes owned by a thread
	defaultMode =
		priorityInherit, /// The default mode for Virtual RTOS mutexes is priority inheritance.
};
} // namespace mutex

#pragma mark - Semaphore -

/// Semaphore types and definitions
namespace semaphore
{
/// Type used to store the semaphore count.
using count_t = int32_t;

/// Generic type for semaphore handles
using handle_t = uintptr_t;

/** Semaphore mode options.
 *
 * Semaphores are typically operated in one of two manners:
 *	- binary mode, which has a ceiling of 1
 *	- counting mode, which has a ceiling specified during semaphore creation
 */
enum class mode
{
	/// Use the default mode specified by the RTOS.
	defaultMode = 0,
	/// Binary mode, ceiling is 1
	binary,
	/// Counting mode, ceiling is > 1
	counting,
};

} // namespace semaphore

#pragma mark - Thread -

/// Thread types and definitions
namespace thread
{
/// Generic type for thread handles
using handle_t = uintptr_t;

/** Thread function's input data type.
 *
 * Type of input argument sent to the thread function on start.
 *
 * Survey results:
 *	 threadx takes in a `ULONG`
 * 	 FreeRTOS, pthread use `void*`
 *	 CMSIS settled on `void*`
 *
 * We default to `void*`
 */
using input_t = void*;

/** Thread function prototype.
 *
 * The function prototype for functions which can be passed to the thread object.
 *
 * Survey results:
 *	 pthread: `void *(*start_routine)(void*)`
 *	 freertos: `void (*task)(void)`
 *	 threadx: `void (*task)(ulong)`
 *    CMSIS: `void*(*task)(void*)`
 */
using func_t = void (*)(input_t);

/// Thread's operational state
enum class state : uint8_t
{
	/// Thread is created, but not yet running
	ready = 0,
	/// Thread is running
	executing,
	/// The thread's execution is suspended or blocked
	suspended,
	/// The thread has been terminated (did not complete on its own)
	terminated,
	/// The thread completed operation on its own
	completed
};

/** Default priority settings
 *
 * These are default priority settings, allowing consumers to specify thread priority by a portable
 * named value, rather than explicit numeric values. The named priority values are set to specific
 * values within the RTOS-specific thread implementation.
 *
 * This enumeration should be preferred to raw numeric values because the priority scheme changes
 * from RTOS to RTOS. Code specifing exact priority numbers is not portable. However, if more
 * precise priority specification is needed, do not use these types.
 */
enum class priority : uint8_t
{
	/// Panic handler should take priority over the system's operation.
	panic = 0,
	/// Interrupt handling priority.
	interrupt,
	/// Enable real-time priority, or highest system priority if real-time is not supported.
	realtime,
	/// Highest non-real-time priority.
	veryHigh,
	high,
	aboveNormal,
	/// Standard operational priority.
	normal,
	belowNormal,
	low,
	/// Lowest operational priority.
	lowest,
	/// Processor is sleeping.
	idle
};

} // namespace thread

#pragma mark - Message Queue -

/// Message queue types and definitions
namespace msgqueue
{
/// Generic storage for Message Queue handles
using handle_t = uintptr_t;
} // namespace msgqueue

/// @}
// end group Framework OS

} // namespace embvm

#endif // VIRTUAL_RTOS_DEFINITIONS_HPP_
