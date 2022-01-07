// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#ifndef TIMER_MANAGER_HPP_
#define TIMER_MANAGER_HPP_

#include <cassert>
#include <driver/timer.hpp>
#include <etl/list.h>
#include <etl/vector.h>
#include <inplace_function/inplace_function.hpp>
#include <limits>
#include <list>
#include <nop_lock/nop_lock.hpp>
#include <vector>

namespace embvm
{
/** Timer Manager Class
 *
 * The Timer Manager takes a hardware timer and uses it to produce a
 * number of software timers.
 *
 * An alternative structure accomplishing this task is the `callback_timer`
 * provided by ETL in `callback_timer.h`, which uses a `tick()` function:
 *		https://www.etlcpp.com/callback_timer.html
 *
 * @tparam TMaxTimers The maximum number of software timers that can be created.
 *	Size 0 indicates dynamic memory will be used. All other sizes will enable
 *	static memory allocation.
 * @tparam TLock The type of lock interface to use. Default is embutil::nop_lock, which disables
 *	locking. To enable locking, declare TimerManager with a functional lock type.
 *	@code
 *	using TimerManager_t = embvm::TimerManager<0, std::mutex>;
 *	@endcode
 * @tparam TTimeUnit The time-keeping units. The default `embvm::timer::timer_period_t` is inherited
 *	from the Timer base class. This can be overriden to provide a specific timer resolution
 *	(e.g. 1ms instead of 1us). TTimeUnit is limited by the granularity that the Timer can support.
 * @tparam TTimeoutCallback The storage type for the callback function.
 * @tparam TTimerDevice The type of timer device that this class will manage.
 *	The default Timer type is the base class that framework timers derive from.
 *
 * @ingroup FrameworkHwPlatform
 */
template<const size_t TMaxTimers = 0, typename TLock = embutil::nop_lock,
		 typename TTimeUnit = embvm::timer::timer_period_t,
		 typename TTimeoutCallback = stdext::inplace_function<void()>,
		 typename TTimerDevice = embvm::timer::Timer>
class TimerManager
{
  public:
	/// @brief Consumers interact with the software timer through the TimerHandle class
	class TimerHandle;

	/// @brief The function prototype for the Dispatcher.
	/// The dispacher uses inplace_function regardless of static or dynamic memory allocation.
	using DispatcherFunc = stdext::inplace_function<void(const TTimeoutCallback&)>;

  private:
	/// @brief Alias for the time unit representation (e.g., uint32_t, uint64_t)
	using TimeRep_t = typename TTimeUnit::rep;

	/// The timer handle can call private TimerManager functions
	friend class TimerManager::TimerHandle;

	/** Timer Delay Information
	 *
	 * The delayInfo struct is used to track information for a given time request.
	 */
	struct delayInfo
	{
		delayInfo() noexcept
			: config(embvm::timer::config::oneshot), current_time(0), target_time(0), cb(),
			  wait_in_progress(false)
		{
			// empty body
		}

		delayInfo(const delayInfo& rhs) noexcept
			: config(rhs.config), current_time(rhs.current_time), target_time(rhs.target_time),
			  cb(rhs.cb), wait_in_progress(rhs.wait_in_progress)
		{
			// empty body
		}

		delayInfo(delayInfo&& rhs) noexcept
			: config(std::move(rhs.config)),
			  target_time(std::move(rhs.target_time)), cb(std::move(rhs.cb)),
			  wait_in_progress(std::move(rhs.wait_in_progress))
		{
			current_time = rhs.current_time;
			rhs.wait_in_progress = false;
		}

		/// @brief The requested timer configuration
		embvm::timer::config config;

		/// @brief The adjusted delay time.
		/// This field allows allowing for delay adjustments due to other timer requests
		TimeRep_t current_time;

		/// @brief The original delay time
		/// This value is stored particularly for periodic timers
		TimeRep_t target_time;

		/// @brief The timer callback function.
		/// When the timer expires, this callback is called
		TTimeoutCallback cb;

		/// @brief Indicates whether the timer is currently running
		bool wait_in_progress;
	};

	/** Type definition for the underlying timer queue.
	 *
	 * This type is used to manage timers which have been allocated.
	 *
	 * The type changes depending on whether static or dynamic memory allocation is being used
	 * (dynamic memory is indicated by `TMaxTimers == 0`)
	 */
	using TTimerQueueType = typename std::conditional<(TMaxTimers == 0), std::list<delayInfo>,
													  etl::list<delayInfo, TMaxTimers>>::type;

	/// @brief Convenience alias for the queue handle type.
	/// A list iterator is used because iterators remain valid even with insertion/deletions
	using TQueueHandle = typename TTimerQueueType::iterator;

	/** Convenience type for the queue of scheduled timers.
	 *
	 * This queue is used to manage the list of currently scheduled timers.
	 *
	 * The size of the scheduled queue will be <= the size of the allocated queue.
	 *
	 * The type changes depending on whether static or dynamic memory allocation is being used
	 * (dynamic memory is indicated by `TMaxTimers == 0`)
	 */
	using TScheduledQueueType =
		typename std::conditional<(TMaxTimers == 0), std::vector<TQueueHandle>,
								  etl::vector<TQueueHandle, TMaxTimers>>::type;

	/** Comparator for sorting scheduled timers
	 *
	 * This comparison operator is used when inserting new timers into the scheduled list.
	 * Timers must be sorted so that they execute in proper timeout order.
	 */
	struct scheduleQueueCompare
	{
		bool operator()(const TQueueHandle& lhs, const TQueueHandle& rhs) noexcept
		{
			return rhs->current_time < lhs->current_time;
		}
	};

  public:
	/** Create a TimerManager without a dispatcher.
	 *
	 * Creating a TimerManager without a dispatcher will result in the manager invoking the
	 * functions.
	 *
	 * When no dispatcher is used, timer callbacks are called directly by the TimerManager.
	 *
	 * @param timer The timer hardware instance which the TimerManager will manage.
	 */
	explicit TimerManager(TTimerDevice& timer) noexcept
		: dispatcher_(std::bind(&TimerManager::TimerManagerDispatch, this, std::placeholders::_1)),
		  timer_hw_(timer)
	{
		timer_hw_.registerCallback(std::bind(&TimerManager::TimerInterruptHandler, this));
		timer.config(embvm::timer::config::oneshot);
	}

	/** Create a TimerManager with a dispatcher.
	 *
	 * Creates a TimerManager class with an external dispatcher. When a timer expires, the
	 * TimerManager forwards the callback to the dispatcher, rather than executing the callbacks
	 *directly. Using a dispatcher improves timer latency, as the manager is focused only on
	 *managing timers.
	 *
	 * @param timer The timer hardware instance which the TimerManager will manage.
	 * @param dispatcher Specifies a function which will dispatch all timer callbacks.
	 *	 Use TimerManager with a dispatch queue, or other type of system event queue.
	 */
	explicit TimerManager(TTimerDevice& timer, const DispatcherFunc& dispatcher) noexcept
		: dispatcher_(dispatcher), timer_hw_(timer)
	{
		timer_hw_.registerCallback(std::bind(&TimerManager::TimerInterruptHandler, this));
		timer.config(embvm::timer::config::oneshot);
	}

	/** Destroy the TimerManager
	 *
	 * On destruction, the TimerManager stops the underlying timer hardware and unregisters
	 * the TimerManager's timer interrupt callback.
	 */
	~TimerManager() noexcept
	{
		scheduled_q_lock_.lock();
		timer_hw_.stop();
		timer_hw_.registerCallback(nullptr);
		scheduled_q_lock_.unlock();
	}

	/// Deleted copy constructor
	TimerManager(const TimerManager&) = delete;

	/// Deleted copy assignment operator
	const TimerManager& operator=(const TimerManager&) = delete;

	/// Deleted move constructor
	TimerManager(TimerManager&&) = delete;

	/// Deleted move assignment operator
	TimerManager& operator=(TimerManager&&) = delete;

	/** Allocate a new software timer
	 *
	 * Allocates a new timer in the TimerManager and returns a handle to the user.
	 *
	 * @returns A handle to the allocated software timer. The handle is used by the consumer
	 * 	to configure the timer. The TimerHandle's lifetime controls the timer's lifetime - once
	 *	the TimerHandle leaves scope, the timer will be automatically unregistered.
	 */
	TimerHandle allocate() noexcept
	{
		TimerHandle handle(this);

		if constexpr(TMaxTimers > 0) // NOLINT
		{
			assert(timer_list_.size() < timer_list_.max_size());
		}

		timer_list_lock_.lock(); // NOLINT
		timer_list_.push_back(delayInfo());
		timer_list_lock_.unlock();

		// We emplaced our element at the back, so decrement 1 from end() to get the iterator
		auto h = timer_list_.end();
		handle.handle_ = --h;

		return handle;
	}

  private:
	/** TimerManager Default Dispatch Function.
	 *
	 * If no external dispatcher is provided, TimerManager has one which
	 * just executes the operation by default.
	 *
	 * @param op The callback function object to execute.
	 */
	void TimerManagerDispatch(const TTimeoutCallback& op) noexcept
	{
		op();
	}

	/** TimerManager's Timer Interrupt Handler
	 *
	 * TimerManager registers its own interrupt with the Timer hardware driver.
	 * When the timer interrupt fires, we account for the time that has elapsed,
	 * clear all scheduled timers, and restart the timer hardware with the next
	 * timeout request.
	 *
	 * Note that this must work with an interrupt bottom-half handler because locking
	 * calls are used here. It cannot be called directly in an ISR context!
	 */
	void TimerInterruptHandler() noexcept
	{
		auto count = timer_hw_.count();

		adjustScheduledTimerCount(count);
		clearExpiredTimers();
		startNextTimer();
	}

	/**
	 *
	 */
	void addToScheduledQueue(TQueueHandle handle) noexcept
	{
		// If we are already waiting, we need to update the existing request, but
		// we don't add to the heap
		if(!handle->wait_in_progress)
		{
			if constexpr(TMaxTimers > 0)
			{
				assert(scheduled_queue_.size() < scheduled_queue_.capacity());
			}

			scheduled_q_lock_.lock();
			scheduled_queue_.push_back(handle);
			scheduled_q_lock_.unlock();

			handle->wait_in_progress = true;
		}

		sortScheduledQueue();
	}

	void popScheduledQueueFront() noexcept
	{
		// Move to end
		std::pop_heap(scheduled_queue_.begin(), scheduled_queue_.end(), scheduleQueueCompare());
		// and remove the last element
		scheduled_queue_.pop_back();
	}

	void lockAndPopScheduledQueueFront() noexcept
	{
		scheduled_q_lock_.lock();
		popScheduledQueueFront();
		scheduled_q_lock_.unlock();
	}

	void adjustScheduledTimerCount(TTimeUnit time_base) noexcept
	{
		scheduled_q_lock_.lock();
		for(auto& s : scheduled_queue_)
		{
			if(time_base.count() <= s->current_time)
			{
				s->current_time -= time_base.count();
			}
			else
			{
				s->current_time = 0;
			}
		}
		scheduled_q_lock_.unlock();
	}

	void startNextTimer() noexcept
	{
		if(!scheduled_queue_.empty())
		{
			scheduled_q_lock_.lock();
			timer_hw_.restart(scheduled_queue_[0]->current_time);
			scheduled_q_lock_.unlock();
		}
	}

	auto stopRunningTimer() noexcept
	{
		scheduled_q_lock_.lock();
		timer_hw_.stop();
		auto time_base = timer_hw_.count();
		scheduled_q_lock_.unlock();

		return time_base;
	}

	void schedule(TQueueHandle handle, TTimeUnit delay, const TTimeoutCallback& func,
				  embvm::timer::config config) noexcept
	{
		auto time_base = stopRunningTimer();

		adjustScheduledTimerCount(time_base);

		handle->config = config;
		handle->target_time = delay.count();
		handle->current_time = delay.count();
		handle->cb = func;

		addToScheduledQueue(handle);
		clearExpiredTimers();
		startNextTimer();
	}

	void schedule(TQueueHandle handle, TTimeUnit delay, TTimeoutCallback&& func,
				  embvm::timer::config config) noexcept
	{
		auto time_base = stopRunningTimer();

		adjustScheduledTimerCount(time_base);

		handle->config = config;
		handle->target_time = delay.count();
		handle->current_time = delay.count();
		handle->cb = std::move(func);

		addToScheduledQueue(handle);
		clearExpiredTimers();
		startNextTimer();
	}

	/// Returns false if no wait was scheduled or if the callback was already called
	/// Returns true if cancelled successfully
	bool cancel(TQueueHandle handle) noexcept
	{
		bool canceled = false;

		if(handle->wait_in_progress)
		{
			// Check if the timer is currently running
			if(handle == scheduled_queue_.front())
			{
				// Stop the timer and update the counts for the structures
				auto count = stopRunningTimer();
				adjustScheduledTimerCount(count);

				// Remove the element from the heap and start the next timer
				lockAndPopScheduledQueueFront();
				if(!scheduled_queue_.empty())
				{
					startNextTimer();
				}
			}
			else
			{
				// Remove from list
				scheduled_q_lock_.lock();
				auto it = std::find(scheduled_queue_.begin(), scheduled_queue_.end(), handle);
				assert(it != scheduled_queue_.end());
				scheduled_queue_.erase(it);
				scheduled_q_lock_.unlock();
			}
			canceled = true;
		}

		return canceled;
	}

	void deleteTimer(TQueueHandle handle) noexcept
	{
		timer_list_lock_.lock();
		timer_list_.erase(handle);
		timer_list_lock_.unlock();
	}

	void sortScheduledQueue() noexcept
	{
		scheduled_q_lock_.lock();
		std::make_heap(scheduled_queue_.begin(), scheduled_queue_.end(), scheduleQueueCompare());
		scheduled_q_lock_.unlock();
	}

	void clearExpiredTimers() noexcept
	{
		int expired_count = 0;

		scheduled_q_lock_.lock();

		for(auto& entry : scheduled_queue_)
		{
			if(entry->wait_in_progress && entry->current_time == 0)
			{
				auto callback = entry->cb;

				// Oneshot timers are removed - periodic timers stay in the list
				if(entry->config == embvm::timer::config::periodic)
				{
					entry->current_time = entry->target_time;
				}
				else
				{
					entry->wait_in_progress = false;
					expired_count++;
				}

				if(callback)
				{
					dispatcher_(callback);
				}
			}
			else
			{
				break;
			}
		}

		while(expired_count > 0)
		{
			popScheduledQueueFront();
			expired_count--;
		}

		scheduled_q_lock_.unlock();
	}

  private:
	// TODO: should this be a reference? Also need to update event center
	const DispatcherFunc dispatcher_;
	TTimerQueueType timer_list_{};
	TScheduledQueueType scheduled_queue_{};
	TLock scheduled_q_lock_;
	TLock timer_list_lock_;
	TTimerDevice& timer_hw_;
};

#pragma mark - Timer Manager Handle -

/** Handle to a software timer
 *
 * TimerHandle represents a handle to an event registration. TimerHandle are created
 * during the TimerManager::allocate() process.
 *
 * The handle is used to schedule delays:
 *	@code
 *	auto h = tm.allocate();
 *	h.asyncDelay(std::chrono::milliseconds(1), cb_called);
 *  @endcode
 *
 * The lifetime of the TimerHandle controls the lifetime of the software timer.
 * If the handle leaves scope, the timer will be automatically unregistered.
 *
 * Template parameters simply match those in the original TimerManager declaration, since
 * TimerHandle is a class member.
 */
template<const size_t TMaxTimers, typename TLock, typename TTimeUnit, typename TTimeoutCallback,
		 typename TTimerDevice>
class TimerManager<TMaxTimers, TLock, TTimeUnit, TTimeoutCallback, TTimerDevice>::TimerHandle
{
	friend class TimerManager<TMaxTimers, TLock, TTimeUnit, TTimeoutCallback, TTimerDevice>;

  public:
	/// Default constructor which creates an invalid object
	TimerHandle() noexcept : mgr_(nullptr) {}

	/// Destroying the TimerHandle also deletes it from the TimerManager's timer allocation queue
	~TimerHandle() noexcept
	{
		destroy();
	}

	/// @brief Move Constructor
	TimerHandle(TimerHandle&& rhs) noexcept : mgr_(rhs.mgr_), handle_(rhs.handle_)
	{
		rhs.mgr_ = nullptr;
		rhs.handle_ = mgr_->timer_list_.end();
	}

	/// @brief Move assignment operator
	TimerHandle& operator=(TimerHandle&& rhs) noexcept
	{
		std::swap(mgr_, rhs.mgr_);
		std::swap(handle_, rhs.handle_);
		rhs.destroy();

		return *this;
	}

	/// Delete the copy constructor
	TimerHandle(const TimerHandle&) = delete;

	/// Delete the copy assignment operator
	const TimerHandle& operator=(const TimerHandle&) = delete;

	/** Check if the TimerHandle is valid.
	 *
	 * @returns true if the TimerHandle is valid, false otherwise
	 */
	bool valid() const noexcept
	{
		return (mgr_ != nullptr && handle_ != mgr_->timer_list_.end());
	}

	/** Operator bool checks validity.
	 *
	 * Operator bool() is used to check that a TimerHandle is valid.
	 *
	 * @returns true if the TimerHandle is valid, false otherwise
	 */
	operator bool() const noexcept
	{
		return valid();
	}

	/** Destroy the TimerHandle
	 *
	 * If the TimerHandle is invalid (i.e. already destroyed), the request will be
	 * ignored.
	 */
	void destroy() noexcept
	{
		if(valid())
		{
			mgr_->deleteTimer(handle_);
			handle_ = mgr_->timer_list_.end();
		}
	}

	/** Cancel a software timer
	 *
	 * Request cancellation for a software timer. The cancellation request is best-effort,
	 * and the timer may expire before the request is completed.
	 *
	 * @pre The TimerHandle is valid
	 * @post The timer is cancelled, or has expired before cancellation was carried out.
	 *
	 * @returns True if the timer was cancelled, false if the timer expired before cancellation.
	 */
	bool cancel() noexcept
	{
		assert(valid());
		return mgr_->cancel(handle_);
	}

	/** Configure a one-shot delay
	 *
	 * Configure the software timer to perform a one-shot asynchronous delay. When the timer
	 *expires, the callback will be called. The timer will be removed from the scheduled queue.
	 *
	 * @pre The TimerHandle is valid.
	 * @post The periodic delay is scheduled.
	 *
	 * The following template parameters should be automatically deduced by the compiler:
	 *
	 * @tparam TRep Underlying storage type (representation) for the time units (e.g., uint32_t,
	 *uint64_t)
	 * @tparam TPeriod A std::ratio representing the tick period (e.g., std::nano)
	 *
	 * @param[in] delay std::chrono::duration representing the periodic timeout value delta.
	 *	For example:
	 *	@code
	 *	std::chrono::duration<uint64_t, std::nano> delay = std::chrono::nanoseconds(1000);
	 *	@endcode
	 * @param[in] func Function object that will be registered as the software timer callback.
	 */
	template<typename TRep, typename TPeriod>
	void asyncDelay(const std::chrono::duration<TRep, TPeriod>& delay,
					const TTimeoutCallback& func) noexcept
	{
		assert(valid());
		auto convertedDelay = std::chrono::duration_cast<TTimeUnit>(delay);

		mgr_->schedule(handle_, convertedDelay, func, embvm::timer::config::oneshot);
	}

	/** Configure a one-shot delay
	 *
	 * Configure the software timer to perform a one-shot asynchronous delay. When the timer
	 *expires, the callback will be called. The timer will be removed from the scheduled queue.
	 *
	 * @pre The TimerHandle is valid.
	 * @post The periodic delay is scheduled.
	 *
	 * The following template parameters should be automatically deduced by the compiler:
	 *
	 * @tparam TRep Underlying storage type (representation) for the time units (e.g., uint32_t,
	 *uint64_t)
	 * @tparam TPeriod A std::ratio representing the tick period (e.g., std::nano)
	 *
	 * @param[in] delay std::chrono::duration representing the periodic timeout value delta.
	 *	For example:
	 *	@code
	 *	std::chrono::duration<uint64_t, std::nano> delay = std::chrono::nanoseconds(1000);
	 *	@endcode
	 * @param[in] func R-value function object that will be registered as the software timer
	 *callback.
	 */
	template<typename TRep, typename TPeriod>
	void asyncDelay(const std::chrono::duration<TRep, TPeriod>& delay,
					TTimeoutCallback&& func) noexcept
	{
		assert(valid());
		auto convertedDelay = std::chrono::duration_cast<TTimeUnit>(delay);

		mgr_->schedule(handle_, convertedDelay, std::move(func), embvm::timer::config::oneshot);
	}

	/** Configure a periodic delay
	 *
	 * Configure the software timer to perform a periodic delay. When the timer expires, the
	 *callback will be called adn the timer will be automatically re-added to the scheduled timer
	 *queue.
	 *
	 * @pre The TimerHandle is valid.
	 * @post The periodic delay is scheduled.
	 *
	 * The following template parameters should be automatically deduced by the compiler:
	 *
	 * @tparam TRep Underlying storage type (representation) for the time units (e.g., uint32_t,
	 *uint64_t)
	 * @tparam TPeriod A std::ratio representing the tick period (e.g., std::nano)
	 *
	 * @param[in] delay std::chrono::duration representing the periodic timeout value delta.
	 *	For example:
	 *	@code
	 *	std::chrono::duration<uint64_t, std::nano> delay = std::chrono::nanoseconds(1000);
	 *	@endcode
	 * @param[in] func Function object that will be registered as the software timer callback.
	 */
	template<typename TRep, typename TPeriod>
	void periodicDelay(const std::chrono::duration<TRep, TPeriod>& delay,
					   const TTimeoutCallback& func) noexcept
	{
		assert(valid());
		auto convertedDelay = std::chrono::duration_cast<TTimeUnit>(delay);

		mgr_->schedule(handle_, convertedDelay, func, embvm::timer::config::periodic);
	}

	/** Configure a periodic delay
	 *
	 * Configure the software timer to perform a periodic delay. When the timer expires, the
	 *callback will be called adn the timer will be automatically re-added to the scheduled timer
	 *queue.
	 *
	 * @pre The TimerHandle is valid.
	 * @post The periodic delay is scheduled.
	 *
	 * The following template parameters should be automatically deduced by the compiler:
	 *
	 * @tparam TRep Underlying storage type (representation) for the time units (e.g., uint32_t,
	 *uint64_t)
	 * @tparam TPeriod A std::ratio representing the tick period (e.g., std::nano)
	 *
	 * @param[in] delay std::chrono::duration representing the periodic timeout value delta.
	 *	For example:
	 *	@code
	 *	std::chrono::duration<uint64_t, std::nano> delay = std::chrono::nanoseconds(1000);
	 *	@endcode
	 * @param[in] func R-value function object that will be registered as the software timer
	 *callback.
	 */
	template<typename TRep, typename TPeriod>
	void periodicDelay(const std::chrono::duration<TRep, TPeriod>& delay,
					   TTimeoutCallback&& func) noexcept
	{
		assert(valid());
		auto convertedDelay = std::chrono::duration_cast<TTimeUnit>(delay);

		mgr_->schedule(handle_, convertedDelay, std::move(func), embvm::timer::config::periodic);
	}

  private:
	/// Private constructor, used by TimerManager::allocate() to create new TimerHandle instances
	explicit TimerHandle(TimerManager* mgr) noexcept : mgr_(mgr) {}

  private:
	/// @brief The owning TimerManager
	TimerManager* mgr_;

	/// @brief The handle for the delayInfo structure which corresponds with this timer
	TQueueHandle handle_;
};

} // namespace embvm

#endif // TIMER_MANAGER_HPP_
