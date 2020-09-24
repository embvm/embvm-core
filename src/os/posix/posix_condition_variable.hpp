#ifndef POSIX_CONDITION_VARIABLE_HPP_
#define POSIX_CONDITION_VARIABLE_HPP_

#include "posix_mutex.hpp"
#include <cassert>
#include <rtos/condition_variable.hpp>

namespace os::posix
{
class ConditionVariable final : public embvm::VirtualConditionVariable
{
  public:
	ConditionVariable() = default;

	~ConditionVariable() noexcept
	{
		auto r = pthread_cond_destroy(&handle_);
		assert(r == 0);
	}

	bool wait(embvm::VirtualMutex* mutex) noexcept final
	{
		auto mutex_handle = reinterpret_cast<Mutex*>(mutex)->native_handle();
		auto r = pthread_cond_wait(&handle_, reinterpret_cast<pthread_mutex_t*>(mutex_handle));
		assert(r == 0);

		return r == 0;
	}

	bool wait(embvm::VirtualMutex* mutex, const embvm::os_timeout_t& timeout) noexcept final
	{
		if(timeout == embvm::OS_WAIT_FOREVER)
		{
			return wait(mutex);
		}
		else
		{
			auto t_abs = std::chrono::time_point_cast<std::chrono::nanoseconds>(
				std::chrono::system_clock::now());
			t_abs += timeout;

			auto ts = embutil::timepointToTimespec(t_abs);

			return timed_wait(mutex, ts);
		}
	}

	bool timed_wait(embvm::VirtualMutex* mutex, timespec timeout) noexcept
	{
		auto mutex_handle = reinterpret_cast<Mutex*>(mutex)->native_handle();
		return 0 == pthread_cond_timedwait(
						&handle_, reinterpret_cast<pthread_mutex_t*>(mutex_handle), &timeout);
	}

	void signal() noexcept final
	{
		auto r = pthread_cond_signal(&handle_);
		assert(r == 0);
	}

	void broadcast() noexcept final
	{
		auto r = pthread_cond_broadcast(&handle_);
		assert(r == 0);
	}

	embvm::cv::handle_t native_handle() const noexcept final
	{
		return reinterpret_cast<embvm::cv::handle_t>(&handle_);
	}

  private:
	pthread_cond_t handle_ = PTHREAD_COND_INITIALIZER;
};

} // namespace os::posix

#endif // POSIX_CONDITION_VARIABLE_HPP_
