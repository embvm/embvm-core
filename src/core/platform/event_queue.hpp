#ifndef EVENT_QUEUE_HPP_
#define EVENT_QUEUE_HPP_

#include "event.hpp"
#include <active_object/active_object.hpp>
#include <etl/queue.h>
#include <queue>

namespace embvm
{
/// @addtogroup FrameworkPlatform
/// @{

/** Keep a queue of events
 *
 * Manage a queue of events of a specified type. The EventQueue can utilize both
 *	static and dynamic memory allocation strategies.
 *
 * @tparam TEvent The event type which this queue manages. Can be embvm::EventBase, or a
 * derived event with data.
 * @tparam TQueueSize The maximum size of the event queue. Size 0 indicates dynamic memory will be
 * used. All other sizes will enable static memory allocation.
 */
template<typename TEvent, size_t TQueueSize = 0>
class EventQueue
{
	/// The type of the underlying queue.
	/// Size 0 indicates dynamic memory usage (std::queue), other sizes indicate static memory.
	using TQueueType = typename std::conditional<(TQueueSize == 0), std::queue<TEvent>,
												 etl::queue<TEvent, TQueueSize>>::type;

  public:
	/// Default constructor
	EventQueue() = default;

	/// Default destructor
	~EventQueue() = default;

	/// Deleted copy constructor
	EventQueue(const EventQueue&) = delete;

	/// Deleted copy assignment operator
	const EventQueue& operator=(const EventQueue&) = delete;

	/// Default move constructor
	EventQueue(EventQueue&&) = default;

	/// Default move assignment operator
	EventQueue& operator=(EventQueue&&) = default;

	/// Check if the queue is empty
	/// @returns true if the queue is empty, false if it has data.
	bool empty() const noexcept
	{
		return q_.empty();
	}

	/// Check the size of the queue.
	/// @returns the number of events currently enqueued.
	size_t size() const noexcept
	{
		return q_.size();
	}

	// TODO: Fix, this is slicing
	/// Add an event to the queue
	/// @param e The event to add to the queue for processing.
	void push(TEvent e) noexcept
	{
		q_.push(e);
	}

	// TODO: should this be a copy? how to handle post-slicing fix?
	/// Get the front element of the queue, without popping
	/// @returns a reference to the front element.
	TEvent& front() const noexcept
	{
		return q_.front()();
	}

	// TODO: Update for slicing fix
	/** Pop the front element from the queue
	 *
	 * @returns the front element from the queue.
	 * @post The element has been removed from the queue.
	 */
	TEvent pop() noexcept
	{
		auto e = q_.front();
		q_.pop();

		return e;
	}

  private:
	/// The underlying event queue storage.
	TQueueType q_;
};

#if 0 // TODO: redesign this class
/** Event Queue with its own thread of control.
 *
 * The ActiveEventQueue is an EventQueue combined with an embutil::activeObject
 *	(an object with its own thread of control). The ActiveEventQueue can utilize both
 *	static and dynamic memory allocation strategies.
 *
 * This active event queue manages a single type of event (TEvent).
 *
 * Derived classes must implement the process_event_(TEvent e) function.
 *
 * @tparam TConcreteQueue The derived queue class. This is the CRTP pattern.
 * @tparam TEvent The event type which this queue manages. Can be embvm::EventBase, or a
 *derived event with data.
 * @tparam TQueueSize The maximum size of the event queue. Size 0 indicates dynamic memory will be
 *used. All other sizes will enable static memory allocation.
 * @tparam TLock The type of lock interface to use. Default is std::mutex.
 * @tparam TCond The type of condition variable interface to use. Default is
 *std::condition_variable.
 */
template<typename TConcreteQueue, typename TEvent, size_t TQueueSize = 0,
	typename TLock = std::mutex, typename TCond = std::condition_variable>
class ActiveEventQueue : embutil::activeObject<ActiveEventQueue, TEvent, TQueueSize, TLock, TCond>
{
  public:
	/// Default constructor
	ActiveEventQueue() {}

	/// Deleted copy constructor
	ActiveEventQueue(const ActiveEventQueue&) = delete;

	/// Deleted copy assignment operator
	const ActiveEventQueue& operator=(const ActiveEventQueue&) = delete;

	/// Deleted move constructor
	ActiveEventQueue(ActiveEventQueue&&) = delete;

	/// Deleted move assignment operator
	ActiveEventQueue& operator=(ActiveEventQueue&&) = delete;

	/** Check if the event queue is empty
	 * @returns true if the queue is empty, false otherwise
	 */
	bool empty()
	{
		return queuedCount() == 0;
	}

	/** Get the current event queue size
	 * @returns number of events currently enqueued
	 */
	size_t size()
	{
		return queuedCount();
	}

	/** Enqueue an event
	 *
	 * Enqueues the event with the Active Object's thread
	 *
	 * @param e Event to push onto the queue
	 */
	void push(TEvent e)
	{
		enqueue(e);
	}

	/** Active Object process_() function
	 *
	 * The process_ operation corresponds to an active object.
	 *
	 * Clients must implement process_event_().
	 *
	 * @param e Event which the client thread must process
	 */
	void process_(TEvent e)
	{
		static_cast<TConcreteQueue*>(this)->process_event_(e);
	}
};
#endif

/// @}
// End Group

} // namespace embvm

#endif // EVENT_QUEUE_HPP_
