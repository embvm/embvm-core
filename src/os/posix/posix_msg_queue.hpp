#ifndef PTHREAD_MSG_QUEUE_HPP_
#define PTHREAD_MSG_QUEUE_HPP_

#include <cassert>
#include <rtos/msg_queue.hpp>

#ifdef __APPLE__
#include "posix_condition_variable.hpp"
#include <pthread.h>
#include <queue>
#include <thread>
#else
#include <mqueue.h>
#endif

namespace os::posix
{
/// @addtogroup POSIXOS
/// @{

#ifdef __APPLE__

/** Posix Message Queue implementation for OSX
 *
 * @tparam TType The type of data to be stored in the message queue
 */
template<typename TType>
class MessageQueue final : public embvm::VirtualMessageQueue<TType>
{
  public:
	/** Construct a message queue
	 *
	 * @param queue_length The maximum size of the message queue.
	 */
	explicit MessageQueue(size_t queue_length) noexcept : queue_max_(queue_length) {}

	/// Default destructor, cleans up the message queue.
	~MessageQueue() {}

	bool push(TType val, embvm::os_timeout_t timeout = embvm::OS_WAIT_FOREVER) noexcept final
	{
		// We initialize to true so we can enter the loop on the first run if full()
		bool wait_success = true;

		while(full() && (wait_success == true))
		{
			mutex_.lock();

			wait_success = cv_.wait(&mutex_, timeout);

			mutex_.unlock();
		}

		if(wait_success == true)
		{
			mutex_.lock();
			queue_.push(val);
			mutex_.unlock();
			cv_.broadcast();
		}

		// The wait status determines our success in pushing a new value
		return wait_success;
	}

	std::optional<TType> pop(embvm::os_timeout_t timeout = embvm::OS_WAIT_FOREVER) noexcept final
	{
		// We set this to true so we run the loop on the first iteration if empty();
		bool wait_success = true;

		std::optional<TType> val;

		while(queue_.empty() && (wait_success == true))
		{
			mutex_.lock();

			wait_success = cv_.wait(&mutex_, timeout);

			mutex_.unlock();
		}

		if(wait_success == true)
		{
			mutex_.lock();
			val = queue_.front();
			queue_.pop();
			mutex_.unlock();
		}

		return val;
	}

	size_t size() const noexcept final
	{
		return queue_.size();
	}

	void reset() noexcept final
	{
		while(!queue_.empty())
		{
			queue_.pop();
		}
	}

	bool empty() const noexcept final
	{
		return queue_.empty();
	}

	bool full() const noexcept final
	{
		return (queue_.size() == queue_max_);
	}

	embvm::msgqueue::handle_t native_handle() const noexcept final
	{
		return reinterpret_cast<embvm::msgqueue::handle_t>(&queue_);
	}

  private:
	/// THe maximum size of this message queue.
	size_t queue_max_;

	/// The storage for this message queue.
	std::queue<TType> queue_;

	/// The condition variable used to notify waiting threads.
	posix::ConditionVariable cv_;

	/// The mutex used to protect access to the queue.
	posix::Mutex mutex_;
};
#else
// TODO: test mqueue.h implementation on Linux
/** Posix Message Queue implementation for Linux
 *
 * @tparam TType The type of data to be stored in the message queue
 */
template<typename TType>
class MessageQueue final : public embvm::VirtualMessageQueue<TType>
{
  public:
	/** Construct a message queue with name and length
	 *
	 * @param queue_length The maximum size of the message queue.
	 */
	explicit MessageQueue(size_t queue_length) noexcept
	{
		struct mq_attr attributes;

		attributes.mq_flags = 0;
		attributes.mq_maxmsg = queue_length;
		attributes.mq_msgsize = sizeof(TType);
		attributes.mq_curmsgs = 0;

		handle_ = mq_open("embvm Msg Q", O_CREAT | O_EXCL | O_RDWR, S_IWUSR, &attributes);
		assert(handle_ != -1);
	}

	/// Default destructor
	~MessageQueue() noexcept
	{
		mq_close(handle_);
	}

	// TODO: implement timed version
	bool push(TType val, embvm::os_timeout_t timeout = embvm::OS_WAIT_FOREVER) noexcept final
	{
		assert(timeout == embvm::OS_WAIT_FOREVER); // timed version not yet supported
		int r = mq_send(handle_, reinterpret_cast<char*>(&val), sizeof(TType),
						1); // 1 picked because all priorities are equal
		assert(r == 0);
	}

	// TODO: implement timed version, use mq_notify
	std::optional<TType> pop(embvm::os_timeout_t timeout = embvm::OS_WAIT_FOREVER) noexcept final
	{
		assert(timeout == embvm::OS_WAIT_FOREVER); // timed version not yet supported

		TType val;

		auto bytes_read =
			mq_receive(handle_, reinterpret_cast<char*>(&val), sizeof(TType), nullptr);

		if(bytes_read == sizeof(TType))
		{
			return val;
		}

		return std::nullopt;
	}

	size_t size() const noexcept final
	{
		struct mq_attr attributes;
		int r = mq_getattr(handle_, &attributes);
		assert(r == 0);

		return attributes.mq_curmsgs;
	}

	void reset() noexcept final
	{
		size_t bytes_read;

		do
		{
			TType val;
			bytes_read = mq_receive(handle_, reinterpret_cast<char*>(&val), sizeof(TType), nullptr);
		} while(bytes_read > 0);
	}

	bool empty() const noexcept final
	{
		struct mq_attr attributes;
		int r = mq_getattr(handle_, &attributes);
		assert(r == 0);

		return attributes.mq_curmsgs == 0;
	}

	bool full() const noexcept final
	{
		struct mq_attr attributes;
		int r = mq_getattr(handle_, &attributes);
		assert(r == 0);

		return attributes.mq_curmsgs == attributes.mq_maxmsg;
	}

	embvm::msgqueue::handle_t native_handle() const noexcept final
	{
		return static_cast<embvm::msgqueue::handle_t>(handle_);
	}

  private:
	/// The message queue instance handle.
	mqd_t handle_;
};
#endif

/// @}

} // namespace os::posix

#endif // PTHREAD_MSG_QUEUE_HPP_
