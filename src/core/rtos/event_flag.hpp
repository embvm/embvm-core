// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#ifndef EVENT_FLAG_HPP_
#define EVENT_FLAG_HPP_

#include "rtos_defs.hpp"

namespace embvm
{
/// @addtogroup FrameworkOS
/// @{

/** Defines the VirtualEventFlag interface.
 *
 * VirtualEventFlag defines the common event flag interfaces for a VirtualRTOS. RTOS implementations
 * should derive from this base class.
 *
 * The factory is responsible for creating the event flag, so the constructor is not defined
 * in this interface class.
 */
class VirtualEventFlag
{
  public:
	/** Get (or wait for) event flags.
	 *
	 * Get the event flag bits. If the required event flags are not set, this function will block
	 * the calling thread until the flags are set or the timeout expires.
	 *
	 * To see all bits currently set, set `bits_wait` to `~0` and `timeout_t` to the minimal value
	 * (`timeout_t(1)`).
	 *
	 * @param bits_wait A bitmask of bits to wait for in the event flags group.
	 * @param opt The wait option, which is combined with bits_wait to control the behavior of
	 *	get(). embvm::eventflag::option::OR will return once any of the bits specified in bits_wait
	 *are set in the event flag group. embvm::eventflag::option::AND will only return once ALL of
	 *the bits specified in bits_wait are set in the event flag group.
	 * @param clearOnExit If true, all event flags that were specified in bits_wait and set in the
	 *	event flag group will be set to 0. If false, event flag bits will not be modified.
	 * @param timeout Specify a timeout for the get operation. If OS_WAIT_FOREVER is supplied, this
	 *call will block until the event flags are set. Otherwise, the function will timeout after the
	 *elapsed time if flags are not set.
	 * @returns 0 if timeout, or flags that were currently set according to bits_wait
	 */
	virtual auto get(embvm::eventflag::flag_t bits_wait,
					 embvm::eventflag::option opt = embvm::eventflag::option::OR,
					 bool clearOnExit = true,
					 const embvm::os_timeout_t& timeout = embvm::OS_WAIT_FOREVER) noexcept
		-> embvm::eventflag::flag_t = 0;

	/** Set event flags.
	 *
	 * Set event flags from a threaded context. Threads waiting on event flags (via get()) will be
	 * notified.
	 *
	 * @param bits A bitmask representing the bits to set in the event flag group.
	 */
	virtual void set(embvm::eventflag::flag_t bits) noexcept = 0;

	/** Set event flags from an ISR.
	 *
	 * Set event flags from an interrupt context. This call must not use a lock.
	 * Threads waiting on event flags (via get()) will be notified.
	 *
	 * @param bits A bitmask representing the bits to set in the event flag group.
	 */
	virtual void setFromISR(embvm::eventflag::flag_t bits) noexcept = 0;

	/** Clear all event flag bits.
	 *
	 * @post Event flag group value is reset to '0'.
	 */
	virtual void clear() noexcept = 0;

	/// Get the native handle for the event flags group object
	/// @returns the native handle for this event flags object. The handle will
	/// 	always return the handle_t and it must be cast to the native handle type by the user.
	[[nodiscard]] virtual auto native_handle() const noexcept -> embvm::eventflag::handle_t = 0;

	/// Delete the copy constructor
	VirtualEventFlag(const VirtualEventFlag&) = delete;

	/// Delete the copy assignment operator
	auto operator=(const VirtualEventFlag&) -> const VirtualEventFlag& = delete;

	/// Default move constructor
	VirtualEventFlag(VirtualEventFlag&&) = default;

	/// Default move assignment operator
	auto operator=(VirtualEventFlag&&) -> VirtualEventFlag& = default;

  protected:
	VirtualEventFlag() = default;

	/// Derived classes should ensure their destructor deletes the OS event flag
	virtual ~VirtualEventFlag() noexcept;
};

/// @}
// End Group

} // namespace embvm

#endif // EVENT_FLAG_HPP_
