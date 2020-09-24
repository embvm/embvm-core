#ifndef INTERRUPT_CONDITION_HPP_
#define INTERRUPT_CONDITION_HPP_

#include <inplace_function/inplace_function.hpp>

namespace embutil
{
/** Generic interrupt condition concept
 *
 * Provides a condition-variable class for use on an embedded system
 * which does not have threading, but wants to use interrupts for notifications
 * (e.g. with ARM's `WFI` instruction).
 *`
 * If we think about interrupt and non-interrupt execution modes as two threads,
 * locking in a non-interrupt thread is equivalent to disabling interrupts;
 * waiting for condition variable to be notified is the equivalent for waiting for interrupts.
 *
 * Clients supply one or two operations:
 *
 * - wait(), which puts the processor in a mode to wait for interrupts (e.g., `wfi`)
 * - notify(), which pulls the processor out of the wait mode
 *
 * @note Some scenarios, like `wfi`, do not require a function to be called. Instead, the `wfi`
 * state exit is equivalent to notify(). Clients do not need to supply the notify operation in
 * such scenarios.
 *
 * This class is designed to be used with an InterruptLock.
 *
 * @tparam TWaitOp Represents the storage type (std::function, stdext::inplace_function)
 *	 and prototype for the wait operation.
 * @tparam TNotifyOp Represents the storage type (std::function, stdext::inplace_function)
 *	 and prototype for the notify operation.
 * @related InterruptLock
 */
template<typename TWaitOp = stdext::inplace_function<void()>,
		 typename TNotifyOp = stdext::inplace_function<void()>>
class InterruptCondition
{
  public:
	/** Construct an InterruptCondition with a wait op
	 *
	 * @param[in] w The wait operation functor.
	 */
	explicit InterruptCondition(const TWaitOp& w) noexcept : wait_(w) {}

	/** Construct an InterruptCondition with a wait op and notify op.
	 *
	 * @param[in] w The wait operation functor.
	 * @param[in] n The notify operation functor.
	 */
	explicit InterruptCondition(const TWaitOp& w, const TNotifyOp& n) noexcept
		: wait_(w), notify_(n)
	{
	}

	/// Deleted copy constructor
	InterruptCondition(const InterruptCondition&) = delete;

	/// Deleted copy assignment operator
	const InterruptCondition& operator=(const InterruptCondition&) = delete;

	/// Deleted move constructor
	InterruptCondition(InterruptCondition&&) = delete;

	/// Deleted move assignment operator
	InterruptCondition& operator=(InterruptCondition&&) = delete;

	/** Initiate a wait state
	 *
	 * Receives the reference to lockable object (has `lock()` and `unlock()` member functions) that
	 * is already locked.
	 *
	 * The behavior of this function matches std::condition_variable:
	 * - locked lock is received
	 * - unlock the lock
	 * - wait for notification to wake up
	 * - re-acquire the lock and return control to the caller
	 *
	 * This function is marked noexcept because we want the program to terminate if an exception
	 * results from this call. We don't actually know whether lock/unlock/wait_ can throw.
	 *
	 * @tparam TLock the type of the lock. The compiler deduces this template parameter.
	 * @param lock The lock instance.
	 * @pre lock is already locked.
	 * @post lock is locked.
	 */
	template<typename TLock>
	void wait(TLock& lock) noexcept
	{
		lock.unlock();
		wait_();
		lock.lock();
	}

	/** Notify the processor to wake up
	 *
	 * This function is used to notify condition that wait should
	 * be terminated. If nothing needs to be done (e.g. WFI),
	 * then client classes do not need to supply a notify function.
	 */
	void notify() const noexcept
	{
		if(notify_)
		{
			notify_();
		}
	}

	/// Set the notify operation.
	/// @param[in] n The operation to call to notify() the processor that the wait is completed.
	void set_notify_op(TNotifyOp&& n) noexcept
	{
		notify_ = std::forward<TNotifyOp>(n);
	}

  private:
	/// (optional) notification operation functor
	TNotifyOp notify_;
	/// Wait operaton functor
	TWaitOp wait_;
};

} // namespace embutil

#endif // INTERRUPT_CONDITION_HPP_
