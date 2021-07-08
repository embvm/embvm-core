// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#ifndef C_SBRM_H_
#define C_SBRM_H_

#include <memory>

namespace embutil
{
/** @defgroup SBRM Scope-bound Resource Management
 * @brief Utilties that enable SBRM techniques
 *
 * @ingroup FrameworkUtils
 * @{
 */

/** Enable Scope-Bound Resource Management (SBRM) for C-style resources (std::unique_ptr variant).
 *
 * The unique_resource() function creates unique pointers that manage C-style Resources.
 * The function takes in an acquisiton function and release function (e.g. fopen and fclose).
 * Remaining arguments are optional. If specified, they will be forwarded to the acquisition
 * function.
 *
 * The resource is created via the acquisition function and stored in a unique_ptr.
 * When the unique_ptr goes out of scope, the delete function will be automatically called.
 *
 * The unique_resource() function returns a std::unique_ptr, and is intended for resources that have
 * a single owner.
 *
 * # Using unique_resource
 *
 * You specify the storage type when invoking unique_resource(). For example, if we want to
 * manage a `FILE`:
 *
 * @code
 * unique_resource<FILE>(...);
 * @endcode
 *
 * Note that we use `FILE` in the template invocation, rather than `FILE*`. std::unique_ptr will
 * actually manage a `FILE*` under the hood, and that's what the acquire function returns.
 *
 * The full invocation requires us to specify arguments. In order to open a file we would normally
 * use fopen(), and to close the file we use fclose(). fopen() requires two arguments: a filename
 * and the mode (e.g., read or write). We open a file `filename` for writing in the following way:
 *
 * @code
 * auto ufile = embutil::unique_resource<FILE>(fopen, fclose, filename, "w");
 * @endcode
 *
 * The resulting `ufile` can be passed into the C-style APIs using the std::unique_ptr
 * `.get()` member function.
 *
 * @code
 * fwrite(test_content, 1, strlen(test_content), ufile.get());
 * @endcode
 *
 * When `ufile` goes out of scope, the resource will be automatically cleaned up
 * by calling the `release` function on the managed pointer.
 *
 * @tparam T The type of the resource that will be created and managed. This type will actually
 * 	be converted to a T*, which std::unique_ptr manages.
 * @tparam AcquisitionFunc The function prototype for the function which is used to create or
 *	open the resource (e.g. fopen()). This type will be deduced from the function arguments.
 * @tparam ReleaseFunc The function prototype for the function which is used to create or
 *	destroy the resource (e.g. fclose()). This type will be deduced from the function arguments.
 * @tparam Args Variadic list of arguments which will be forwarded to the AcquisitionFunc.
 *	This type will be deduced from the function arguments.
 * @param acquire The function used to acquire the resource.
 * @param release The function used to release the resource.
 * @param args A variadic list of arguments which will be forwarded to the acquire function.
 * @returns a std::unique_ptr which manages an instance of type T, configured with release as the
 * 	deleter function. When the resource goes out of scope, it will be automatically cleaned up
 *	via the release function.
 */
template<typename T, typename AcquisitionFunc, typename ReleaseFunc, typename... Args>
std::unique_ptr<T, ReleaseFunc> unique_resource(AcquisitionFunc acquire, ReleaseFunc release,
												Args&&... args) noexcept
{
	return {acquire(std::forward<Args>(args)...), release};
}

/** Enable Scope-Bound Resource Management (SBRM) for C-style resources (std::shared_ptr variant).
 *
 * The shared_resource() function creates unique pointers that manage C-style Resources.
 * The function takes in an acquisition function and release function (e.g. fopen and fclose).
 * Remaining arguments are optional. If specified, they will be forwarded to the acquisition
 * function.
 *
 * The resource is created via the acquisition function and stored in a shared_ptr.
 * When the shared_ptr refcount reaches 0, the delete function will be automatically called.
 *
 * The shared_resource() function returns a std::shared, and is intended for resources that have
 * multiple owners.
 *
 * # Using shared_resource
 *
 * You specify the storage type when invoking shared_resource(). For example, if we want to
 * manage a `FILE`:
 *
 * @code
 * shared_resource<FILE>(...);
 * @endcode
 *
 * Note that we use `FILE` in the template invocation, rather than `FILE*`. std::shared_resource
 * will actually manage a `FILE*` under the hood, and that's what the acquire function returns.
 *
 * The full invocation requires us to specify arguments. In order to open a file we would normally
 * use fopen(), and to close the file we use fclose(). fopen() requires two arguments: a filename
 * and the mode (e.g., read or write). We open a file `filename` for writing in the following way:
 *
 * @code
 * auto sfile = embutil::shared_resource<FILE>(fopen, fclose, filename, "w");
 * @endcode
 *
 * The resulting `sfile` can be passed into the C-style APIs using the std::shared_ptr
 * `.get()` member function.
 *
 * @code
 * fwrite(test_content, 1, strlen(test_content), sfile.get());
 * @endcode
 *
 * When `sfile` goes out of scope, the resource will be automatically cleaned up
 * by calling the `release` function on the managed pointer.
 *
 * @tparam T The type of the resource that will be created and managed. This type will actually
 * 	be converted to a T*, which std::shared_ptr manages.
 * @tparam AcquisitionFunc The function prototype for the function which is used to create or
 *	open the resource (e.g. fopen()). This type will be deduced from the function arguments.
 * @tparam ReleaseFunc The function prototype for the function which is used to create or
 *	destroy the resource (e.g. fclose()). This type will be deduced from the function arguments.
 * @tparam Args Variadic list of arguments which will be forwarded to the AcquisitionFunc.
 *	This type will be deduced from the function arguments.
 * @param acquire The function used to acquire the resource.
 * @param release The function used to release the resource.
 * @param args A variadic list of arguments which will be forwarded to the acquire function.
 * @returns a std::shared_ptr which manages an instance of type T, configured with release as the
 * 	deleter function. When the std::shared_ptr refcount reaches 0, it will be automatically cleaned
 *	up via the release function.
 */
template<typename T, typename AcquisitionFunc, typename ReleaseFunc, typename... Args>
std::shared_ptr<T> shared_resource(AcquisitionFunc acquire, ReleaseFunc release,
								   Args&&... args) noexcept
{
	return {acquire(std::forward<Args>(args)...), release};
}

/// @}
// end group

} // namespace embutil

#endif // C_SBRM_H_
