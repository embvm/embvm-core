#ifndef VIRTUAL_MSG_QUEUE_HPP_
#define VIRTUAL_MSG_QUEUE_HPP_

#include "rtos_defs.hpp"
#include <optional>

namespace embvm
{
/// @addtogroup FrameworkOS
/// @{

/** Defines the VirtualMessageQueue interface.
 *
 * VirtualMessageQueue defines the common message queue interfaces for a VirtualRTOS. RTOS
 * implementations should derive from this base class. This class is templated, because message
 * queues can hold any type of data structure.
 *
 * The factory is responsible for creating the message queue, so the constructor is not defined
 * in this interface class.
 *
 * @tparam TType The type of data to be stored in the message queue.
 */
template<typename TType>
class VirtualMessageQueue
{
  public:
	/// Deleted copy constructor
	VirtualMessageQueue(const VirtualMessageQueue&) = delete;

	/// Deleted copy assignment operator
	const VirtualMessageQueue& operator=(const VirtualMessageQueue&) = delete;

	/// Default move constructor
	VirtualMessageQueue(VirtualMessageQueue&&) noexcept = default;

	/// Default move assignment operator
	VirtualMessageQueue& operator=(VirtualMessageQueue&&) noexcept = default;

	/** Post a message to the queue.
	 *
	 * Post a message to the queue. This function blocks the calling thread until there is an
	 *	available slot (if OS_WAIT_FOREVER is specified as a timeout), or until the timeout expires.
	 *
	 * Posting a message to the queue will notify all threads waiting on a new message (via pop()).
	 *
	 * @param val The data value to post to the queue.
	 * @param timeout The time to wait for a new message. A value of OS_WAIT_FOREVER
	 * 	indicates that the thread should block until a new message is posted.
	 * @returns true if the message was successfully posted, false otherwise.
	 */
	virtual bool push(TType val, embvm::os_timeout_t timeout = embvm::OS_WAIT_FOREVER) noexcept = 0;

	/** Retrieve a message from the queue.
	 *
	 * Retrieve a message from the queue. This function blocks the calling thread until a message
	 *	is received (if OS_WAIT_FOREVER is specified as a timeout), or until the timeout expires.
	 *
	 * @param timeout The time to wait for a new message. A value of OS_WAIT_FOREVER
	 * 	indicates that the thread should block until a new message is posted.
	 * @returns an optional value. Use `has_value()` member function to determine if the value is
	 * 	valid. A valid value is only returned on success.
	 */
	virtual std::optional<TType>
		pop(embvm::os_timeout_t timeout = embvm::OS_WAIT_FOREVER) noexcept = 0;

	/** Get the current size of the message queue.
	 *
	 * Size represents the number of elements stored, rather than the maximum size (aka capacity).
	 *
	 * @returns the current number of elements stored in the message queue.
	 */
	virtual size_t size() const noexcept = 0;

	/** Reset the message queue to an empty state.
	 *
	 * @post The message queue is emptied, with size 0.
	 */
	virtual void reset() noexcept = 0;

	/** Check if the message queue is empty.
	 *
	 * @returns true if the queue is empty, false otherwise.
	 */
	virtual bool empty() const noexcept = 0;

	/** Check if the message queue is full.
	 *
	 * @returns true if the queue is full, false otherwise.
	 */
	virtual bool full() const noexcept = 0;

	/// Get the native handle for the message queue object
	/// @returns the native handle for this message queue object. The handle will
	/// 	always return the handle_t and it must be cast to the native handle type by the user.
	[[nodiscard]] virtual embvm::msgqueue::handle_t native_handle() const noexcept = 0;

  protected:
	VirtualMessageQueue() = default;

	/// Derived classes should ensure their destructor deletes the OS msg queue
	virtual ~VirtualMessageQueue() noexcept = default;
};

/// @}
// End Group

} // namespace embvm

#endif // VIRTUAL_MSG_QUEUE_HPP_
