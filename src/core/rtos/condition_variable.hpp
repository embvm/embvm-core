// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#ifndef VIRTUAL_CONDITION_VARIABLE_HPP_
#define VIRTUAL_CONDITION_VARIABLE_HPP_

#include "mutex.hpp"
#include "rtos_defs.hpp"
#include <chrono>
#include <cstdint>
#include <inplace_function/inplace_function.hpp>
#include <time/time.hpp>

// TODO: how can we enforce that the mutex is locked?

namespace embvm
{
/// @addtogroup FrameworkOS
/// @{

namespace cv
{
using func_t = stdext::inplace_function<bool(void)>;
}

/** Defines the VirtualConditionVariable interface.
 *
 * VirtualConditionVariable defines the common condition variable interfaces for a VirtualRTOS.
 * RTOS implementations should derive from this base class.
 *
 * The factory is responsible for creating the cv, so the constructor is not defined
 * in this interface class.
 */
class VirtualConditionVariable
{
  public:
	/// Delete the copy constructor
	VirtualConditionVariable(const VirtualConditionVariable&) = delete;

	/// Delete the copy assignment operator
	auto operator=(const VirtualConditionVariable&) -> const VirtualConditionVariable& = delete;

	/// Default move constructor
	VirtualConditionVariable(VirtualConditionVariable&&) = default;

	/// Default move operator
	auto operator=(VirtualConditionVariable&&) -> VirtualConditionVariable& = default;

	/// Blocks the current thread until the condition variable is woken up or after the specified
	/// timeout duration
	// Precondition: mutex is locked
	virtual auto wait(embvm::VirtualMutex* mutex) noexcept -> bool = 0;
	// Precondition: mutex is locked
	virtual auto wait(embvm::VirtualMutex* mutex, const embvm::os_timeout_t& timeout) noexcept
		-> bool = 0;

	// TODO: template these like in the STL
	// Precondition: mutex is locked
	auto wait(embvm::VirtualMutex* mutex, const embvm::cv::func_t& predicate) noexcept
		-> bool
	{
		while(!predicate())
		{
			wait(mutex);
		}

		return true;
	}

	// TODO: template these for predicate like in the STL?
	// Precondition: mutex is locked
	auto wait(embvm::VirtualMutex* mutex, const embvm::cv::func_t& predicate,
					 const embvm::os_timeout_t& timeout = embvm::OS_WAIT_FOREVER) noexcept -> bool
	{
		bool success = true;

		/// We break when the predicate is true or when we've timed out
		while(!predicate() && success)
		{
			success = wait(mutex, timeout);
		}

		return success;
	}

	/// notifies one waiting thread
	virtual void signal() noexcept = 0;

	/// Alias for signal so we can work with STL types
	void notify_one() noexcept
	{
		signal();
	}

	/// Notify all waiting threads
	virtual void broadcast() noexcept = 0;

	/// Alias for broadcast so we can work with STL types
	void notify_all() noexcept
	{
		broadcast();
	}

	[[nodiscard]] virtual auto native_handle() const noexcept -> embvm::cv::handle_t = 0;

  protected:
	VirtualConditionVariable() = default;

	/// Derived classes should ensure their destructor deletes the OS CV
	virtual ~VirtualConditionVariable() noexcept;
};

/// @}
// End Group

} // namespace embvm

#endif // VIRTUAL_CONDITION_VARIABLE_HPP_
