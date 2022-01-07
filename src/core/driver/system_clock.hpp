// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#ifndef SYSTEM_CLOCK_HPP_
#define SYSTEM_CLOCK_HPP_

#include "clock.hpp"
#include "driver.hpp"

namespace embvm
{
/// @addtogroup FrameworkDriver
/// @{

namespace clk
{
/** This class defines common system clock interfaces.
 *
 * # Define a system clock driver
 *
 * To create a system clock driver, derive from the SystemClock object:
 *
 * @code
 * class SimulatorSystemClock final : public SystemClock
 * { ... };
 * @endcode
 *
 * Derived classes must implement the following interfaces:
 * - frequency()
 * - tips()
 * - spin()
 *
 * Derived classes must also implement pure virtual embvm::DriverBase functions:
 * - start_()
 * - stop_()
 */
class SystemClock : public embvm::DriverBase
{
  public:
	/// Default spin increment is microseconds
	using spin_duration_t = std::chrono::duration<uint32_t, std::micro>;

	// std::chrono::duration type for the tick return
	using tick_duration_t = std::chrono::duration<uint64_t, std::micro>;

  protected:
	/** Default constructor.
	 *
	 * Initializes the base class with the proper typeid.
	 */
	SystemClock() noexcept : embvm::DriverBase(embvm::DriverType::SYSTEM_CLOCK)
	{
		// empty constructor
	}

	/// Default destructor
	~SystemClock() noexcept;

  public:
	/** SystemClock Driver Type ID
	 *
	 * @returns SystemClock type ID.
	 */
	static constexpr auto type() noexcept -> embvm::DriverType
	{
		return embvm::DriverType::SYSTEM_CLOCK;
	}

	/** Get the frequency of the system clock.
	 *
	 * @returns the frequency of the system clock, in Hz.
	 */
	[[nodiscard]] virtual freq_hz_t::rep frequency() const noexcept = 0;

	/** Check the ticks elapsed since boot.
	 *
	 * Derived classes must implement the ticks() function.
	 *
	 * @returns ticks elapsed since boot.
	 */
	[[nodiscard]] virtual tick_duration_t::rep ticks() const noexcept = 0;

	/** Spin the processor for the specified number of ticks.
	 *
	 * Derived classes must implement the spin() function.
	 *
	 * @param count The number of ticks to spin the processor for.
	 */
	virtual void spin(spin_duration_t::rep count) noexcept = 0;

  protected:
	// embvm::DriverBase function that derived classes must implement.
	void start_() noexcept override = 0;

	// embvm::DriverBase function that derived classes must implement.
	void stop_() noexcept override = 0;
};

} // namespace clk

/// @}
// End group

} // namespace embvm

#endif // SYSTEM_CLOCK_HPP_
