// Copyright 2022 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#ifndef PTHREAD_SEMAPHORE_APPLE__HPP_
#define PTHREAD_SEMAPHORE_APPLE__HPP_

#include <atomic>
#include <cassert>
#include <dispatch/dispatch.h>
#include <rtos/semaphore.hpp>

namespace os::posix
{
/// @addtogroup POSIXOS
/// @{

/** Create a POSIX semaphore (OSX variant)
 *
 * Useful reference documentation:
 *	- POSIX semaphore APIs:
 *		https://docs.oracle.com/cd/E19120-01/open.solaris/817-4415/svipc-75667/index.html
 */
class Semaphore final : public embvm::VirtualSemaphore
{
  public:
	// TODO: refactor into a sem_init() so we can have a constructor which specifices a ceiling and
	// assumes counting mode
	/** Create a POSIX sempahore
	 *
	 * @param mode The semaphore mode (binary, counting).
	 * @param ceiling The maximum count of the semaphore
	 * @param initial_count The starting count of the semaphore. Can be used to indicate that
	 * resources are in use at the time of creation.
	 */
	explicit Semaphore(embvm::semaphore::mode mode = embvm::semaphore::mode::counting,
					   embvm::semaphore::count_t ceiling = 1,
					   embvm::semaphore::count_t initial_count = -1) noexcept
		: count_(ceiling), ceiling_(ceiling)
	{
		if(mode == embvm::semaphore::mode::binary)
		{
			// binary: force max of 1
			ceiling_ = 1;

			if(initial_count > ceiling_)
			{
				initial_count = 1;
			}
		}

		handle_ = dispatch_semaphore_create(ceiling_);

		if(initial_count >= 0)
		{
			for(int i = (ceiling_ - initial_count); i > 0; i--)
			{
				take();
			}
		}
	}

	/// Default destructor, cleans up after semaphore on destruction.
	~Semaphore() noexcept override;

	void give() noexcept final
	{
		dispatch_semaphore_signal(handle_);

		if(count_ < ceiling_)
		{
			count_++;
		}
	}

	void giveFromISR() noexcept final
	{
		dispatch_semaphore_signal(handle_);
		if(count_ < ceiling_)
		{
			count_++;
		}
	}

	auto take(const embvm::os_timeout_t& timeout = embvm::OS_WAIT_FOREVER) noexcept -> bool final
	{
		dispatch_time_t t_converted = 0;

		if(timeout == embvm::OS_WAIT_FOREVER)
		{
			t_converted = DISPATCH_TIME_FOREVER;
		}
		else
		{
			t_converted = dispatch_time(
				DISPATCH_TIME_NOW,
				std::chrono::duration_cast<std::chrono::nanoseconds>(timeout).count());
		}

		auto r = dispatch_semaphore_wait(handle_, t_converted);

		if(r == 0)
		{
			count_--;
		}

		return r == 0;
	}

	[[nodiscard]] auto count() const noexcept -> embvm::semaphore::count_t final
	{
		return count_;
	}

	[[nodiscard]] auto native_handle() const noexcept -> embvm::semaphore::handle_t final
	{
		return reinterpret_cast<embvm::semaphore::handle_t>(&handle_);
	}

  private:
	/// The handle to the sempahore
	dispatch_semaphore_t handle_;

	/// Current semaphore count
	std::atomic<embvm::semaphore::count_t> count_{};

	/// The semaphore ceiling
	embvm::semaphore::count_t ceiling_;
};

/// @}

} // namespace os::posix

#endif // PTHREAD_SEMAPHORE_APPLE__HPP_
