// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#ifndef SCOPE_GUARD_HPP_
#define SCOPE_GUARD_HPP_

#include <functional>
#include <memory>
#include <type_traits>

namespace embutil
{
/// @addtogroup SBRM
/// @{

/** Provides a scope guard idiom to enable custom SBRM behaviors
 *
 * The scope guard idiom allows a user to call a function when the guard object is destructed.
 * The guard class enables developes to implement custom SBRM behaviors.
 *
 * The scope guard uses static memory allocation on the stack. The guard takes as much space on the
 * stack as needed to bind the provided function with all its arguments.
 *
 * # Using the scope guard
 *
 * In order to properly create a scope guard, you must use the `makeScopeGuard()` function.
 *
 * @code
 * // Binding function with parameters:
 * auto guard = makeScopeGuard(&func, std::ref(arg1), arg2);
 *
 * // Binding lambda function:
 * auto guard = makeScopeGuard([&argByRef, argByValue]()
 * {
 * 	//guard code goes here
 * });
 * @endcode
 *
 * Note that all the bound parameters are passed by value. To use references with the bound object,
 * use std::ref() (or std::cref() for `const` reference).
 *
 * The guard function can be manually called using the `release()` function.
 *
 * A guard object only supports move semantics.
 *
 * @tparam TFunc Functor object type. The template parameter must be type of the functor that
 * doesn't receive any parameters and doesn't return any value.
 */
template<typename TFunc>
class ScopeGuard
{
  public:
	/** Constructor
	 *
	 * @param[in] func Functor that will be executed when the scope guard is
	 *  destructed (unless it is manually released). Functor must provide move/copy
	 *  constructor.
	 * @note Thread safety: Safe for distinct functors.
	 * @note Exception guarantee: nothrow if copy/move constructors of
	 *       the bind arguments do not throw, Strong otherwise.
	 */
	explicit ScopeGuard(TFunc&& func) noexcept : func_(std::forward<TFunc>(func)), active_(true) {}

	/** Move constructor
	 *
	 * After the functor is moved, it will be released in the
	 * provided guard.
	 *
	 * @param[in] guard The other scope guard of the same type.
	 * @note Thread safety: Unsafe
	 * @note Exception guarantee: nothrow if copy/move constructors of
	 *       the bind arguments do not throw, Strong otherwise.
	 */
	ScopeGuard(ScopeGuard&& guard) noexcept
		: func_(std::move(guard.func_)), active_(std::move(guard.active_))
	{
		guard.release();
	}

	/// Deleted copy constructor
	ScopeGuard(const ScopeGuard&) = delete;

	/// Deleted copy assignment operator
	const ScopeGuard& operator=(const ScopeGuard&) = delete;

	/** Destructor
	 *
	 * This function is marked noexcept because we want the program to terminate if an exception
	 * results from this call.
	 *
	 * @post The functor is called unless it was released with the `release()`
	 * function prior to destruction.
	 */
	~ScopeGuard() noexcept
	{
		if(!isReleased())
		{
			func_();
		}
	}

	/** Release the bound functor.
	 *
	 * @post The functor won't be called when the scope guard is out of scope.
	 * @note Thread safety: Safe on distinct objects.
	 * @note Exception guarantee: No throw.
	 */
	void release() noexcept
	{
		active_ = false;
	}

	/** Check whether the functor is released.
	 *
	 * @return true in case of being released.
	 * @note Thread safety: Unsafe
	 * @note Exception guarantee: No throw
	 */
	bool isReleased() const noexcept
	{
		return !active_;
	}

  private:
	typename std::remove_reference<TFunc>::type func_;
	bool active_;
};

/** Create scope guard with provided functor.
 *
 * Use this function to create a scope guard with lambda function.
 *
 * Example:
 * @code
 *  auto guard = embxx::util::makeScopeGuard([&argByRef, argByValue]()
 *  {
 *    //Some code here
 *   });
 * @endcode
 *
 * @tparam TFunctor Functor type, should be deduced automatically
 * @param[in] func Functor
 * @return Scope guard.
 * @note Thread safety: Safe on distinct functors
 * @note Exception guarantee: nothrow in case copy/move constructors of
 *       the bind arguments do not throw, Basic otherwise.
 *
 * @related ScopeGuard
 */
template<typename TFunctor>
ScopeGuard<TFunctor> makeScopeGuard(TFunctor&& func) noexcept
{
	return ScopeGuard<TFunctor>(std::forward<TFunctor>(func));
}

/** Create scope guard by binding the provided function and
 *        all the arguments.
 *
 * Use this function to create a scope guard when a function
 * with one or more arguments needs to be called.
 *
 * Example:
 *
 * @code
 * // Binding function with parameters:
 * auto guard = makeScopeGuard(&func, std::ref(arg1), arg2);
 * @endcode
 *
 * Note that all the bound parameters are passed by value. If
 * there is any need to use references with the bound object,
 * use `std::ref()` (or `std::cref()` to create a `const` reference).
 *
 * This function is marked noexcept because we want the program to terminate if an exception
 * results from this call.
 *
 * @tparam TFunc Pointer to function type
 * @tparam TParams Types of other arguments. Variadic, so supports any number of arguments.
 * @param[in] func Functor
 * @param[in] args Function arguments
 * @return Scope guard.
 * @note Thread safety: Unsafe
 * @note Exception guarantee: `nothrow` in case copy/move constructors of
 *       the bind arguments do not throw, Basic otherwise.
 *
 * @related ScopeGuard
 */
template<typename TFunc, typename... TParams>
auto makeScopeGuard(TFunc&& func, TParams... args) noexcept
	-> ScopeGuard<decltype(std::bind(std::forward<TFunc>(func), std::forward<TParams>(args)...))>
{
	auto bindObj = std::bind(std::forward<TFunc>(func), std::forward<TParams>(args)...);
	return ScopeGuard<decltype(bindObj)>(std::move(bindObj));
}

/// @}
// End group

}; // namespace embutil

#endif // SCOPE_GUARD_HPP_
