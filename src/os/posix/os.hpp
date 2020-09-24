#ifndef PTHREAD_OS_HPP_
#define PTHREAD_OS_HPP_

#include "posix_condition_variable.hpp"
#include "posix_event_flags.hpp"
#include "posix_msg_queue.hpp"
#include "posix_mutex.hpp"
#include "posix_semaphore.hpp"
#include "posix_thread.hpp"
#include <rtos/rtos.hpp>

namespace os
{
namespace posix
{
/// @addtogroup POSIXOS
/// @{

/// Implementation of the POSIX OS Factory
/// For API documentation, see embvm::embvm::VirtualOSFactory
/// @related embvm::embvm::VirtualOSFactory
class posixOSFactory_impl
{
  public:
	static embvm::VirtualConditionVariable* createConditionVariable_impl() noexcept
	{
		return new posix::ConditionVariable();
	}

	static embvm::VirtualThread* createThread_impl(std::string_view name, embvm::thread::func_t f,
												   embvm::thread::input_t input,
												   embvm::thread::priority p, size_t stack_size,
												   void* stack_ptr) noexcept
	{
		return new posix::Thread(name, f, input, p, stack_size, stack_ptr);
	}

	static embvm::VirtualMutex*
		createMutex_impl(embvm::mutex::type type = embvm::mutex::type::defaultType,
						 embvm::mutex::mode mode = embvm::mutex::mode::defaultMode) noexcept
	{
		return new posix::Mutex(type, mode);
	}

	static embvm::VirtualSemaphore*
		createSemaphore_impl(embvm::semaphore::mode mode, embvm::semaphore::count_t ceiling,
							 embvm::semaphore::count_t initial_count) noexcept
	{
		return new posix::Semaphore(mode, ceiling, initial_count);
	}

	template<typename TType>
	static embvm::VirtualMessageQueue<TType>* createMessageQueue_impl(size_t queue_length) noexcept
	{
		return new posix::MessageQueue<TType>(queue_length);
	}

	static embvm::VirtualEventFlag* createEventFlag_impl() noexcept
	{
		return new posix::EventFlag();
	}

	static void destroy_impl(embvm::VirtualThread* item) noexcept
	{
		delete reinterpret_cast<Thread*>(item);
	}

	static void destroy_impl(embvm::VirtualMutex* item) noexcept
	{
		delete reinterpret_cast<Mutex*>(item);
	}

	static void destroy_impl(embvm::VirtualSemaphore* item) noexcept
	{
		delete reinterpret_cast<Semaphore*>(item);
	}

	static void destroy_impl(embvm::VirtualEventFlag* item) noexcept
	{
		delete reinterpret_cast<EventFlag*>(item);
	}

	static void destroy_impl(embvm::VirtualConditionVariable* item) noexcept
	{
		delete reinterpret_cast<ConditionVariable*>(item);
	}

  public:
	posixOSFactory_impl() = default;
	~posixOSFactory_impl() = default;
};

/// @}

} // namespace posix

/// Convenience alias for the POSIX OS Factory.
/// Use this type instea dof the verbose embvm::VirtualOSFactory definition.
using Factory = embvm::VirtualOSFactory<os::posix::posixOSFactory_impl>;

} // namespace os

#endif // PTHREAD_OS_HPP_
