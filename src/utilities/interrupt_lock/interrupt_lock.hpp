// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#ifndef INTERRUPT_LOCK_HPP_
#define INTERRUPT_LOCK_HPP_

#include <atomic>
#include <cassert>
#include <condition_variable>
#include <etl/function.h>
#include <etl/queue.h>
#include <etl/vector.h>
#include <inplace_function/inplace_function.hpp>
#include <mutex>
#include <string>
#include <thread>

namespace embutil
{
// TODO: Document
/*
 * @tparam TReturn The return type for the TDisableOp function. The return value of TDisableOp
 * 	should report the current interrupt config. This value is passed to TEnableOp to restore the
 * 	prior interrupt config.
 * @tparam TDisableOp The type (std::function or stdext::inplace_function) and prototype of the
 *	function which is used to disable interrupts.
 * @tparam TEnableOp The type (std::function or stdext::inplace_function) and prototype of the
 *	function which is used to enable interrupts.
 */
struct InterruptLockPolicyExample
{
	using TReturn = bool;

	static void enable_interrupts(bool enable) noexcept
	{
		(void)enable;
	}

	/// Disable interrupts and return the current setting
	static auto disable_interrupts() noexcept -> TReturn
	{
		return false;
	}
};

// TODO: update documentation... this is wrong
// TODO: create a concept for InterruptLockPolicy (Typename TReturn, enable_interrupts static func,
// disable_interrupts static func)
/** BasicLockable class which disables/enables interrupts
 *
 * This class defines a BasicLockable interface for enabling and disabling interrupts.
 * The lock() function is used to disable interrupts, and unlock() to enable them.
 *
 * @note This lock is not re-entrant.
 * @ingroup FrameworkUtils
 */
template<typename InterruptLockPolicy>
class InterruptLock
{
  public:
	/// Default constructor
	InterruptLock() = default;

	/// Default destructor
	~InterruptLock() = default;

	/// Deleted copy constructor
	InterruptLock(const InterruptLock&) = delete;

	/// Deleted copy assignment operator
	auto operator=(const InterruptLock&) -> const InterruptLock& = delete;

	/// Deleted move constructor
	InterruptLock(InterruptLock&&) noexcept = default;

	/// Deleted move assignment operator
	auto operator=(InterruptLock&&) noexcept -> InterruptLock& = default;

	/* Disable interrupts to enter critical section
	 *
	 * This function is marked noexcept because we want the program to terminate if an exception
	 * results from this call.
	 */
	void lock() noexcept
	{
		irq_status_ = InterruptLockPolicy::disable_interrupts();
	}

	/** Enable interrupts to leave critical section
	 *
	 * This function is marked noexcept because we want the program to terminate if an exception
	 * results from this call.
	 */
	void unlock() noexcept
	{
		InterruptLockPolicy::enable_interrupts(irq_status_);
	}

  private:
	/// Local storage for the disable function result, which is passed into the enable function
	/// to ensure we restore the prior value.
	typename InterruptLockPolicy::TReturn irq_status_;
};

} // namespace embutil

#endif // INTERRUPT_LOCK_HPP_
