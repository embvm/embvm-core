#ifndef VIRTUAL_PLATFORM_DISPATCHER_HPP_
#define VIRTUAL_PLATFORM_DISPATCHER_HPP_

#include "virtual_platform.hpp"
#include <dispatch/dispatch.hpp>

namespace embvm
{
/** Add a dispatch queue to the VirtualPlatform through inheritance.
 *
 * Your platform can inherit from this class to add dispatching APIs a the platform level.
 *	This template is specialized based on the class of Dispatch Queue you want to create.
 *
 * Example declaration:
 *	```
 * 	using PlatformDispatchQueue = embutil::DynamicDispatchQueue<>;
 *
 *	class UnitTestPlatform : public VirtualPlatform, public
 *		PlatformDispatcher<PlatformDispatchQueue>
 *	{...}
 *	```
 *
 * @tparam TDispatchQueue The type declaration for the underlying dispatch queue.
 *	This is user specified because each platform will have its own specific dispatch queue
 *	configuration.
 *
 * @ingroup FrameworkPlatform
 */
template<class TDispatchQueue>
class PlatformDispatcher
{
	using TFunc = typename TDispatchQueue::DispatchFunc_t;

  public:
	/** Construct a platform dispatcher.
	 *
	 * @param threads The number of threads the dispatch queue will use.
	 */
	explicit PlatformDispatcher(size_t threads = 1) noexcept
		: dispatch_queue_("Platform Dispatch Queue", threads)
	{
	}

	/** Construct a platform dispatcher with a C-string name.
	 *
	 * @param name The name for the dispatch queue.
	 * @param threads The number of threads the dispatch queue will use.
	 */
	explicit PlatformDispatcher(const char* name, size_t threads = 1) noexcept
		: dispatch_queue_(name, threads)
	{
	}

	/** Construct a platform dispatcher with a C-string name.
	 *
	 * @param name The name for the dispatch queue.
	 * @param threads The number of threads the dispatch queue will use
	 */
	explicit PlatformDispatcher(const std::string& name, size_t threads = 1) noexcept
		: dispatch_queue_(name, threads)
	{
	}

	/** Construct a platform dispatcher with a std::string_views name.
	 *
	 * @param name The name for the dispatch queue.
	 * @param threads The number of threads the dispatch queue will use
	 */
	explicit PlatformDispatcher(const std::string_view& name, size_t threads = 1) noexcept
		: dispatch_queue_(name, threads)
	{
	}

	/** Get std::bind object corresponding dispatch(const&) function.
	 *
	 * If you need to get the function dispatch(const&) variant for use with another class,
	 * (e.g., passing dispatch() as a callback), use this function.
	 *
	 * @returns A `std::bind` object corresponding to the dispatch(const&) function.
	 */
	auto getBoundDispatch() noexcept
	{
		return dispatch_queue_.getBoundDispatch();
	}

	/** Get std::bind object corresponding dispatch(&&) function.
	 *
	 * If you need to get the function dispatch(&&) variant for use with another class,
	 * (e.g., passing dispatch() as a callback), use this function.
	 *
	 * @returns A `std::bind` object corresponding to the dispatch(&&) function.
	 */
	auto getBoundMoveDispatch() noexcept
	{
		return dispatch_queue_.getBoundMoveDispatch();
	}

	/** Dispatch an operation.
	 *
	 * Dispatch an operation to the queue. Forwards the function object to the underlying
	 * dispatch queue.
	 *
	 * @param op The function object containing the operation that will be dispatched
	 * 	to the queue.
	 */
	void dispatch(const TFunc& op) noexcept
	{
		dispatch_queue_.dispatch(op);
	}

	/** Dispatch an operation.
	 *
	 * Dispatch an operation to the queue. Forwards the function object to the underlying
	 * dispatch queue.
	 *
	 * @param op The function object containing the operation that will be dispatched
	 * 	to the queue.
	 */
	void dispatch(TFunc&& op) noexcept
	{
		dispatch_queue_.dispatch(std::move(op));
	}

  protected:
	TDispatchQueue dispatch_queue_;
};

} // namespace embvm

#endif // VIRTUAL_PLATFORM_DISPATCHER_HPP_
