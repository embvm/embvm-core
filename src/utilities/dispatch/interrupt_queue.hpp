// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#ifndef INTERRUPT_QUEUE_HPP_
#define INTERRUPT_QUEUE_HPP_

#include <cassert>
#include <etl/function.h>
#include <etl/queue.h>
#include <etl/vector.h>
#include <functional>
#include <inplace_function/inplace_function.hpp>
#include <interrupt_lock/interrupt_lock.hpp>
#include <os.hpp>
#include <rtos/event_flag.hpp>
#include <thread>

namespace embutil
{
/// @addtogroup DispatchQueue
/// @{

// TODO: fix this so the size doesn't need to be so big.
// TODO: document
using IRQBottomHalfOp_t = stdext::inplace_function<void(), 96>;
using IRQDispatcherFunc_t = stdext::inplace_function<void(const IRQBottomHalfOp_t&)>;

// TODO: update documentation (tparams, for instance)
/** IRQ safe dispatch queue (For running application-level (bottom-half) interrupt handlers)
 *
 * This dispatch queue can queue operations from a context which cannot acquire a mutex
 * The function queue is fully serial, so each operation will execute in order.
 *
 * This queue uses only static memory to avoid the potential for dynamic
 * memory allocations in an IRQ context
 *
 * This queue represents the bottom-half handler of an IRQ system.
 * To avoid operations impacting the IRQ latency, this queue forwards interrupt
 * operations to a standard dispatch queue for execution.
 *
 * TDisableOp and TEnableOp are functions which enable/disable interrupts.
 * TDisableOp must be an operation which returns a value that can be passed
 * into TEnableOp. TDisableOp's return is used to restore the proper value
 * once interrupts are re-enabled.
 *
 * @tparam TLockType The type of lock to use for protecting the queue. Must meet the requirements
 *	of a basic lockable type.
 * @tparam TSize The maximum number of operations to store in the queue.
 */
template<typename TLockType, const size_t TSize = 32>
class InterruptQueue
{
	static constexpr uint32_t WORK_READY_FLAG = (1U << 0U);
	static constexpr uint32_t QUIT_FLAG = (1U << 1U);

  public:
	explicit InterruptQueue() noexcept : flags_(os::Factory::createEventFlag())
	{
		// Initialize thread inside of the constructor body to prevent race conditions
		// With irq_lock_ not being properly initialized
		thread_ = std::thread(&InterruptQueue::dispatch_thread_handler, this);
	}

	/** Destroy an interupt queue and kill the thread
	 *
	 * This function is marked noexcept because we want the program to terminate if an exception
	 * results from this call.
	 */
	~InterruptQueue() noexcept
	{
		// Signal to dispatch threads that it's time to wrap up
		flags_->set(QUIT_FLAG);

		if(thread_.joinable())
		{
			thread_.join();
		}

		os::Factory::destroy(flags_);
		flags_ = nullptr;
	}

	/// Deleted copy constructor
	InterruptQueue(const InterruptQueue&) = delete;

	/// Deleted copy assignment operator
	auto operator=(const InterruptQueue&) -> const InterruptQueue& = delete;

	/// Deleted move constructor
	InterruptQueue(InterruptQueue&&) = delete;

	/// Deleted move assignment operator
	auto operator=(InterruptQueue&&) -> InterruptQueue& = delete;

	/** Dispatch an operation to the thread via copy
	 * Adds the operation to the queue
	 *
	 * This function is marked noexcept because we want the program to terminate if an exception
	 * results from this call. It's possible that pushing to the queue would trigger
	 * an exception depending on the underlying type, for example.
	 */
	void dispatch(const IRQBottomHalfOp_t& input_op) noexcept
	{
		irq_lock_.lock();

		if(q_.size() < q_.capacity())
		{
			q_.push(input_op);

			irq_lock_.unlock();

			flags_->setFromISR(WORK_READY_FLAG);
		}
		else
		{
			irq_lock_.unlock();
			// IRQ queue filled to capacity - increase queue size or investigate thread blockage
			assert(0);
		}
	}

	/** Dispatch an operation to the thread via move
	 * Adds the operation to the queue
	 *
	 * This function is marked noexcept because we want the program to terminate if an exception
	 * results from this call. It's possible that pushing to the queue would trigger
	 * an exception depending on the underlying type, for example.
	 */
	void dispatch(IRQBottomHalfOp_t&& input_op) noexcept
	{
		irq_lock_.lock();

		if(q_.size() < q_.capacity())
		{
			q_.push(std::move(input_op));

			irq_lock_.unlock();

			flags_->setFromISR(WORK_READY_FLAG);
		}
		else
		{
			irq_lock_.unlock();
			// IRQ queue filled to capacity - increase queue size or investigate thread blockage
			assert(0);
		}
	}

	auto getBoundDispatch() noexcept
	{
		return std::bind(static_cast<void (InterruptQueue::*)(const IRQBottomHalfOp_t&)>(
							 &InterruptQueue::dispatch),
						 this, std::placeholders::_1);
	}

	/// Return the current number of enqueued operations
	[[nodiscard]] auto queue_size() const noexcept -> size_t
	{
		return q_.size();
	}

	/// Return the capacity of the queue
	[[nodiscard]] constexpr auto capacity() const noexcept -> size_t
	{
		return q_.capacity();
	}

	/// Return the number of threads associated with the interrupt queue (always 1)
	[[nodiscard]] constexpr auto thread_count() const noexcept -> size_t
	{
		return 1;
	}

  private:
	std::thread thread_{};
	embvm::VirtualEventFlag* flags_{};
	etl::queue<IRQBottomHalfOp_t, TSize> q_{};
	TLockType irq_lock_;

	/**
	 * Convenience function which waits for the queue and pops off of it without requiring excessive
	 * locks/unlocks
	 *
	 * Using a reference parameter to receive the result is used to transfer ownership
	 * out of the queue in order to avoid the exception safety issues of returning data
	 * by-value: if the copy constructor of a by-value return throws, then the data
	 * has been removed from the queue, but is lost, whereas with this approach, the
	 * potentially problematic copy is performed prior to modifying the queue
	 * (see Herb Sutter's Guru Of The Week #8 for a discussion of the issues).4
	 *
	 *
	 * This function is marked noexcept because we want the program to terminate if an exception
	 * results from this call.
	 *
	 * @returns false if the thread should exit, true otherwise
	 */
	auto wait_and_pop(IRQBottomHalfOp_t& input_op) noexcept -> bool
	{
		while(q_.empty())
		{
			auto flags = flags_->get(WORK_READY_FLAG | QUIT_FLAG);
			bool quit = (flags & QUIT_FLAG) != 0U;

			if(quit)
			{
				return false;
			}
		}

		std::lock_guard<TLockType> lock(irq_lock_);
		input_op = std::move(q_.front());
		q_.pop();

		return true;
	}

	/** Thread handler for dispatch queue threads
	 * Sleeps until there is an operation in the thread (or quit_ is set)
	 * Processes operations from queue
	 *
	 * This function is marked noexcept because we want the program to terminate if an exception
	 * results from this call.
	 */
	void dispatch_thread_handler() noexcept
	{
		IRQBottomHalfOp_t current_op{};

		while(wait_and_pop(current_op))
		{
			if(current_op)
			{
				current_op();
			}
		};
	}
};

/// @}
// endgroup dispatch queue

} // namespace embutil

#endif // INTERRUPT_QUEUE_HPP_
