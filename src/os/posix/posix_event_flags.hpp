#ifndef PTHREAD_EVENT_FLAGS_HPP_
#define PTHREAD_EVENT_FLAGS_HPP_

#include "posix_condition_variable.hpp"
#include "posix_mutex.hpp"
#include <atomic>
#include <pthread.h>
#include <rtos/event_flag.hpp>
#include <string>
#include <time/time.hpp>

namespace os::posix
{
/** POSIX Event Flag Implementation.
 *
 * @ingroup POSIXOS
 */
class EventFlag final : public embvm::VirtualEventFlag
{
  public:
	EventFlag() = default;

	/// Default destructor which cleans up the event flag group.
	~EventFlag() noexcept;

	embvm::eventflag::flag_t
		get(embvm::eventflag::flag_t bits_wait,
			embvm::eventflag::option opt = embvm::eventflag::option::OR, bool clearOnExit = true,
			const embvm::os_timeout_t& timeout = embvm::OS_WAIT_FOREVER) noexcept final
	{
		// We start wait_success as true in case flags is 0: we want to enter the loop
		bool wait_success = true;

		mutex_.lock();

		auto flags = checkAndClearFlags(bits_wait, opt, clearOnExit);

		while(wait_success == true && flags == 0)
		{
			wait_success = cv_.wait(&mutex_, timeout);

			if(wait_success == true)
			{
				flags = checkAndClearFlags(bits_wait, opt, clearOnExit);
			}
		}

		mutex_.unlock();

		return flags;
	}

	void set(embvm::eventflag::flag_t bits) noexcept final
	{
		mutex_.lock();
		flags_ |= bits;
		mutex_.unlock();

		cv_.broadcast();
	}

	void setFromISR(embvm::eventflag::flag_t bits) noexcept final
	{
		mutex_.lock();
		flags_ |= bits;
		mutex_.unlock();

		cv_.broadcast();
	}

	void clear() noexcept final
	{
		mutex_.lock();
		flags_ = 0;
		mutex_.unlock();
	}

	embvm::eventflag::handle_t native_handle() const noexcept final
	{
		return 0;
	}

  private:
	/** Helper function which evaluates flags and returns the matching value.
	 *
	 * @param bits_wait The bits the caller is waiting for, specified as a bitmask
	 * @param opt The comparison option (AND or OR)
	 * @param clearOnExit If true, matching flags will be cleared from flag_ when the function exits
	 * @returns the resulting event flag bits, compared to bits_wait and with the selected option
	 *(opt).
	 * @post If clearOnExit is true:
	 *	- All flags set in bits_wait will be cleared from flag_ if AND option is used.
	 * 	- Flags set in both btis_wait and flags_ will be cleared from flag_ if OR option is used.
	 */
	embvm::eventflag::flag_t checkAndClearFlags(embvm::eventflag::flag_t bits_wait,
												embvm::eventflag::option opt,
												bool clearOnExit) noexcept
	{
		embvm::eventflag::flag_t return_flags = 0;
		auto flags_copy = flags_.load();

		if(opt == embvm::eventflag::option::OR)
		{
			return_flags = flags_copy & bits_wait;
		}
		else if(bits_wait == (flags_copy & bits_wait))
		{
			return_flags = bits_wait;
		}

		if(return_flags && clearOnExit)
		{
			flags_ &= ~(return_flags);
		}

		return return_flags;
	}

  private:
	/// The storage type for the event flag group's flags.
	std::atomic<embvm::eventflag::flag_t> flags_ = 0;

	/// Condition variable which is used to wake threads waiting on this group.
	posix::ConditionVariable cv_;

	/// Mutex which protects the flags.
	posix::Mutex mutex_;
};

} // namespace os::posix

#endif // PTHREAD_EVENT_FLAGS_HPP_
