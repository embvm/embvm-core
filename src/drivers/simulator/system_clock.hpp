// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#ifndef SIMULATOR_SYSTEM_CLOCK_HPP_
#define SIMULATOR_SYSTEM_CLOCK_HPP_

#include <driver/system_clock.hpp>
#include <ratio>
#include <thread>

namespace embdrv
{
/** Simulator system clock driver.
 *
 * This driver uses the std::chrono::steady_clock to represent a SystemClock driver for simulator
 * applications.
 *
 * @ingroup SimulatorDrivers
 */
class SimulatorSystemClock final : public embvm::clk::SystemClock
{
  public:
	/// Create a generic simulator system clock.
	SimulatorSystemClock() noexcept
	{
		static_assert(std::ratio_less_equal<std::chrono::steady_clock::period,
											tick_duration_t::period>::value,
					  "Clock frequency cannot support period specified by SystemClock base class");
	}

	/// Default destructor
	~SimulatorSystemClock() noexcept;

	[[nodiscard]] embvm::clk::freq_hz_t::rep frequency() const noexcept final
	{
		return std::ratio_divide<std::ratio<1, 1>, std::chrono::steady_clock::period>::num;
	}

	[[nodiscard]] tick_duration_t::rep ticks() const noexcept final
	{
		return std::chrono::duration_cast<tick_duration_t>(
				   std::chrono::steady_clock::now().time_since_epoch())
			.count();
	}

	void spin(spin_duration_t::rep count) noexcept final
	{
		std::this_thread::sleep_for(spin_duration_t{count});
	}

  protected:
	void start_() noexcept final
	{
		// Empty passthrough for simulator - no need to start/stop anything
	}

	void stop_() noexcept final
	{
		// Empty passthrough for simulator - no need to start/stop anything
	}
};

} // namespace embdrv

#endif // SIMULATOR_SYSTEM_CLOCK_HPP_
