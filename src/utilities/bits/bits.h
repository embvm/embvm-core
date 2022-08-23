// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#ifndef BITS_H_
#define BITS_H_

/// @defgroup BitUtilities Bit Manipulation
/// @brief Functions and macros that operate in a bitwise manner.
/// @ingroup FrameworkUtils
/// @{

/// @name Alignment Macros
/// @{

// We turn off this clang-tidy check because we want the macros for
// C programs
// NOLINTBEGIN(cppcoreguidelines-macro-usage)

/** Increase a number to the next aligned value
 *
 * @param num The number to start with.
 * @param align The target alignment.
 * @returns The next value greater than num matching the target alignment.
 */
#define align_up(num, align) (((num) + ((align)-1)) & ~((align)-1))

/** Check if a number is a power of 2
 *
 * @param x The number to check.
 * @returns true if the number is a power of 2, false otherwise.
 */
#define IS_POWER_2(x) (!((x) & ((x)-1)))

/** Check if a value is aligned.
 *
 * @param val The number to check.
 * @param align The target alignment.
 * @returns true if the number matches the target alignment, false otherwise.
 */
#define IS_ALIGNED(val, align) (((val) & ((align)-1)) == 0)

/// @}

/// @name Bitfield Macros
/// @{

/** Set bit at position X
 *
 * @param x The bit position to set.
 * @returns A number with bit x set.
 */
#define SET_BIT(x) (1 << (x))

/** Extract a field of bits from a value.
 *
 * Given a number, such as: 0b11010100
 *
 * If we wanted to extract bits 7..5, we would supply:
 * 	high_bit = 7
 *	low_bit = 5
 *
 * And the result would be 0b110
 *
 * @param value The value to extract the bitfield from.
 * @param high_bit the bit representing the upper limit of the bitfield.
 * @param low_bit the bit representing the lower limit of the bitfield.
 * @returns the extracted bitfield, shifted right by low_bit.
 */
#define EXTRACT_BITFIELD(value, high_bit, low_bit) \
	(((value) >> (low_bit)) & N_SET_BITS((high_bit) - (low_bit) + 1))

/// @}

/// @name Count Bits Macros
/// @{

/** Count the number of set bits.
 *
 * @param n_ones The number to check.
 * @returns the number of set bits.
 */
#define N_SET_BITS(n_ones) ((1 << (n_ones)) - 1)

// NOLINTEND(cppcoreguidelines-macro-usage)

///@}

/// @}
// End BitUtilities

#endif // BITS_H_
