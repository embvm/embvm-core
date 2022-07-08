// Copyright 2022 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#ifndef PTHREAD_SEMAPHORE_HPP_
#define PTHREAD_SEMAPHORE_HPP_

#include <cassert>
#include <rtos/semaphore.hpp>
#include <semaphore.h>

namespace os::posix
{
/// @addtogroup POSIXOS
/// @{

// TODO: test on Linux
/** Create a POSIX semaphore (OSX variant)
 *
 * Useful reference documentation:
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

/// @}

} // namespace os::posix

#endif // PTHREAD_SEMAPHORE_HPP_
