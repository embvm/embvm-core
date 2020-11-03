// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#ifndef PTHREAD_MUTEX_HPP_
#define PTHREAD_MUTEX_HPP_

#include <cassert>
#include <cerrno>
#include <pthread.h>
#include <rtos/mutex.hpp>

namespace os::posix
{
/** POSIX Mutex Implementation.
 *
 * Useful reference documentation:
 *	- posix mutex API: https://docs.oracle.com/cd/E19455-01/806-5257/6je9h032o/index.html
 * 	- posix mutex attributes: https://docs.oracle.com/cd/E19683-01/806-6867/6jfpgdcnf/index.html
 *
 * @ingroup POSIXOS
 */
class Mutex final : public embvm::VirtualMutex
{
  public:
	/** Construct a POSIX mutex
	 *
	 * @param type The mutex type to create (normal, recursive)
	 * @param mode The mutex poperating mode, which controls priority inheritance behaviors.
	 */
	explicit Mutex(embvm::mutex::type type = embvm::mutex::type::defaultType,
				   embvm::mutex::mode mode = embvm::mutex::mode::defaultMode) noexcept
	{
		pthread_mutexattr_t attributes;

		int r = pthread_mutexattr_init(&attributes);
		assert(r == 0);

		int pthread_type;
		switch(type)
		{
			case embvm::mutex::type::normal:
				pthread_type = PTHREAD_MUTEX_NORMAL;
				break;
			case embvm::mutex::type::recursive:
			default:
				pthread_type = PTHREAD_MUTEX_RECURSIVE;
				break;
		}

		r = pthread_mutexattr_settype(&attributes, pthread_type);
		assert(r == 0);

		int protocol;
		switch(mode)
		{
			case embvm::mutex::mode::none:
				// A thread's priority and scheduling are not affected by the mutex ownership.
				protocol = PTHREAD_PRIO_NONE;
				break;
			case embvm::mutex::mode::protect:
				protocol = PTHREAD_PRIO_PROTECT;
				break;
			case embvm::mutex::mode::priorityInherit:
			default:
				protocol = PTHREAD_PRIO_INHERIT;
				break;
		}

		r = pthread_mutexattr_setprotocol(&attributes, protocol);
		assert(r == 0);

		r = pthread_mutex_init(&handle_, nullptr);
		assert(r == 0);

		r = pthread_mutexattr_destroy(&attributes);
		assert(r == 0);
	}

	/// Default destructor
	~Mutex() noexcept;

	void lock() noexcept final
	{
		int r = pthread_mutex_lock(&handle_);
		assert(r == 0);
	}

	void unlock() noexcept final
	{
		int r = pthread_mutex_unlock(&handle_);
		assert(r == 0);
	}

	bool trylock() noexcept final
	{
		int r = pthread_mutex_trylock(&handle_);
		assert(r == 0 || r == EBUSY);

		// Success means we actually locked it
		return (r == 0);
	}

	embvm::mutex::handle_t native_handle() const noexcept final
	{
		return reinterpret_cast<embvm::mutex::handle_t>(&handle_);
	}

  private:
	/// The pthread handle associated with this mutex;
	pthread_mutex_t handle_{};
};

} // namespace os::posix

#endif // PTHREAD_MUTEX_HPP_
