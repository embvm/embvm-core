// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#ifndef BITS_HPP_
#define BITS_HPP_

#include "bits.h"
#include <algorithm>
#include <array>
#include <climits>

namespace embutil
{
/// @addtogroup BitUtilities
/// @{

#pragma mark - Alignment

/// @name Alignment Functions
/// @{

/** Check if a number is a power of 2.
 *
 * @tparam TIntegralType The type of the number to check. This type does not need to be manually
 * 	specified. The compiler should automatically deduce this type. TIntegralType must be an
 *	integral type.
 * @param val The number to evaluate.
 * @returns true if the number is a power of 2, false otherwise.
 */
template<typename TIntegralType>
constexpr auto is_power_2(TIntegralType val) noexcept -> bool
{
	static_assert(std::is_integral<TIntegralType>::value,
				  "This function can only be called on integral types");

	// cppcheck-suppress MultipleUnaryOperator
	return static_cast<bool>(IS_POWER_2(val));
}

/** Checks if a value meets a specified alignment.
 *
 * Behavior for integral types: check if the value is aligned.
 *
 * Behavior for pointers: check if the pointer is aligned.
 *
 * Behavior for non-integral non-pointer types: Take the address and determine if it is aligned.
 *
 * @tparam TType the type of data to check the alignment of. TType can be integral, pointer, or
 *	non-integral and non-pointer. The behavior of the function changes depending on the type.
 * @param val The value to check the alignment of.
 * @param align The target alignment.
 * @returns true if the value has the target alignment, false otherwise.
 */
template<typename TType>
inline constexpr auto is_aligned(const TType val, const size_t align) noexcept -> bool
{
	// This assignment is NOT redundant in a constexpr context, and will break GCC
	// cppcheck-suppress redundantAssignment
	bool r = false;

	if constexpr(std::is_integral<TType>::value)
	{
		// cppcheck-suppress redundantAssignment
		r = static_cast<bool>(IS_ALIGNED(val, align));
	}
	else if constexpr(std::is_pointer<TType>::value)
	{
		r = IS_ALIGNED(reinterpret_cast<uintptr_t>(val), align);
	}
	else
	{
		r = IS_ALIGNED(reinterpret_cast<uintptr_t>(&val), align);
	}

	return r;
}

/// @}

#pragma mark - Bitmask -

/// @name Bitfield Functions
/// @{

/** A C++ 17 bitmask generator.
 *
 * This function was inspired by Martin Moene.
 *
 * Example: bitmask<uint8_t>(6, 5, 0) -> 0b0110'0001
 *
 * @tparam TIntegralType The desired type for the bitmask.
 * @tparam Args A variadic list corresponding to bits to be set. Args is detected by the
 * compiler.
 * @param args The variadic list of bits to set in the bitmask.
 * @returns A number of type TIntegralType with the bits in the input list set.
 */
template<typename TIntegralType, typename... Args>
constexpr auto bitmask(Args&&... args) noexcept -> TIntegralType
{
	static_assert(std::is_integral<TIntegralType>::value,
				  "This function can only be called on integral types");

	return static_cast<TIntegralType>(((1 << args) | ...));
}

/// @}

#pragma mark - Count Bits -

/** Template struct for counting set bits and significant bits at compile-time.
 *
 * Examples:
 * @code
 *  countbits<8>::setbits == 1 (0b1000)
 *	countbits<9>::setbits == 2 (0b1100)
 * 	countbits<8>::significant == 4 (0b1111)
 *	countbits<9>::significant == 4 (0b1111)
 * @endcode
 *
 * @tparam tval Unsigned integral value to analyze.
 */
template<unsigned tval>
struct countbits
{
	using t_ = countbits<tval / 2>;

	/// Shows how many bits in the value are set
	static constexpr unsigned set = ((tval & 1) != 0) ? (t_::set + 1) : ((tval == 0) ? 0 : t_::set);

	/// shows how many bits in the value matter
	static constexpr unsigned significant_bits = (tval != 0) ? t_::significant_bits + 1 : 0;
};

/// @}
// End BitUtilities

} // namespace embutil

#endif // BITS_HPP_
