// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#ifndef PTHREAD_THREAD_HPP_
#define PTHREAD_THREAD_HPP_

#include <atomic>
#include <bits/bits.hpp>
#include <bounce/bounce.hpp>
#include <pthread.h>
#include <rtos/thread.hpp>
#include <string_view>

/*
 * Useful reference documentation:
 *	- posix thread Attributes: https://docs.oracle.com/cd/E19455-01/806-5257/6je9h032j/index.html
 */

#ifndef PTHREAD_STACK_MIN
#define PTHREAD_STACK_MIN 16384
#endif

namespace os::posix
{
/// @addtogroup POSIXOS
/// @{

/// PThread priority values
/// @note For PThreads: higher number = higher priority
enum class pthread_priority : uint8_t
{
	panic = 99,
	interrupt = 99,
	realtime = 99,
	veryHigh = 50,
	high = 40,
	aboveNormal = 30,
	normal = 20,
	belowNormal = 10,
	low = 5,
	lowest = 1,
	idle = 0
};

/// Convert between framework thread priorities and PThread priorities
constexpr posix::pthread_priority convert_to_pthread_priority(embvm::thread::priority p) noexcept
{
	switch(p)
	{
		case embvm::thread::priority::panic:
			return posix::pthread_priority::panic;
		case embvm::thread::priority::interrupt:
			return posix::pthread_priority::interrupt;
		case embvm::thread::priority::realtime:
			return posix::pthread_priority::realtime;
		case embvm::thread::priority::veryHigh:
			return posix::pthread_priority::veryHigh;
		case embvm::thread::priority::high:
			return posix::pthread_priority::high;
		case embvm::thread::priority::aboveNormal:
			return posix::pthread_priority::aboveNormal;
		case embvm::thread::priority::normal:
			return posix::pthread_priority::normal;
		case embvm::thread::priority::belowNormal:
			return posix::pthread_priority::belowNormal;
		case embvm::thread::priority::low:
			return posix::pthread_priority::low;
		case embvm::thread::priority::lowest:
			return posix::pthread_priority::lowest;
		case embvm::thread::priority::idle:
			return posix::pthread_priority::idle;
		default:
			// Invalid priority value
			assert(0);
			return posix::pthread_priority::normal;
	}
}

// TODO: expand creation for numerical thread priority value? just in case?

/// Create a POSIX thread
class Thread final : public embvm::VirtualThread
{
  public:
	/** Construct a POSIX thread
	 *
	 * @param name The name associated with the mutex.
	 *	@note A std::string input must remain valid for the lifetime of this object, since
	 * 	std::string_view is used to store the name.
	 * @param func The thread function to execute; can be any functor type.
	 * @param arg The thread's optional input argument. This value is passed to the thread
	 * 	when it is created.
	 * @param p The thread priority setting.
	 * @param stack_size The thread stack size.
	 * @param stack_ptr The thread stack pointer. If stack_ptr is nullptr, then memory
	 * 	will be allocated by the pthread library.
	 */
	explicit Thread(std::string_view name, embvm::thread::func_t func, embvm::thread::input_t arg,
					embvm::thread::priority p = embvm::thread::priority::normal,
					size_t stack_size = PTHREAD_STACK_MIN, void* stack_ptr = nullptr) noexcept
		: name_(name), arg_(arg), func_(func)
	{
		pthread_attr_t attributes;

		// PThread requires stack alignment, this alignment was empirically determined
		assert(stack_ptr == NULL || embutil::is_aligned(stack_ptr, PTHREAD_STACK_MIN / 2));
		assert(func != nullptr);

		int r = pthread_attr_init(&attributes);
		assert(r == 0);

		if(stack_size < PTHREAD_STACK_MIN)
		{
			stack_size = PTHREAD_STACK_MIN;
		}

		if(stack_ptr)
		{
			r = pthread_attr_setstack(&attributes, stack_ptr, stack_size);
			assert(r == 0);
		}
		else
		{
			r = pthread_attr_setstacksize(&attributes, stack_size);
			assert(r == 0);
		}

		// Puts the thread into the Real-time (RT) scheduling class
		r = pthread_attr_setscope(&attributes, PTHREAD_SCOPE_SYSTEM);
		assert(r == 0);

		// These commands will certainly work in linux, but if you don't run
		// the application as `sudo`, you won't be able to set the scheduling policy
		// and pthread_create will fail with EPERM (1)
#ifndef __linux__
		/*
		 * Options are:
		 *	SCHED_FIFO: run to completion if not preempted by higher priority (Realtime)
		 *	SCHED_RR: if not preempted by priority, threads execute for a time period determined
		 *		by the system
		 *		SCHED_RR should be used for time slicing
		 *	SCHED_OTHER: time-sharing, default value
		 */
		r = pthread_attr_setschedpolicy(&attributes, SCHED_FIFO);
		assert(r == 0);

		r = pthread_attr_setinheritsched(&attributes, PTHREAD_EXPLICIT_SCHED);
		assert(r == 0);

		sched_param schedule;
		r = pthread_attr_getschedparam(&attributes, &schedule);
		assert(r == 0);

		auto priority = convert_to_pthread_priority(p);
		schedule.sched_priority = static_cast<int>(priority);
		r = pthread_attr_setschedparam(&attributes, &schedule);
		assert(r == 0);
#endif

		state_ = embvm::thread::state::ready;

		// Our interfaces demand a void(*)(void*), so we adapt to posix requirement
		// and simply skip out on the return to the caller's perspective
		r = pthread_create(&handle_, &attributes,
						   reinterpret_cast<void* (*)(void*)>(BOUNCE(Thread, thread_func)),
						   reinterpret_cast<void*>(this));
		assert(r == 0);

		r = pthread_attr_destroy(&attributes);
		assert(r == 0);
	}

	/// Default destructor, cleans up thread on deletion.
	~Thread() noexcept;

	// posix auto-starts threads
	void start() noexcept final {}

	void terminate() noexcept final
	{
		int r = pthread_cancel(handle_);
		assert(r == 0);
	}

	void join() noexcept final
	{
		int r = pthread_join(handle_, nullptr);
		assert(r != EDEADLK && r != EINVAL);

		// ESRCH can indicate that the thread has completed - let join() complete
	}

	int detach() noexcept
	{
		return pthread_detach(handle_);
	}

	std::string_view name() const noexcept final
	{
		return name_;
	}

	embvm::thread::state state() const noexcept final
	{
		return state_;
	}

	embvm::thread::handle_t native_handle() const noexcept final
	{
		return reinterpret_cast<embvm::thread::handle_t>(handle_);
	}

  private:
	/* Termination Cleanup Function
	 *
	 * This function is used to indicate that a thread has been terminated
	 * by changing the thread state once Pthread handles the request.
	 */
	void terminate_func() noexcept
	{
		state_ = embvm::thread::state::terminated;
	}

	/** Thread handling wrapper to support thread canceling.
	 *
	 * We wrap the thread to enable canceling, and so we can gracefully tell if
	 * the thread has completed successfully
	 */
	void* thread_func() noexcept
	{
		// Register a cleanup handler that will be called if the thread is terminated
		pthread_cleanup_push(reinterpret_cast<void (*)(void*)>(BOUNCE(Thread, terminate_func)),
							 reinterpret_cast<void*>(this));

		// Enable threads to be cancelled at any time
		int r = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, nullptr);
		assert(r == 0);
		r = pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, nullptr);
		assert(r == 0);

		state_ = embvm::thread::state::executing;

		func_(arg_);

		state_ = embvm::thread::state::completed;

		// 0 = do not execute cleanup handler, which should only be invoked
		// in the event that the thread is terminated
		pthread_cleanup_pop(0);

		return nullptr;
	}

  private:
	/// THe name of this thread instance.
	std::string_view name_;

	/// The pthread handle corresponding to this thread instance.
	pthread_t handle_;

	/// Thread input arguments.
	embvm::thread::input_t arg_;

	/// The functor which will be invoked by the thread
	embvm::thread::func_t func_;

	std::atomic<embvm::thread::state> state_ = embvm::thread::state::ready;
};

/// @}

} // namespace os::posix

#endif // PTHREAD_THREAD_HPP_
