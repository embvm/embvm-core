// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#ifndef MUTEX_HPP_
#define MUTEX_HPP_

#include "rtos_defs.hpp"
#include <string_view>

namespace embvm
{
/// @addtogroup FrameworkOS
/// @{

/** Defines the VirtualMutex interface.
 *
 * VirtualMutex defines the common mutex interfaces for a VirtualRTOS. RTOS implementations
 * should derive from this base class.
 *
 * The factory is responsible for creating the mutex, so the constructor is not defined
 * in this interface class.
 */
class VirtualMutex
{
  public:
	/// Deleted copy constructor
	VirtualMutex(const VirtualMutex&) = delete;

	/// Deleted copy assignment operator
	auto operator=(const VirtualMutex&) -> const VirtualMutex& = delete;

	/// Default move constructor
	VirtualMutex(VirtualMutex&&) = default;

	/// Default move assignment operator
	auto operator=(VirtualMutex&&) -> VirtualMutex& = default;

	/** Lock the mutex.
	 *
	 * Attempts to lock the mutex. If successful, the function returns with the calling thread
	 * owning the lock. If the lock is already owned, the calling thread will block until the
	 * lock is available.
	 *
	 * @post The mutex is locked and owned by the calling thread.
	 */
	virtual void lock() noexcept = 0;

	/** Unlock the mutex.
	 *
	 * @pre The mutex is locked and owned by the calling thread.
	 * @post The mutex is unlocked.
	 */
	virtual void unlock() noexcept = 0;

	/** Try to lock the mutex.
	 *
	 * Try to lock the mutex, but don't block the thread if the mutex is already owned.
	 *
	 * @post If the mutex is unowned, the mutex will be locked and owned by the calling thread.
	 * @returns true if the mutex is successfully locked, false if another thread owns the mutex.
	 */
	virtual auto trylock() noexcept -> bool = 0;

	[[nodiscard]] virtual auto native_handle() const noexcept -> embvm::mutex::handle_t = 0;

  protected:
	VirtualMutex() = default;

	/// Derived classes should ensure their destructor deletes the OS mutex
	virtual ~VirtualMutex() noexcept;
};

/// @}
// End Group

} // namespace embvm

#endif // MUTEX_HPP_
