#ifndef THREAD_HPP_
#define THREAD_HPP_

#include "rtos_defs.hpp"
#include <string_view>

// TODO: How do we account for time slicing?
// TODO: suspend/resume

namespace embvm
{
/// @addtogroup FrameworkOS
/// @{

/** Defines the VirtualThread interface.
 *
 * VirtualThread defines the common thread interfaces for a VirtualRTOS. RTOS implementations
 * should derive from this base class.
 *
 * The factory is responsible for creating the thread, so the constructor is not defined
 * in this interface class.
 *
 */
class VirtualThread
{
  public:
	/// Deleted copy constructor
	VirtualThread(const VirtualThread&) = delete;

	/// Deleted copy assignment operator
	const VirtualThread& operator=(const VirtualThread&) = delete;

	/// Default move constructor
	VirtualThread(VirtualThread&&) = delete;

	/// Default move assignment operator
	VirtualThread& operator=(VirtualThread&&) = delete;

	/** Get the thread state.
	 *
	 * @returns The state of the thread.
	 */
	virtual embvm::thread::state state() const noexcept = 0;

	/// Start the thread, if it is not auto-started
	virtual void start() noexcept = 0;

	/** Terminate a running thread.
	 *
	 * @pre The thread is running.
	 * @post The thread is aborted.
	 */
	virtual void terminate() noexcept = 0;

	/** View the thread's name
	 *
	 * Storage for the thread name is implementation defined. For example, the name may be stored in
	 * the RTOS memory, or it may require internal object storage.
	 *
	 * @returns The thread object's name.
	 */
	[[nodiscard]] virtual std::string_view name() const noexcept = 0;

	/** Join a running thread.
	 *
	 * Joins a running thread, blocking the calling thread until the joined thread completes.
	 *
	 * @pre The thread has not exited.
	 * @post The thread has exited.
	 */
	virtual void join() noexcept = 0;

	/** Get the native handle for the thread
	 *
	 * If custom extensions or behaviors are needed, the thread handle can be retrieved and
	 * manipulated directly.
	 *
	 * @returns the native thread object handle as a `void*`. The user must cast appropriately.
	 */
	[[nodiscard]] virtual embvm::thread::handle_t native_handle() const noexcept = 0;

  protected:
	VirtualThread() = default;

	/// Derived classes should ensure their destructor deletes the OS task
	virtual ~VirtualThread() noexcept;
};

/** Functions that operate on the currently active thread
 *
 * These static functions must be implemented for each OS so the linker has
 * an active definition. If the function is not supported, you should call
 * assert() or abort().
 */
namespace this_thread
{
/// Sleep the currently running thread for a set amount of time
/// The thread will sleep for *at least* the specified time, but may sleep longer
/// depending on the scheduler.
/// @param delay The number of milliseconds to sleep the thread for
void sleep_for(const embvm::os_timeout_t& delay) noexcept;

/// Yields execution of the current thread so other threads can run.
void yield() noexcept;

/** Get the native handle for the currently active thread.
 *
 * If custom extensions or behaviors are needed, the thread handle can be retrieved and
 * manipulated directly.
 *
 * @returns the native thread object handle as a `void*`. The user must cast appropriately.
 */
embvm::thread::handle_t get_handle() noexcept;
} // namespace this_thread

/// @}
// End Group

} // namespace embvm

#endif // THREAD_HPP_
