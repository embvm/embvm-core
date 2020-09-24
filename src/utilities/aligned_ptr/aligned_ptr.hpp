/*
 * Copyright © 2017 Embedded Artistry LLC.
 * License: MIT. See LICENSE file for details.
 */

#ifndef ALIGNED_PTR_H_
#define ALIGNED_PTR_H_

#include <aligned_malloc.h>
#include <memory>

namespace embutil
{
/// @addtogroup SBRM
/// @{

/** Unique pointer alias for aligned memory (calls aligned_free()).
 *
 * This is an alias to aid the aligned pointer declaration.
 * We specify that our alias has a fixed value: the deleter function type (aligned_free()).
 * We use this for the unique_ptr declaration, as it requires us to specify the type of the deleter.
 *
 * @tparam TType The type of data being allocated.
 */
template<typename TType>
using unique_ptr_aligned = std::unique_ptr<TType, decltype(&aligned_free)>;

/** Create a unique pointer to aligned memory.
 *
 * This template function simplifies our declarations of aligned unique pointers.
 * Alignment and size are passed to aligned_malloc(). aligned_free() is always used as the deleter.
 *
 * @tparam TType The type of the data being allocated.
 * @param align The desired alignment. align must be a power of 2.
 * @param size The size of the allocation in bytes.
 *
 * @returns a std::unique_ptr to aligned memory with aligned_free() configured as the deleter func.
 */
template<typename TType>
unique_ptr_aligned<TType> aligned_uptr(size_t align, size_t size) noexcept
{
	return unique_ptr_aligned<TType>(static_cast<TType*>(aligned_malloc(align, size)),
									 &aligned_free);
}

/** Create a shared pointer to aligned memory.
 *
 * This template function simplifies our declarations of aligned shared pointers.
 * Alignment and size are passed to aligned_malloc(), and aligned_free() is always used as the
 * deleter.
 *
 * Notice here that the shared pointer doesn't need a special type declaration to work with a
 * deleter.
 *
 * @tparam TType The type of the data being allocated.
 * @param align The desired alignment. align must be a power of 2.
 * @param size The size of the allocation in bytes.
 *
 * @returns a std::shared_ptr to aligned memory with aligned_free() configured as the deleter func.
 */
template<typename TType>
std::shared_ptr<TType> aligned_sptr(size_t align, size_t size) noexcept
{
	return std::shared_ptr<TType>(static_cast<TType*>(aligned_malloc(align, size)), &aligned_free);
}

/// @}
// end group

} // namespace embutil

#endif
