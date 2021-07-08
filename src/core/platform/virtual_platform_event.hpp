// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#ifndef VIRTUAL_PLATFORM_EVENT_HPP_
#define VIRTUAL_PLATFORM_EVENT_HPP_

#include "event_center.hpp"
#include "virtual_platform.hpp"

namespace embvm
{
/** Add an event manager to the VirtualPlatform through inheritance.
 *
 * Your platform can inherit from this class to add event APIs a the platform level
 * This template is specialized based on the class of EventCenter you want to create
 *
 * Example declaration:
 * @code
 *	using PlatformEventCenter = DynamicEventCenter;
 *	class UnitTestPlatform : public VirtualPlatform,
 *		public PlatformEventManagement<PlatformEventCenter>`
 * @endcode
 *
 * @tparam TEventCenter The class of EventCenter you want to use with the Platform Event Manager.
 *
 * @ingroup FrameworkPlatform
 */
template<class TEventCenter>
class PlatformEventManagement
{
  private:
	/// Convenience alias for the signal type
	using event_sig_t = typename TEventCenter::sig_t;

	/// Convenience alias for the callback type
	using event_cb_t = typename TEventCenter::cb_t;

  public:
	/** Subscribe to an event.
	 *
	 * @param sig The signal to subscribe to.
	 * @param cb The callback function to invoke when the signal is raised.
	 * @returns A handle to the event subscription. The handle must remain within scope for the
	 * subscription to be valid. When he handle leaves scope, the
	 */
	auto subscribeToEvent(event_sig_t sig, const event_cb_t& cb) noexcept
	{
		return event_manager_.subscribe(sig, cb);
	}

	/// @overload auto subscribeToEvent(event_sig_t sig, const event_cb_t& cb)
	auto subscribeToEvent(event_sig_t sig, event_cb_t&& cb) noexcept
	{
		return event_manager_.subscribe(sig, cb);
	}

	/// Unsubscribe from an event
	/// @param handle The event handle to unsubscribe.
	void unsubscribeFromEvent(typename TEventCenter::EventHandle& handle) noexcept
	{
		event_manager_.unsubscribe(handle);
	}

	/** Forwards a signal to the event manager
	 *
	 * @param sig The signal to raise to the platform event manager.
	 */
	void signal(event_sig_t sig) noexcept
	{
		event_manager_.signal(sig);
	}

	// TODO: Fix, this is slicing
	/** Forwards an event to the event manager
	 *
	 * @param event The event to publish to the platform event manager.
	 */
	void publishEvent(EventBase event) noexcept
	{
		event_manager_.publish(event);
	}

  protected:
	/// The instance of the event manager that belongs to the platform.
	TEventCenter event_manager_;
};

} // namespace embvm

#endif // VIRTUAL_PLATFORM_EVENT_HPP_
