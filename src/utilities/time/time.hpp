// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#ifndef UTIL_TIME_HPP_
#define UTIL_TIME_HPP_

#include <chrono>

#ifdef UTIL_TIME_INCLUDE_TIMEVAL
#include <sys/time.h>
#endif

namespace embutil
{
/** \defgroup TimeUtil Time Utilities
 *
 * Functions that work with C and C++ time types.
 *
 * You can define `UTIL_TIME_INCLUDE_TIMEVAL` to include timeval conversion functions.
 * Since these are defined in sys/time.h on POSIX systems, embedded devices may not have
 * them, so we do not use them by default.
 *
 * Time conversionf unctions were inspired by this post on StackOverflow:
 *	https://stackoverflow.com/questions/31409036/converting-from-struct-timespec-to-stdchrono
 *
 * @ingroup FrameworkUtils
 * @{
 */

#pragma mark - C/C++ Time Conversion -

///@name Time Conversion Functions
/// @brief Convert between C and C++ time representations.

/** Convert from struct timespec to std::chrono::duration
 *
 * @param[in] ts Timespec value, which can be relative or absolute.
 * @returns the equivalent std::chrono::duration value in nanoseconds.
 */
constexpr std::chrono::nanoseconds timespecToDuration(const timespec ts) noexcept
{
	auto duration = std::chrono::seconds{ts.tv_sec} + std::chrono::nanoseconds{ts.tv_nsec};

	return std::chrono::duration_cast<std::chrono::nanoseconds>(duration);
}

/** Convert from struct timespec to std::chrono::time_point
 *
 * @param[in] ts Timespec value, which can be relative or absolute.
 * @returns the equivalent std::chrono::time_point value using the system_clock.
 */
constexpr std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds>
	timespecToTimePoint(const timespec ts) noexcept
{
	return std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds>{
		std::chrono::duration_cast<std::chrono::system_clock::duration>(timespecToDuration(ts))};
}

/** Convert from std::chrono::duration to struct timespec
 *
 * @param[in] dur std::chrono::duration value, which can be relative or absolute.
 * @returns the equivalent struct timespec value.
 */
constexpr timespec durationToTimespec(std::chrono::nanoseconds dur) noexcept
{
	auto secs = std::chrono::duration_cast<std::chrono::seconds>(dur);
	dur -= secs;

	return timespec{static_cast<time_t>(secs.count()), static_cast<long>(dur.count())};
}

/** Convert from std::chrono::time_point to struct timespec
 *
 * @param[in] tp std::chrono::time_point value using the system_clock.
 * @returns the equivalent struct timespec value.
 */
constexpr timespec timepointToTimespec(
	std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> tp) noexcept
{
	auto secs = std::chrono::time_point_cast<std::chrono::seconds>(tp);
	auto ns = std::chrono::time_point_cast<std::chrono::nanoseconds>(tp) -
			  std::chrono::time_point_cast<std::chrono::nanoseconds>(secs);

	return timespec{static_cast<time_t>(secs.time_since_epoch().count()),
					static_cast<long>(ns.count())};
}

#ifdef UTIL_TIME_INCLUDE_TIMEVAL
/** Convert from struct timeval to std::chrono::duration
 *
 * @param[in] tv timeval value, which can be relative or absolute.
 * @returns the equivalent std::chrono::duration value in nanoseconds.
 */
constexpr std::chrono::microseconds timevalToDuration(const timeval tv) noexcept
{
	auto duration = std::chrono::seconds{tv.tv_sec} + std::chrono::microseconds{tv.tv_usec};

	return std::chrono::duration_cast<std::chrono::microseconds>(duration);
}

/** Convert from struct timeval to std::chrono::time_point
 *
 * @param[in] tv timeval value, which can be relative or absolute.
 * @returns the equivalent std::chrono::time_point value using the system_clock.
 */
constexpr std::chrono::system_clock::time_point timevalToTimePoint(const timeval tv) noexcept
{
	return std::chrono::system_clock::time_point{
		std::chrono::duration_cast<std::chrono::system_clock::duration>(timevalToDuration(tv))};
}
#endif

/// @}
// End time conversion group

/// @}
// End time module

} // namespace embutil

#endif // UTIL_TIME_HPP_
