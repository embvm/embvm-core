// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#ifndef CLOCK_HPP_
#define CLOCK_HPP_

#include <chrono>

namespace embvm
{
/// @addtogroup FrameworkDriver
/// @{

/// Definitions, functions, and classes related to clock devices.
namespace clk
{
/// Represents the frequency of the clock, in Hz.
using freq_hz_t = std::chrono::duration<uint64_t, std::ratio<1>>;

/// Represents the period of the clock, in nanoseconds.
using clock_period_t = std::chrono::duration<uint64_t, std::nano>;
} // namespace clk

/// @}
// End group

} // namespace embvm

#endif // CLOCK_HPP_
