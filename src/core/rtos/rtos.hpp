#ifndef RTOS_HPP_
#define RTOS_HPP_

#include "condition_variable.hpp"
#include "event_flag.hpp"
#include "msg_queue.hpp"
#include "mutex.hpp"
#include "semaphore.hpp"
#include "thread.hpp"
#include <cassert>

#ifdef FRAMEWORK_DEFAULT_STACK_SIZE
inline constexpr size_t DEFAULT_STACK_SIZE = FRAMEWORK_DEFAULT_STACK_SIZE;
#else
inline constexpr size_t DEFAULT_STACK_SIZE = 4096; // bytes
#endif

namespace embvm
{
/// @addtogroup FrameworkOS
/// @{

#pragma mark - Factory Classes -

/** Factory interface for creating RTOS-related objects.
 *
 * This calls provides the virtual interface for creating RTOS objects.
 *
 * We use the curiously recurring template pattern (CRTP) for this factory.
 * To create a new factory for your OS, derive in the following way:
 *	@code
 *	class newOS : public VirtualOSFactory<newOS>
 *	@endcode
 *
 * The derived class must implement the `_impl()` versions of the functions defined here.
 *
 * @tparam TFactory Derived factory class which implements the functions as required by the
 *	target RTOS.
 */
template<typename TFactory>
class VirtualOSFactory
{
  public:
	/** Create a virtual condition variable.
	 * Creates a condition variable using the underlying platform RTOS. A generic
	 * embvm::VirtualConditionVariable pointer is returned, masking the underlying implementation
	 * from higher layers.
	 *
	 * The caller owns the returnd pointer. It must be returned with the factory destroy() function.
	 *
	 * @returns a embvm::VirtualConditionVariable pointer
	 */
	static embvm::VirtualConditionVariable* createConditionVariable() noexcept
	{
		return TFactory::createConditionVariable_impl();
	}

	/** Create a virtual thread.
	 *
	 * Creates a thread using the underlying platform RTOS. A generic embvm::VirtualThread pointer
	 *is returned, masking the underlying implementation from higher layers.
	 *
	 * The caller owns the returnd pointer. It must be returned with the factory destroy() function.
	 *
	 * @pre f is not nullptr
	 * @post A thread object is created and the thread is started.
	 *
	 * @param name The name of the thread.
	 * @param f The thread function which will be invoked. f must not be a nullptr.
	 * @param input Optional input value; nullptr can be specified for unused input.
	 * @param p Priority for the thread.
	 * @param stack_size the size of the thread's stack.
	 * @param stack_ptr Pointer to the thread's stack.
	 *			nullptr input means dynamic memory will be used to allocate the stack.
	 *
	 * @returns a embvm::VirtualThread pointer
	 */
	static embvm::VirtualThread*
		createThread(std::string_view name, embvm::thread::func_t f,
					 embvm::thread::input_t input = nullptr,
					 embvm::thread::priority p = embvm::thread::priority::normal,
					 size_t stack_size = DEFAULT_STACK_SIZE, void* stack_ptr = nullptr) noexcept
	{
		assert(f != nullptr); // We need a thread function - block nullptr

		return TFactory::createThread_impl(name, f, input, p, stack_size, stack_ptr);
	}

	/** Create a virtual Mutex.
	 *
	 * Creates a virtual mutex using the underlying RTOS. A generic embvm::VirtualMutex pointer is
	 * returned, masking the underlying implementation from higher layers.
	 *
	 * The caller owns the returnd pointer. It must be returned with the factory destroy() function.
	 *
	 * @param type The type of mutex to create.
	 * @param mode The mutex's operational mode.
	 * @returns a embvm::VirtualMutex pointer.
	 */
	static embvm::VirtualMutex*
		createMutex(embvm::mutex::type type = embvm::mutex::type::defaultType,
					embvm::mutex::mode mode = embvm::mutex::mode::defaultMode) noexcept
	{
		return TFactory::createMutex_impl(type, mode);
	}

	/** Create a virtual binary semaphore.
	 *
	 * Creates a virtual binary semaphore using the underlying RTOS. A generic
	 * embvm::VirtualSemaphore pointer is returned, masking the underlying implementation from
	 * higher layers.
	 *
	 * A binary semaphore is limited to a count of 1.
	 *
	 * The caller owns the returnd pointer. It must be returned with the factory destroy() function.
	 *
	 * @returns a embvm::VirtualSemaphore pointer
	 */
	static embvm::VirtualSemaphore* createBinarySemaphore() noexcept
	{
		return TFactory::createSemaphore_impl(embvm::semaphore::mode::binary, 1, 0);
	}

	/** Create a virtual counting semaphore.
	 *
	 * Creates a virtual counting semaphore using the underlying RTOS. A generic
	 * embvm::VirtualSemaphore pointer is returned, masking the underlying implementation from
	 * higher layers.
	 *
	 * The counting semaphore supports counts > 1.
	 *
	 * The caller owns the returnd pointer. It must be returned with the factory destroy() function.
	 *
	 * @param ceiling The maximum count the semaphore supports.
	 * @param initial_count Sets the initial count of the semaphore variable.
	 *		Initial-count enables support for starting the semaphore with resources in use.
	 *		Negative value indicates default setting - match ceiling
	 * @returns a embvm::VirtualSemaphore pointer.
	 */
	static embvm::VirtualSemaphore*
		createSemaphore(embvm::semaphore::count_t ceiling,
						embvm::semaphore::count_t initial_count = -1) noexcept
	{
		return TFactory::createSemaphore_impl(embvm::semaphore::mode::counting, ceiling,
											  initial_count);
	}

	/** Create a virtual message queue.
	 *
	 * Creates a virtual message queue using the underlying RTOS. The type of data stored in the
	 * message queue is determined using a template parameter. A generic embvm::VirtualMessageQueue
	 * pointer is returned, masking the underlying implementation from higher layers.
	 *
	 * The caller owns the returnd pointer. It must be returned with the factory destroy() function.
	 *
	 * @tparam TType The type of the underlying data to be stored.
	 * @param queue_length The maximum number of elements to store in the queue.
	 * @returns a embvm::VirtualMessageQueue<TType> pointer.
	 */
	template<typename TType>
	static embvm::VirtualMessageQueue<TType>* createMessageQueue(size_t queue_length) noexcept
	{
		return TFactory::template createMessageQueue_impl<TType>(queue_length);
	}

	/** Create a virtual event flag.
	 *
	 * Creates a virtual event flag using the underlying RTOS. A generic embvm::VirtualEventFlag
	 * pointer is returned, masking the underlying implementation from higher layers.
	 *
	 * The caller owns the returnd pointer. It must be returned with the factory destroy() function.
	 *
	 * @returns a embvm::VirtualEventFlag pointer.
	 */
	static embvm::VirtualEventFlag* createEventFlag() noexcept
	{
		return TFactory::createEventFlag_impl();
	}

	static void destroy(embvm::VirtualThread* item) noexcept
	{
		return TFactory::destroy_impl(item);
	}

	static void destroy(embvm::VirtualMutex* item) noexcept
	{
		return TFactory::destroy_impl(item);
	}

	static void destroy(embvm::VirtualSemaphore* item) noexcept
	{
		return TFactory::destroy_impl(item);
	}

	static void destroy(embvm::VirtualEventFlag* item) noexcept
	{
		return TFactory::destroy_impl(item);
	}

	static void destroy(embvm::VirtualConditionVariable* item) noexcept
	{
		return TFactory::destroy_impl(item);
	}

#if 0 // TODO: message queue, once it's generic
	static void destroy(embvm::VirtualEventFlag* item)
	{
		return TFactory::destroy_impl(item);
	}
#endif

  public:
	VirtualOSFactory() = default;
	~VirtualOSFactory() = default;

	// We want to call private _impl() functions in the derived class.
	friend TFactory;
};

/// @}
// End Group

} // namespace embvm

#endif // RTOS_HPP_
