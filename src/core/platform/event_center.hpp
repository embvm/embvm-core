// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#ifndef EVENT_MANAGER_HPP_
#define EVENT_MANAGER_HPP_

#include "event.hpp"
#include <cassert>
#include <dispatch/dispatch.hpp>
#include <etl/list.h>
#include <etl/map.h>
#include <inplace_function/inplace_function.hpp>
#include <list>
#include <map>
#include <mutex>

namespace embvm
{
///@addtogroup FrameworkPlatform
/// @{

// TODO: update to a function queue to remove this need
/// Maximum size of the Event center callback functor object.
static constexpr size_t EVENT_CENTER_REQD_STATIC_FUNCTION_SIZE = (sizeof(void*) * 8);

/**
 * This alias represents a static dispatch queue which can be used with the event manager
 * Primarily this alias is used to prevent users from needing to declare a long
 * type declaration just because of the need to increase inplace_function<>'s size
 *
 * @tparam TSize the maximum number of entries in the dispatch queue
 * @tparam TThreadCnt the number of threads to use in the dispatch queue
 * @tparam TFunc the function object that will be stored in the queue
 */
template<const size_t TSize, const size_t TThreadCnt = 1,
		 typename TFunc = stdext::inplace_function<void(), EVENT_CENTER_REQD_STATIC_FUNCTION_SIZE>>
using StaticEventQueue = embutil::StaticDispatchQueue<TSize, TThreadCnt, TFunc>;

/** Global Event Manager class
 *
 * Allows you to subscribe and unsubscribe to/from events.
 * Allows you to post signals and events.
 * Calls are dispatched.
 * You must keep your handle alive when you register for a callback.
 *Handles will be automatically cleaned up when they go out of scope!
 *
 * Allowed: publishing and signaling from an interrupt.
 * Not allowed: subscribing or unsubscribing from an interrupt.
 *
 * This class provides support for both dynamic memory allocation and static memory allocation.
 * The default TMaxEvents and TMaxSubscribersPerEvent settings (0) indicate dynamic memory.
 *
 * @note Total static memory usage is `(TMaxEvents * TMaxSubscribersPerEvent * sizeof(event))`
 *
 * @tparam TMaxEvents The maximum number of events to store in the queue.
 *	0 indicates dynamic memory allocation will be used; all other numbers use static memory.
 * @tparam TMaxSubscribersPerEvent The maximum number of subscribers that can register
 * 	for each event. 0 indicates dynamic memory allocation will be used; all other numbers
 *  use static memory.
 * @tparam TFunc The function object type for the DispatcherFunc.
 */
template<const size_t TMaxEvents = 0, const size_t TMaxSubscribersPerEvent = 0,
		 typename TFunc = stdext::inplace_function<void(), EVENT_CENTER_REQD_STATIC_FUNCTION_SIZE>>
class EventCenter
{
	static_assert(
		// Check that we are using dynamic memory throughout
		((TMaxEvents == 0) && (TMaxSubscribersPerEvent == 0)) ||
			// or that we are using static memory throughout
			((TMaxEvents != 0) && (TMaxSubscribersPerEvent != 0)),
		"TMaxEvents and TMaxSubscribersPerEvent should both be 0 (dynamic memory) or non-zero "
		"(static memory)");

  public:
	// Forward declaration of EventHandle, defined below,
	class EventHandle;

	/// @brief Underlying signal type.
	using sig_t = Signal_t;

	/// @brief Callback type.
	// Uses inplace_function regardless of static or dynamic memory allocation.
	using cb_t = stdext::inplace_function<void(embvm::EventBase)>;

  private:
	/// @brief The function prototype for the Dispatcher.
	/// The dispacher uses inplace_function regardless of static or dynamic memory allocation.
	using DispatcherFunc = stdext::inplace_function<void(const TFunc&)>;

	/// @brief Type for storing lists of event subscribers.
	/// TListType is either std::list or etl::list depending on static or dynamic memory
	/// allocations. For static allocations, TListType is capped at TMaxSubscribersPerEvent.
	typedef typename std::conditional<(TMaxSubscribersPerEvent == 0), std::list<cb_t>,
									  etl::list<cb_t, TMaxSubscribersPerEvent>>::type TListType;

	/// @brief Type for storing event subscription lists.
	/// TMapType is either std::map or etl::map depending on static or dynamic memory allocations.
	/// For static allocations, TMapType is capped at TMaxEvents.
	/// The map key is the signal ID.
	typedef typename std::conditional<(TMaxEvents == 0), std::map<Signal_t, TListType>,
									  etl::map<Signal_t, TListType, TMaxEvents>>::type TMapType;

  public:
	/** Default EventCenter constructor.
	 *
	 * Creating an EventCenter without a dispatcher will result in the manager invoking the
	 * functions.
	 */
	EventCenter() noexcept
		: dispatcher_(std::bind(&EventCenter::EventCenterDispatch, this, std::placeholders::_1))
	{
	}

	/** Construct EventCenter with a dispatcher.
	 *
	 * Specify a function to dispatch all of the event callbacks to.
	 * Used with a dispatch queue, or other type of system event queue.
	 */
	explicit EventCenter(const DispatcherFunc& dispatcher) noexcept : dispatcher_(dispatcher) {}

	/// Default destructor
	~EventCenter() {}

	/// Deleted copy constructor
	EventCenter(const EventCenter&) = delete;

	/// Deleted copy assignment operator
	const EventCenter& operator=(const EventCenter&) = delete;

	/// Deleted move constructor
	EventCenter(EventCenter&&) = delete;

	/// Deleted move assignment operator
	EventCenter& operator=(EventCenter&&) = delete;

	/** Create an event on the stack.
	 *
	 * @returns A stack allocated event with the specified signal ID
	 */
	embvm::EventBase createSignal(Signal_t sig = Signal::Event_Invalid) noexcept
	{
		return embvm::EventBase(sig);
	}

	// TODO: Disabled because we need to be able to allocate derived classes, not just the base
	// See issue #149
#if 0
/** Create an event with dynamic memory.
	 *
	 * @returns A stack allocated event with the specified signal ID
	 */
	template<typename TReturn = std::unique_ptr<embvm::EventBase>>
	auto allocateSignal(Signal_t sig = Signal::Event_Invalid)
		-> std::enable_if_t<TMaxEvents == 0, TReturn>
	{
		return embvm::EventBase::allocate(sig);
	}
#endif

	/** Check how many event types are registered.
	 *
	 * @returns The count of event types which have been subscribed to by listeners.
	 */
	size_t numSubscribedEvents() noexcept
	{
		return events_.size();
	}

	/** Check how many subscribers are listening to a specific event.
	 *
	 * @param sig The type of event to check.
	 * @returns The number of subscribers listening for the specified event.
	 */
	size_t numSubscribers(Signal_t sig) noexcept
	{
		return events_[sig].size();
	}

	/** Subscribe to an event.
	 *
	 * Subscribes to an event with a callback function. The callback will be
	 * invoked whenever an event of the specified type is signalled.
	 *
	 * @param sig The type of event to listen for.
	 * @param cb The function which will be called when the event occurs.
	 * @returns Handle representing event subscription.
	 *	The user must keep the handle within scope. Once the handle goes out of
	 *	scope, the subscription will be automatically unregistered.
	 */
	EventCenter::EventHandle subscribe(Signal_t sig, const cb_t& cb) noexcept
	{
		std::lock_guard<std::mutex> lock(mutex_);

		if constexpr(TMaxEvents > 0)
		{
			assert(events_.size() < TMaxEvents &&
				   "Max events reached - increase EventCenter::TMaxEvents\n");
		}

		auto& list = events_[sig];

		if constexpr(TMaxSubscribersPerEvent > 0)
		{
			assert(list.size() < TMaxSubscribersPerEvent &&
				   "Max subscribers reached for this event - increase "
				   "EventCenter::TMaxSubscribersPerEvent\n");
		}

		EventHandle handle(this, sig, list.insert(list.end(), cb));
		return handle;
	}

	/** Subscribe to an event.
	 *
	 * Subscribes to an event with a callback function. The callback will be
	 * invoked whenever an event of the specified type is signalled.
	 *
	 * @param sig The type of event to listen for.
	 * @param cb The function which will be called when the event occurs.
	 * @returns Handle representing event subscription.
	 *	The user must keep the handle within scope. Once the handle goes out of
	 *	scope, the subscription will be automatically unregistered.
	 */
	EventCenter::EventHandle subscribe(Signal_t sig, cb_t&& cb) noexcept
	{
		std::lock_guard<std::mutex> lock(mutex_);

		if constexpr(TMaxEvents > 0)
		{
			assert(events_.size() < TMaxEvents &&
				   "Max events reached - increase EventCenter::TMaxEvents\n");
		}

		auto& list = events_[sig];

		if constexpr(TMaxSubscribersPerEvent > 0)
		{
			assert(list.size() < TMaxSubscribersPerEvent &&
				   "Max events reached - increase EventCenter::TMaxSubscribersPerEvent\n");
		}

		EventHandle handle(this, sig, list.insert(list.end(), std::move(cb)));
		return handle;
	}

	/** Unsubscribe from listening for an event.
	 *
	 * Removes the subscription to the event. An unsubscribe request uses the EventHandle
	 * which was returned by subscribe(). Unsubscribe also happens automatically when
	 * the EventHandle goes out of scope.
	 *
	 * @param handle The handle to the subscription, which was returned by subscribe()
	 */
	void unsubscribe(EventCenter::EventHandle& handle) noexcept
	{
		std::lock_guard<std::mutex> lock(mutex_);

		if(handle)
		{
			auto& list = events_[handle.sig()];
			list.erase(handle.get());
			handle.is_valid_ = false;

			// Cleanup - this keeps numSubscribedEvents() accurate
			if(0 == list.size())
			{
				events_.erase(handle.sig());
			}
		}
	}

	/** Signal an event.
	 *
	 * Signals to the system that an event has occurred. All subscribers will
	 * have their callback function invoked.
	 *
	 * @param sig The event type to signal
	 */
	void signal(Signal_t sig) noexcept
	{
		auto& list = events_[sig];

		for(const auto& cb : list)
		{
			dispatcher_(std::bind(cb, embvm::EventBase(sig)));
		}
	}

	/** Publish an event.
	 *
	 * Signals to the system that an event has occurred. All subscribers will
	 * have their callback function invoked and will be passed the input event.
	 *
	 * @param event The event object that will be shared with the subscribers
	 */
	void publish(embvm::EventBase event) noexcept
	{
		auto& list = events_[event.sig];

		for(const auto& cb : list)
		{
			dispatcher_(std::bind(cb, event));
		}
	}

  private:
	/** EventCenter Default Dispatch Function.
	 *
	 * If no external dispatcher is provided, event center has one which
	 * just executes the operation by default.
	 */
	void EventCenterDispatch(const TFunc& op) noexcept
	{
		op();
	}

  private:
	/// The map which stores event subscriptions.
	TMapType events_;
	/// Mutex which protects
	std::mutex mutex_;
	/// Dispatcher function which will handle callbacks.
	const DispatcherFunc dispatcher_;
};

#pragma mark - Event Handle Definition -

/** Handle for an event subscription
 *
 * EventHandle represents a handle to an event registration. EventHandles are created
 * during the subscribe() process. The lifetime of the EventHandle controls the lifetime of
 * the subscription: if the handle leaves scope, the subscription will be automatically revoked.
 *
 * Template parameters simply match those in the original EventCenter declaration, since
 * EventHandle is a class member.
 *
 *
 */
template<const size_t TMaxEvents, const size_t TMaxSubscribersPerEvent, typename TFunc>
class EventCenter<TMaxEvents, TMaxSubscribersPerEvent, TFunc>::EventHandle
{
  private:
	/// Event Center is able to invoke private EventHandle functions
	friend class EventCenter;

	/// Convenience declaration: our handle is an iterator to the mapped type
	using TEventHandle = typename decltype(events_)::mapped_type::iterator;

	// We require iterators that are safe to use after an element is
	// inserted or deleted
	static_assert(std::is_convertible<TEventHandle, typename TMapType::iterator>::value ||
					  std::is_convertible<TEventHandle, typename TListType::iterator>::value,
				  "EventCenter::EventHandle does not have a safe iterator type -- "
				  "Either fix this static_assert to add another type which"
				  "supports iterators that are safe against insert/remove,"
				  "Or switch to std/etl::map and std/etl::list.");

  public:
	/// Move assignment operator
	EventHandle& operator=(EventHandle&& rhs) noexcept
	{
		handle_ = rhs.handle_;
		sig_ = rhs.sig_;
		is_valid_ = rhs.is_valid_;
		mgr_ = rhs.mgr_;
		rhs.is_valid_ = false;

		return (*this);
	}

	/// Delete the copy constructor
	EventHandle(const EventHandle&) = delete;

	/// Delete the copy assignment operator
	const EventHandle& operator=(const EventHandle&) = delete;

	/** EventHandle destructor.
	 *
	 * On destruction, the handle will automatically unsubscribe itself from the event.
	 */
	~EventHandle() noexcept
	{
		// Handles should unregister themselves on destruction
		reset();
	}

	/// Boolean operator returns true if the handle is valid, false otherwise.
	operator bool() const noexcept
	{
		return is_valid_;
	}

	/** Check the signal this subscription corresponds to.
	 *
	 * @returns the signal object which matches this handle
	 */
	const Signal_t& sig() const noexcept
	{
		return sig_;
	}

	/// Unregister the callback and invalidate the handle
	void reset() noexcept
	{
		if(is_valid_)
		{
			mgr_->unsubscribe(*this);
			is_valid_ = false;
		}
	}

  private:
	/** EventHandle constructor.
	 *
	 * @param owner The EventCenter which owns this EventHandle
	 * @param sig The signal which the subscriber is interested in
	 * @param iter The handle object
	 */
	explicit EventHandle(EventCenter* owner, Signal_t sig, TEventHandle iter) noexcept
		: sig_(sig), handle_(iter), is_valid_(true), mgr_(owner)
	{
	}

	/// Move constructor
	EventHandle(EventHandle&& rhs) noexcept
		: sig_(rhs.sig_), handle_(rhs.handle_), is_valid_(rhs.is_valid_), mgr_(rhs.mgr_)
	{
		rhs.is_valid_ = false;
	}

	/** Get the underlying handle
	 *
	 * @returns The underlying handle storage object
	 */
	TEventHandle get() const noexcept
	{
		return handle_;
	}

  private:
	Signal_t sig_;
	TEventHandle handle_;
	bool is_valid_;
	EventCenter* mgr_;
};

/** Declare an EventCenter that uses dynamic memory allocation
 *
 * A EventCenter utilizes dynamic memory allocation and can grow to the limit that memory
 * allows. This type is useful for host-based simulation platforms, as well as embedded platforms
 * which allow dynamic memory allocation.
 */
using DynamicEventCenter = EventCenter<0, 0>;

/** Declare a DriverRegistry that uses static memory allocation.
 *
 * The size and memory allocation of the StaticDriverRegistry is known at compile-time.
 * No dynamic memory allocations are used.
 *
 * @tparam TMaxEvents The maximum number of events to store in the queue.
 *	0 indicates dynamic memory allocation will be used; all other numbers use static memory.
 * @tparam TMaxSubscribersPerEvent The maximum number of subscribers that can register
 * 	for each event. 0 indicates dynamic memory allocation will be used; all other numbers
 *  use static memory.
 */
template<const size_t TMaxEvents = 16, const size_t TMaxSubscribersPerEvent = 4>
using StaticEventCenter = EventCenter<TMaxEvents, TMaxSubscribersPerEvent>;

/// @}
// End group

} // namespace embvm

#endif // EVENT_MANAGER_HPP_
