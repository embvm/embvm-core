// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#ifndef DISPATCH_HPP_
#define DISPATCH_HPP_

#include <atomic>
#include <condition_variable>
#include <etl/function.h>
#include <etl/queue.h>
#include <etl/vector.h>
#include <function_queue/function_queue.hpp>
#include <functional>
#include <inplace_function/inplace_function.hpp>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>

namespace embutil
{
/// @defgroup DispatchQueue Dispatch Queues
/// Group of worker threads that enables asynchronous processing.
/// @ingroup FrameworkUtils
/// @{

/** Base class for dispatch queues
 *
 * This templated base class supports both static and dynamic dispatch queue implementations.
 *
 * This class is not intended to be used directly in a dispatch queue declaration.
 * Instead, use one of the specializations:
 * 	- `DynamicDispatchQueue`
 * 	- `StaticDispatchQueue`
 *
 * # Using a Dispatch Queue
 *
 * Users interact with a dispatch queue using the dispatch() APIs. The dispatch queue can accept
 * any functor: functions, function pointers, lambdas. The client passes a functor to the dispatch()
 * operation, which returns immediately to the client. The functor will be executed asynchronously
 * on the next available dispatch thread.
 *
 * @code
 * static void increment(void)
 * {
 * 	count++;
 * }
 *
 * embutil::DynamicDispatchQueue<> q(num_threads);
 * q.dispatch(count);
 * q.dispatch([] { printf("Hello from a lambda!\n"); });
 * @endcode
 *
 * Dispatch queues are used when an operation needs to run on a separate thread, but you don't want
 * to use explicit thread management. They support asynchronous processing models. A common usage
 * mode is to have the operation call dispatch() to carry out the next step in a processing
 * chain.
 *
 * Dispatch queues cannot be copied or moved.
 *
 * @tparam TSize The size of the storage queue. When TSize is 0, dynamic memory allocation will be
 *	used. Otherwise static memory types are used and the maximum number of operations is limited to
 *	TSize.
 * @tparam TThreadCount The number of threads to use with the dispatch queue.
 * @tparam TFunc The type representing a dispatch function prototype. Defaults to any functor
 *	which can be represented by `void(void)`.
 * @tparam TLock Type to use for the lock. Can be overriden if a custom mutex/lock should be used.
 * @tparam TCond Type to use for the condition variable. Can be overridden if a custom condition
 *	variable implementation should be used.
 */
template<const size_t TSize, const size_t TThreadCount,
		 typename TFunc = stdext::inplace_function<void()>, typename TLock = std::mutex,
		 typename TCond = std::condition_variable>
class DispatchQueue_Base
{
	/// Type definition for the thread storage vector.
	/// If TSize is 0, std::vector will be used (dynamic memory mode). Otherwise an etl::vector
	/// of size TSize is used.
	using TVecType = typename std::conditional<(TSize == 0), std::vector<std::thread>,
											   etl::vector<std::thread, TThreadCount>>::type;

	/// Type definition for the operation storage queue.
	/// If TSize is 0, std::queue will be used (dynamic memory mode). Otherwise a
	/// StaticFunctionQueue of size TSize is used.
	using TQueueType = typename std::conditional<(TSize == 0), std::queue<TFunc>,
												 embutil::StaticFunctionQueue<TSize>>::type;

  public:
	/// Public alias for the dispatch functor type.
	using DispatchFunc_t = TFunc;

	/** Create an unnamed dispatch queue.
	 *
	 * Constucts a dispatch queue with a generic name.
	 * @param thread_count Optional parameter that determines the number of threads associated with
	 * 	this queue. thread_count cannot exceed TThreadCount.
	 */
	explicit DispatchQueue_Base(size_t thread_count = 1) noexcept : name_("GenericDispatchQueue")
	{
		if constexpr(TThreadCount == 0)
		{
			threads_.reserve(thread_count);
		}

		for(size_t i = 0; i < threads_.capacity(); i++)
		{
			threads_.emplace_back(&DispatchQueue_Base::dispatch_thread_handler, this);
		}
	}

	/** Create a dispatch queue with a C-string name.
	 *
	 * @param thread_count Optional parameter that determines the number of threads associated with
	 * 	this queue. thread_count cannot exceed TThreadCount.
	 * @param name The name of the dispatch queue.
	 */
	explicit DispatchQueue_Base(const char* name, size_t thread_count = 1) noexcept : name_(name)
	{
		if constexpr(TThreadCount == 0)
		{
			threads_.reserve(thread_count);
		}

		for(size_t i = 0; i < threads_.capacity(); i++)
		{
			threads_.emplace_back(&DispatchQueue_Base::dispatch_thread_handler, this);
		}
	}

	/** Create a dispatch queue with a std::string name.
	 *
	 * @param name The name of the dispatch queue.
	 * 	@note DispatchQueue_Base() uses a std::string_view, so a std::string input must remain
	 * valid.
	 * @param thread_count Optional parameter that determines the number of threads associated with
	 * 	this queue. thread_count cannot exceed TThreadCount.
	 */
	explicit DispatchQueue_Base(const std::string name, size_t thread_count = 1) noexcept
		: name_(std::move(name))
	{
		if constexpr(TThreadCount == 0)
		{
			threads_.reserve(thread_count);
		}

		for(size_t i = 0; i < threads_.capacity(); i++)
		{
			threads_.emplace_back(&DispatchQueue_Base::dispatch_thread_handler, this);
		}
	}

	/** Create a dispatch queue with a std::string name.
	 *
	 * @param name The name of the dispatch queue.
	 * 	@note DispatchQueue_Base() uses a std::string_view, so the original std::string must
	 *	remain valid.
	 * @param thread_count Optional parameter that determines the number of threads associated with
	 * 	this queue. thread_count cannot exceed TThreadCount.
	 */
	explicit DispatchQueue_Base(const std::string_view name, size_t thread_count = 1) noexcept
		: name_(std::move(name))
	{
		if constexpr(TThreadCount == 0)
		{
			threads_.reserve(thread_count);
		}

		for(size_t i = 0; i < threads_.capacity(); i++)
		{
			threads_.emplace_back(&DispatchQueue_Base::dispatch_thread_handler, this);
		}
	}

	/** Default destructor.
	 *
	 * The destructor notifies dispatch threads that it is time to quit. We must wait for threads
	 * to exit before destroying the class to prevent race conditions and memory faults.
	 * Once all threads are joined, the object is destructed.
	 *
	 * This function is marked noexcept because we want the program to terminate if an exception
	 * results from this call.
	 */
	~DispatchQueue_Base() noexcept
	{
		// Signal to dispatch threads that it's time to wrap up
		std::unique_lock<TLock> lock(lock_);
		quit_ = true;
		lock.unlock();
		cv_.notify_all();

		// Wait for threads to finish before we exit
		for(size_t i = 0; i < threads_.size(); i++)
		{
			if(threads_[i].joinable())
			{
				threads_[i].join();
			}
		}
	}

	/// Deleted copy constructor
	DispatchQueue_Base(const DispatchQueue_Base&) = delete;

	/// Deleted copy assignment operator
	const DispatchQueue_Base& operator=(const DispatchQueue_Base&) = delete;

	/// Deleted move constructor
	DispatchQueue_Base(DispatchQueue_Base&&) = delete;

	/// Deleted move assignment operator
	DispatchQueue_Base& operator=(DispatchQueue_Base&&) = delete;

	/** Dispatch an operation
	 *
	 * This call adds the operation to the function queue and notifies worker threads that new
	 * work is available.
	 *
	 * The dispatch call returns immediately. Processing happens asynchronously whenever a worker
	 * thread is available for processing the operation.
	 *
	 * This function is marked noexcept because we want the program to terminate if an exception
	 * results from this call. It's possible that the queue might throw on push(), for example.
	 *
	 * @param op The operation to dispatch to a worker thread.
	 */
	void dispatch(const TFunc& op) noexcept
	{
		if constexpr(TSize > 0)
		{
			assert(q_.size() < q_.capacity() &&
				   "Max dispatch operations reached - increase DispatchQueue_Base::TSize\n");
		}

		std::unique_lock<TLock> lock(lock_);
		q_.push(std::move(op));

		// Manual unlocking is done before notifying, to avoid waking up
		// the waiting thread only to block again (see notify_one for details)
		lock.unlock();
		cv_.notify_one();
	}

	/// @overload void dispatch(const TFunc& op)
	void dispatch(TFunc&& op) noexcept
	{
		if constexpr(TSize > 0)
		{
			assert(q_.size() < q_.capacity() &&
				   "Max dispatch operations reached - increase DispatchQueue_Base::TSize\n");
		}

		std::unique_lock<TLock> lock(lock_);
		q_.push(op);

		// Manual unlocking is done before notifying, to avoid waking up
		// the waiting thread only to block again (see notify_one for details)
		lock.unlock();
		cv_.notify_one();
	}

	/** Get a std::bind object for the instance's dispatch(const&) function.
	 *
	 * If you need to get the dispatch(const&) variant for another class, use this function
	 * to avoid making your code messy with an insane std::bind call.
	 *
	 * @returns std::bind construct which will map to the the dispatch(const&) function
	 *	for this queue instance.
	 */
	auto getBoundDispatch() noexcept
	{
		return std::bind(static_cast<void (DispatchQueue_Base::*)(const DispatchFunc_t&)>(
							 &DispatchQueue_Base::dispatch),
						 this, std::placeholders::_1);
	}

	/** Get a std::bind object for the instance's dispatch(&&) function.
	 *
	 * If you need to get the dispatch(&&) variant for another class, use this function
	 * to avoid making your code messy with a stupid std::bind call.
	 *
	 * @returns std::bind construct which will map to the the dispatch(&&) function
	 *	for this queue instance.
	 */
	auto getBoundMoveDispatch() noexcept
	{
		return std::bind(static_cast<void (DispatchQueue_Base::*)(DispatchFunc_t &&)>(
							 &DispatchQueue_Base::dispatch),
						 this, std::placeholders::_1);
	}

	/** Get the current size of the operation queue.
	 *
	 * @returns the current number of enqueued operations.
	 */
	size_t queue_size() const noexcept
	{
		return q_.size();
	}

	/** Get the capacity of the operation queue.
	 *
	 * @returns the capacity of the queue
	 */
	constexpr size_t capacity() const noexcept
	{
		return q_.capacity();
	}

	/** Get the number of threads used by this dispatch queue.
	 *
	 * @return the number of threads associated with this dispatch queue
	 */
	constexpr size_t thread_count() const noexcept
	{
		return threads_.size();
	}

  private:
	/// Name of the dispatch queue.
	const std::string_view name_;
	/// Instance of the lock used to protect the operation queue.
	TLock lock_;
	/// Vector of threads which handle dispatch operations.
	TVecType threads_;
	/// The operation queue instance.
	TQueueType q_;
	/// The condition variable which is used to notify dispatch threads.
	TCond cv_;
	/// Flag used to signal to threads that it is time to shutdown the queue.
	std::atomic<bool> quit_ = false;

	/** Worker thread handler.
	 *
	 * Worker threads all utilize the same operational flow:
	 *
	 * This function is marked noexcept because we want the program to terminate if an exception
	 * results from this call. We can't guarantee that ops or the queue won't have exceptions.
	 *
	 * - Threads sleep until there is work in the queue (or quit_ is set)
	 * - Whenever work is available, an operation is popped from the queue
	 *	and processed locally by the worker thread.
	 * - When there is no longer any work available, the worker thread sleeps until notified
	 */
	void dispatch_thread_handler() noexcept
	{
		std::unique_lock<TLock> lock(lock_);

		do
		{
			// Wait until we have data or a quit signal
			cv_.wait(lock, [this] { return (quit_ || q_.size()); });

			// after wait, we own the lock
			if(!quit_ && q_.size())
			{
				auto op = std::move(q_.front());
				q_.pop();

				// unlock now that we're done messing with the queue
				lock.unlock();

				if constexpr(TSize > 0)
				{
					op->exec();
				}
				else
				{
					op();
				}

				lock.lock();
			}
		} while(!quit_);

		lock.unlock();
	}
};

/** Dispatch queue class supporting dynamic sizes and dynamic memory allocation.
 *
 * This dispatch queue contains a dynamicly sized queue and configurable thread count.
 * The function queue is fully serial, so each operation will execute in order.
 *
 * To use this queue as a serial queue, use a thread count of 1.
 * With multiple threads, operations will be executed as soon as a thread is
 * available.
 *
 * @note This queue is only safe to call from contexts which can aquire mutexes.
 * @note To instantiate with default TFunc type, use `DynamicDispatchQueue<>`.
 *
 * @tparam TFunc The type representing a dispatch function prototype. Defaults to any functor
 *	which can be represented by `void(void)`.
 * @tparam TLock Type to use for the lock. Can be overriden if a custom mutex/lock should be used.
 * @tparam TCond Type to use for the condition variable. Can be overridden if a custom condition
 *	variable implementation should be used.
 */
template<typename TFunc = std::function<void()>, typename TLock = std::mutex,
		 typename TCond = std::condition_variable>
using DynamicDispatchQueue = DispatchQueue_Base<0, 0, TFunc, TLock, TCond>;

/** Dispatch queue specializatoin using only static memory allocation.
 *
 * This dispatch queue type uses a staticly sized queue and configurable thread count.
 * The function queue is fully serial, so each operation will execute in order.
 *
 * To use this queue as a serial queue, use a thread count of 1.
 * With multiple threads, operations will be executed as soon as a thread is
 * available.
 *
 * TSize = size of the underlying queue in bytes
 *
 * @note This queue is only safe to call from contexts which can aquire mutexes.
 *
 * @tparam TSize The size of the storage queue. When TSize is 0, dynamic memory allocation will be
 *	used. Otherwise static memory types are used and the maximum number of operations is limited to
 *	TSize.
 * @tparam TThreadCount The number of threads to use with the dispatch queue.
 * @tparam TFunc The type representing a dispatch function prototype. Defaults to any functor
 *	which can be represented by `void(void)`.
 * @tparam TLock Type to use for the lock. Can be overriden if a custom mutex/lock should be used.
 * @tparam TCond Type to use for the condition variable. Can be overridden if a custom condition
 *	variable implementation should be used.
 */
template<const size_t TSize, const size_t TThreadCount = 1,
		 typename TFunc = stdext::inplace_function<void()>, typename TLock = std::mutex,
		 typename TCond = std::condition_variable>
using StaticDispatchQueue = DispatchQueue_Base<TSize, TThreadCount, TFunc, TLock, TCond>;

/// @}
// End DispatchQueue

} // namespace embutil

#endif // DISPATCH_HPP_
