// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#ifndef SEMAPHORE_HPP_
#define SEMAPHORE_HPP_

#include "rtos_defs.hpp"

namespace embvm
{
/// @addtogroup FrameworkOS
/// @{

/** Defines the VirtualSemaphore interface.
 *
 * VirtualSemaphore defines the common semaphore interfaces for a VirtualRTOS. RTOS implementations
 * should derive from this base class.
 *
 * The factory is responsible for creating the semaphore, so the constructor is not defined
 * in this interface class.
 */
class VirtualSemaphore
{
  public:
	/// Delete the copy constructor
	VirtualSemaphore(const VirtualSemaphore&) = delete;

	/// Delete the copy assignment operator
	auto operator=(const VirtualSemaphore&) -> const VirtualSemaphore& = delete;

	/// Default move constructor
	VirtualSemaphore(VirtualSemaphore&&) = default;

	/// Default move operator
	auto operator=(VirtualSemaphore&&) -> VirtualSemaphore& = default;

	/** Increment count
	 * Return a resource to the semaphore, incrementing the count.
	 */
	virtual void give() noexcept = 0;

	/** Increment count from an ISR
	 * Return a resource to the semaphore, incrementing the count.
	 * This function variant is safe to call from an interrupt routine. No locking
	 * can be used inside this variant.
	 */
	virtual void giveFromISR() noexcept = 0;

	/** Decrement count
	 * Take a resource from the semaphore, decrementing the count.
	 *
	 * @param timeout The time to wait for a semaphore resource. A value of WAIT_FOREVER
	 * 	indicates that the thread should block until the semaphore can be acquired.
	 * @returns true if take was successful, false if a timeout occurred.
	 */
	virtual auto take(const embvm::os_timeout_t& timeout = embvm::OS_WAIT_FOREVER) noexcept
		-> bool = 0;

	/** Check the count of the semaphore.
	 *
	 * @returns The current semaphore count.
	 */
	[[nodiscard]] virtual auto count() const noexcept -> embvm::semaphore::count_t = 0;

	[[nodiscard]] virtual auto native_handle() const noexcept -> embvm::semaphore::handle_t = 0;

  protected:
	VirtualSemaphore() = default;

	/// Derived classes should ensure their destructor deletes the OS semaphor
	virtual ~VirtualSemaphore() noexcept;
};

/// @}
// End Group

} // namespace embvm

#endif // SEMAPHORE_HPP_
