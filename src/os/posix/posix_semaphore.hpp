// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#ifndef PTHREAD_SEMAPHORE_HPP_
#define PTHREAD_SEMAPHORE_HPP_

#include <rtos/semaphore.hpp>
#ifdef __APPLE__
#include <atomic>
#include <dispatch/dispatch.h>
#else
#include <semaphore.h>
#endif
#include <cassert>

namespace os::posix
{
/// @addtogroup POSIXOS
/// @{

#ifdef __APPLE__

/** Create a POSIX semaphore (OSX variant)
 *
 * Useful reference documenation:
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
	 * resources are in use at hte itme of creation.
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
	~Semaphore() noexcept;

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

	bool take(const embvm::os_timeout_t& timeout = embvm::OS_WAIT_FOREVER) noexcept final
	{
		dispatch_time_t t_converted;

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

	embvm::semaphore::count_t count() const noexcept final
	{
		return count_;
	}

	embvm::semaphore::handle_t native_handle() const noexcept final
	{
		return reinterpret_cast<embvm::semaphore::handle_t>(&handle_);
	}

  private:
	/// The handle to the sempahore
	dispatch_semaphore_t handle_;

	/// Current semaphore count
	std::atomic<embvm::semaphore::count_t> count_;

	/// The semaphore ceiling
	embvm::semaphore::count_t ceiling_;
};
#else
// TODO: test on Linux
/** Create a POSIX semaphore (OSX variant)
 *
 * Useful reference documenation:
 *	- POSIX semaphore APIs:
 *		https://docs.oracle.com/cd/E19120-01/open.solaris/817-4415/svipc-75667/index.html
 */
class Semaphore final : public embvm::VirtualSemaphore
{
  public:
	explicit Semaphore(embvm::semaphore::mode mode = embvm::semaphore::mode::counting,
					   embvm::semaphore::count_t ceiling = 1,
					   embvm::semaphore::count_t initial_count = -1) noexcept
	{
		if(mode == embvm::semaphore::mode::binary)
		{
			// binary: force max of 1
			ceiling = 1;

			if(initial_count > ceiling)
			{
				initial_count = 1;
			}
		}

		int r = sem_init(&handle_, false, ceiling);
		assert(r == 0);

		if(initial_count >= 0)
		{
			for(unsigned i = (ceiling - initial_count); i > 0; i--)
			{
				take();
			}
		}
	}

	~Semaphore();

	void give() noexcept final
	{
		int r = sem_post(&handle_);
		assert(r == 0);
	}

	void giveFromISR() noexcept final
	{
		int r = sem_post(&handle_);
		assert(r == 0);
	}

	bool take(const embvm::os_timeout_t& timeout = embvm::OS_WAIT_FOREVER) noexcept final
	{
		assert(timeout == embvm::OS_WAIT_FOREVER && "Semaphore doesn't support timed waits\n");

		int r = sem_wait(&handle_);
		assert(r == 0);

		return (r == 0);
	}

	embvm::semaphore::count_t count() const noexcept final
	{
		int count;
		int r = sem_getvalue(const_cast<sem_t*>(&handle_), &count);
		assert(r == 0);

		return static_cast<embvm::semaphore::count_t>(count);
	}

	embvm::semaphore::handle_t native_handle() const noexcept final
	{
		return reinterpret_cast<embvm::semaphore::handle_t>(&handle_);
	}

  private:
	sem_t handle_;
};
#endif

/// @}

} // namespace os::posix

#endif // PTHREAD_SEMAPHORE_HPP_
