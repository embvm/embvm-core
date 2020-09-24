#ifndef BOUNCE_HPP_
#define BOUNCE_HPP_

namespace embutil
{
/** Enable a C++ member function to work with a C-style callback interface.
 *
 * This function is intended to be passed to a C-style callback system which takes
 * a function pointer and private data pointer as input values. Using C-style callbacks
 * is problematic with object member functions, as you need the object's pointer to get to the
 * correct instance.
 *
 * This bounce function requires use of the private data pointer in a C-style callback. The data
 * pointer must be used for the object's instance pointer.
 * Any other provided arguments are forwarded to the object's member function,
 * and the return value is passed to the caller.
 *
 * You can simplify this call with the BOUNCE() macro.
 *
 * You may need to reinterpret_cast the bounce function to the expected type:
 * @code
 * reinterpret_cast<void*(*)(void*)>(BOUNCE(c, function))
 * @endcode
 *
 * You will need to couple a bounce function with the `this` pointer as a function input
 * to get to the correct instance.
 *
 * # Example usage with a C-style callback interface
 *
 * posixThread is a C++ class, and posixThread::thread_func() is a member function
 * which we want to pass to pthread_create(). The `this` pointer is captured and passed into
 * the private input data argument, which is forwarded to the thread.
 *
 * @code
 * void* posixThread::thread_func()
 * {...}
 *
 * [...]
 *
 * // Our interfaces demand a void(*)(void*), so we adapt to posix's requirement
 * // and simply skip out on the return to the caller's perspective
 * r = pthread_create(&handle_, &attributes,
 *	reinterpret_cast<void* (*)(void*)>(
 *		embutil::bounce<posixThread, decltype(&posixThread::thread_func),
 *	&posixThread::thread_func>), *		reinterpret_cast<void*>(this));
 * @endcode
 *
 * The use case above is too verbose, so we recommend the BOUNCE() macro instead:
 *
 * @code
 * BOUNCE(posixThread, thread_func);
 * @endcode
 *
 * This function is marked noexcept because we want the program to terminate if an exception
 * results from this call. We can't guarantee the bounce function won't throw.
 *
 * @tparam TClass The type of class with the member function to be used as a callback.
 * @tparam Method The type of the member function to use as a callback.
 * @tparam m The function pointer corresponding to the class's member function.
 * @tparam Params Variadic template list of function parameters to forward to the member function.
 * @param priv The private data pointer, which must always be passed the class's `this` pointer
 *	so bounce can resolve to the correct instance.
 * @param params Variadic list of parameters to forward to the member function.
 * @returns the value returned by the member function.
 * @ingroup FrameworkUtils
 */
template<class TClass, class Method, Method m, class... Params>
static auto bounce(void* priv, Params... params) noexcept
	-> decltype(((*reinterpret_cast<TClass*>(priv)).*m)(params...))
{
	return ((*reinterpret_cast<TClass*>(priv)).*m)(params...);
}

/** Convenience macro to simplify bounce statement usage.
 *
 * The BOUNCE() macro simplifies the creation of a bounce function to a class member
 * for use with C-style callbacks. The BOUNCE() macro must be paried with a `this`
 * pointer to get to the proper class instance.
 *
 * # Example usage with a C-style callback interface.
 * posixThread is a C++ class, and posixThread::thread_func() is a member function
 * which we want to pass to pthread_create().
 *
 * @code
 * // Our interfaces demand a void(*)(void*), so we adapt to posix's requirement
 * // and simply skip out on the return to the caller's perspective
 * r = pthread_create(&handle_, &attributes,
 *	reinterpret_cast<void* (*)(void*)>(BOUNCE(posixThread, thread_func)),
 *	reinterpret_cast<void*>(this));
 * @endcode
 *
 * @param c The class type.
 * @param m The member function in the class
 * @returns A bounce function which redirects a C-style callback to a class member function.
 */
#define BOUNCE(c, m) embutil::bounce<c, decltype(&c::m), &c::m>

} // namespace embutil

#endif // BOUNCE_HPP_
