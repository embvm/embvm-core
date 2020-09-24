#ifndef EVENT_HPP_
#define EVENT_HPP_

#include <cstdint>
#include <type_safe/boolean.hpp>

// TODO: clean up memory problems,shown below. This will slice.
/*
 * You can then transform event base objects to your class (as long as the types match!)
 *	```
 *	void passEventWithDataCb(EventBase e)
 *	{
 *		assert(e.sig == eventWithData);
 * 		TestEventWithData* event = static_cast<TestEventWithData*>(&e);
 *		eventUpdate = event->data;
 *	}
 *	```
 */

namespace embvm
{
/// @addtogroup FrameworkPlatform
/// @{

/// Signal type which is used for function inputs.
/// This type is preferred because users are able to define their own signal types.
using Signal_t = uint32_t;

/** Default framework event signal definitions
 *
 * These default signals carry no data.
 *
 * To add your own event types, create an enum in your project code and use Event_EXTENSION_START
 * as the first value.
 */
enum Signal
{
	Event_Invalid = 0,
	Event_ProcessorInitd,
	Event_HwPlatformInitd,
	Event_PlatformInitd,
	Event_EXTENSION_START
};

/** Base class for events
 *
 * The EventBase class represents a signal with no associated data
 * Templated to allow use of any signal definition type within the framework
 *
 * You can define custom events with data by inheriting from this class:
 *
 * ```
 * class TestEventWithData : public EventBase<MySignal_t>
 * {
 *  public:
 *	TestEventWithData() : EventBase<MySignal_t>(eventWithData) {}
 *	bool data = false;
 * };
 * ```
 *
 * You can then transform event base objects to your class (as long as the types match!)
 *	```
 *	void passEventWithDataCb(EventBase e)
 *	{
 *		assert(e.sig == eventWithData);
 * 		TestEventWithData* event = static_cast<TestEventWithData*>(&e);
 *		eventUpdate = event->data;
 *	}
 *	```
 */
class EventBase
{
  public:
	/** Default constructor
	 *
	 * Creates a static event base (e.g. on the stack)
	 *
	 * @param s The signal which corresponds to this event
	 */
	explicit EventBase(Signal_t s = Event_Invalid) noexcept : sig(s) {}

	/** Check whether this event is safe to free
	 *
	 * @returns true if the event was dynamically allocated, false if statically allocated (can't
	 * free)
	 */
	type_safe::boolean safeToFree() const noexcept
	{
		return dynamic_;
	}

	// TODO: this doesn't work for generating events that derive from EventBase
	// because slicing will occur - we won't actually have allocated large enough space.
	// need to figure that out.
	// See issue #149
#if 0
	/** Allocate a new event dynamically
	 *
	 * Dynamically allocates a new event using the requested signal
	 *
	 * returns a unique ptr to an Event with the specified Signal
	 */
	static std::unique_ptr<EventBase<TSignal>> allocate(TSignal s = TSignal::Invalid)
	{
		return std::unique_ptr<EventBase<TSignal>>(new EventBase<TSignal>(s, true));
	}
#endif

  private:
	EventBase(Signal_t s, bool d) noexcept : sig(s), dynamic_(d) {}

  public:
	/// Signal ID for this event
	Signal_t sig = Event_Invalid;

  private:
	/// nullptr indicates this is statically allocated and should not be freed
	type_safe::boolean dynamic_ = false;
};

/// @}
// End group

} // namespace embvm

#endif // EVENT_HPP_
