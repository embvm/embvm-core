// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#ifndef FUNCTION_QUEUE_HPP_
#define FUNCTION_QUEUE_HPP_

#include <etl/function.h>
#include <etl/largest.h>
#include <etl/pool.h>
#include <etl/queue.h>
#include <functional>
#include <inplace_function/inplace_function.hpp>
#include <memory>
#include <vector>

namespace embutil
{
inline constexpr size_t DefaultStaticQueueLargestSize = 128;

/** @defgroup FunctionQueue Function Queue
 * Queues for storing function objects.
 *
 * Users should utilize the embutil::StaticFunctionQueue or embutil::DynamicFunctionQueue template
 * specializations. Other type definitions are used to make the queue work properly.
 *
 * @ingroup FrameworkUtils
 * @{
 */

/** Base Class for storing functions.
 *
 * This base class represents a function operation stored in the function queue.
 *
 * In order to support functors of different sizes in a single statically-allocated function queue,
 * we need to provide a common interface for each size.
 *
 * A functor is stored in multiple blocks of FuncOp objects. The derived class, FuncOpBound,
 * overloads exec() to call the bound operation.
 *
 * @related StaticFunctionQueue
 */
class FuncOp
{
  public:
	/// Default destructor
	virtual ~FuncOp();

	/** Invoke the function.
	 *
	 * Base class invocation does nothing. Derived FuncOpBound classes overload this
	 * for the actual execution.
	 */
	virtual void exec() {}

	/// operator() invokes the function.
	void operator()()
	{
		exec();
	}
};

/** Represents a bound function object
 *
 * A bound function object takes in an operation of any type and stores it as a private member.
 * Since our queue is intended to support objects of different sizes, we decay to the FuncOp
 * base class interface for managing the queue.
 *
 * @tparam TFuncOp The function prototype to store in the bound object.
 * @related StaticFunctionQueue
 */
template<typename TFuncOp>
class FuncOpBound final : public FuncOp
{
  public:
	/** Construct the FuncOpBound object with an operation.
	 *
	 * @param op The operation to bind. Can be any functor.
	 */
	explicit FuncOpBound(const TFuncOp& op) : op_(op) {}

	/** Construct the FuncOpBound object with an operation.
	 *
	 * @param op The operation to bind. Can be any functor.
	 */
	explicit FuncOpBound(TFuncOp&& op) : op_(std::move(op)) {}

	/// Default destructor
	~FuncOpBound() final = default;

	/// Invoke the bound operation.
	void exec() final
	{
		op_();
	}

  private:
	/// The bound functor.
	TFuncOp op_;
};

#pragma mark - Function Queue Base -

/** Static-memory function queue that accepts functors of multiple sizes.
 *
 * This class stores function objects of varying sizes in a single queue. If you were to manage a
 * queue of stdext::inplace_function, you would have to increase the size of each element to
 * accommodate the largest possible size. This is inefficient. Instead, we support varying sizes
 * by using more complicated memory management.
 *
 * If you want a dynamic memory alternative, use `std::queue<std::function>`.
 *
 * # Using a Function Queue
 *
 * This class is intended to be used with functions of type `void(void)`. You can store functions
 * with arguments in the queue, but you must use a std::bind expression to reduce the arguments.
 *
 * Functors can be pushed to the queue using push(). This API creates a FuncOpBound object and
 * stores the element for future processing.
 *
 * @note After the function is stored in the queue, it cannot be recreated explicitly. It is only
 * accessible as a pointer to a FuncOp object.
 *
 * Elements can be  removed from the queue using front(). A std::unique_ptr is returned, ensuring
 * that the memory will be returned to the function queue's pool once the caller is done with the
 * function. This means that front() can only be called once per queue element - there is no
 *peeking. pop() can then be called to remove the element from the front of the queue.
 *
 * Invoking the function after pop() ensures two things:
 * 1. The element is removed, enabling another thread to pop off the next function immediately.
 * 2. The queue is not blocked while the function is executing.
 *
 * @note Elements returned through front() cannot be directly invoked with operator(). Instead, the
 * exec() API must be used.
 *
 * Here is an example flow for removing elements from the function queue:
 *
 * @code
 * auto op = std::move(q_.front());
 * q_.pop();
 *
 * op->exec();
 * @endcode
 *
 * If a single thread is managing the queue, popAndExec() can be used to remove the function from
 * the queue and execute it.
 *
 * @tparam TFunc The functor storage type, such as std::function or stdext::inplace_function.
 * @tparam TQueueElements The number of elements to store in the queue. TQueueElements must be > 0.
 * @tparam TLargestSize The size of the largest expected allocation, in bytes.
 *	@note The total memory pool size in bytes is `(TQueueElements * TLargestSize)`.
 */
template<const size_t TQueueElements, const size_t TLargestSize = DefaultStaticQueueLargestSize,
		 typename TFunc = stdext::inplace_function<void()>>
class StaticFunctionQueue
{
	static_assert(
		TQueueElements > 0,
		"This class does not support dynamic memory allocation. TQueueElements must be > 0");

	/// Memory storage for FuncElement types
	/// We utilize a memory pool to allocate FuncOpBound objects for storage.
	etl::generic_pool<TLargestSize, etl::largest<FuncOp, TFunc>::alignment, TQueueElements>
		mem_pool_;

	/** Return FuncOp objects to the memory pool.
	 *
	 * This deleter is used by with the std::unique_ptr that is used to store
	 * the function elements. When the FuncOp object goes out of scope, its memory
	 * is automatically returned to the pool.
	 */
	void deleter(FuncOp* ptr)
	{
		mem_pool_.template destroy<FuncOp>(ptr);
	}

	/// Convenience alias for the unique pointer to the element
	/// We return functors to the client as a std::unique_ptr<FuncOp>.
	/// Memory is allocated from mem_pool_, so we use a deleter to ensure memory is recovered.
	using UniqueElementPtr_t =
		typename std::unique_ptr<FuncOp,
								 stdext::inplace_function<void(FuncOp*), 3 * sizeof(void*)>>;

	/// Alias for our element.
	/// We store pointers to FuncOp objects.
	using ElementPtr_t = FuncOp*;

	/// Alias for the element storage queue.
	using FuncOpStorage_t = etl::queue<ElementPtr_t, TQueueElements>;

  public:
	/// Default constructor
	StaticFunctionQueue() = default;

	/// Default destructor
	~StaticFunctionQueue() = default;

	/// Deleted copy constructor
	StaticFunctionQueue(const StaticFunctionQueue&) = delete;

	/// Deleted copy assignment operator
	const StaticFunctionQueue& operator=(const StaticFunctionQueue&) = delete;

	/// Deleted move constructor
	StaticFunctionQueue(StaticFunctionQueue&&) = delete;

	/// Deleted move assignment operator
	StaticFunctionQueue& operator=(StaticFunctionQueue&&) = delete;

	/** Add a function to the queue.
	 *
	 * This function is noexcept because we want our program to abort if we are out of memory.
	 * Depending on the underlying queue type, it is certainly likely that there is throw support.
	 *
	 * @tparam TFuncOp The type of the functor to add to the queue. This template parameter is
	 * 	automatically deduced by the compiler.
	 * @param op The functor object to add to the queue.
	 */
	template<typename TFuncOp>
	void push(const TFuncOp& op) noexcept
	{
		using FuncBoundType = FuncOpBound<typename std::decay<TFuncOp>::type>;

		static_assert(std::alignment_of<FuncBoundType>::value % std::alignment_of<FuncOp>::value ==
						  0,
					  "Alignment of FuncBoundType must be a multiple of the alignment of FuncOp");

		assert((queue_.size() < queue_.capacity()) &&
			   "Could not allocate space for function object");

		queue_.emplace(mem_pool_.template create<FuncBoundType>(op));
	}

	/// @overload void push(const TFuncOp& op)
	template<typename TFuncOp>
	void push(TFuncOp&& op) noexcept
	{
		using FuncBoundType = FuncOpBound<typename std::decay<TFuncOp>::type>;

		static_assert(std::alignment_of<FuncBoundType>::value % std::alignment_of<FuncOp>::value ==
						  0,
					  "Alignment of FuncBoundType must be a multiple of the alignment of FuncOp");

		assert((queue_.size() < queue_.capacity()) &&
			   "Could not allocate space for function in circular buffer");
		queue_.emplace(mem_pool_.template create<FuncBoundType>(std::forward<TFuncOp>(op)));
	}

	/// Remove the next functor from the front of the queue and execute it.
	void popAndExec() noexcept
	{
		auto ptr = queue_.front();
		queue_.pop();
		ptr->exec();

		// we call the deleter manually b/c we haven't created a std::unique_ptr
		// in this scenario
		deleter(ptr);
	}

	/** Get a copy of the functor at the front of the queue.
	 *
	 * For efficient queue usage, clients will want to get the element from the front of the queue,
	 * call pop() to remove it from the queue, and then execute the function. This flow will ensure
	 * that the queue is not locked while the function is running (which happens with popAndExec()).
	 *
	 * @note Because we have a std::unique_ptr, front() can only be called once per functor.
	 *
	 * @pre front() has not been previously called for this element.
	 * @returns a UniqueElementPtr_t to the functor at the front of the queue.
	 *	Once the UniqueElementPtr_t goes out of scope, the function queue will recover the memory
	 *   automatically.
	 */
	auto front() noexcept
	{
		return UniqueElementPtr_t(
			std::move(queue_.front()),
			std::bind(&StaticFunctionQueue::deleter, this, std::placeholders::_1));
	}

	/** Remove the element at the front of the queue.
	 *
	 * @pre There is an element in the queue.
	 * @post The element has been removed from the queue.
	 */
	void pop() noexcept
	{
		queue_.pop();
	}

	/** Check if the queue is empty.
	 *
	 * @returns true if the queue is empty, false otherwise.
	 */
	[[nodiscard]] bool empty() const noexcept
	{
		return size() == 0;
	}

	/** Check the current number of elements in the queue
	 *
	 * @returns the number of elements currently stored in the queue.
	 */
	[[nodiscard]] size_t size() const noexcept
	{
		return queue_.size();
	}

	/** Get the capacity in elements
	 *
	 * @returns the number of elements that the queue can support.
	 */
	constexpr size_t capacity() const noexcept
	{
		return queue_.capacity();
	}

	/** Get the total memory consumed by this queue in bytes
	 *
	 * @returns the size of this function queue in bytes.
	 */
	constexpr size_t capacity_bytes() const noexcept
	{
		return TQueueElements * TLargestSize;
	}

  private:
	/// Queue which stores function objects
	FuncOpStorage_t queue_;
};

/// @}
// End FunctionQueue

} // namespace embutil

#endif // FUNCTION_QUEUE_HPP_
