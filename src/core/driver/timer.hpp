#ifndef TIMER_HPP_
#define TIMER_HPP_

#include "driver.hpp"
#include <chrono>
#include <inplace_function/inplace_function.hpp>

namespace embvm
{
/// @addtogroup FrameworkDriver
/// @{

/// Definitions, functions, and classes related to Timer devices.
namespace timer
{
/// Represents the frequency of the timer, in Hz.
using freq_hz_t = std::chrono::duration<uint64_t, std::ratio<1>>;

/// Represents the period of the timer, in microseconds.
using timer_period_t = std::chrono::duration<uint64_t, std::micro>;

/// Alias for timer callback functions.
using cb_t = stdext::inplace_function<void(void)>;

/// Configuration for the timer.
enum class config : uint8_t
{
	/// The timer will run once, and then will remain in the expired state
	/// until reconfigured.
	oneshot = 0,
	/// When the timer expires, the existing configuration will be reloaded and
	/// it will be rearmed.
	periodic
};

/// Possible timer states.
enum class state : uint8_t
{
	/// The timer has been manually stopped or is not yet configured.
	stopped = 0,
	/// The timer has expired and is no longer running.
	expired,
	/// The timer is armed and running.
	armed,
};

/** Timer device ID helper representation
 *
 * This is a "portable" definition for a Timer device/channel (e.g. TIM3 registers)
 * that use can use as a parameter for implementation classes, since
 * you may not want to expose the register definitions publicly in a head.
 */
enum channel : uint8_t
{
	CH0 = 0,
	CH1,
	CH2,
	CH3,
	CH4,
	CH5,
	CH6,
	CH7,
	CH8,
	CH9,
	CH10,
	MAX_CHANNELS
};

/** This class provides generic Timer interface definitions.
 *
 * The Timer class defines the common interfaces for Timer drivers. This class
 * can be inherited to provide specific Timer implementations.
 *
 * Timers can be used with the embvm::TimerManager<> class to provide software timer support.
 *
 * # Define a Timer driver
 *
 * To create a specialized Timer driver, derive from the Timer object:
 *
 * @code
 * class SimulatorTimer final : public timer::Timer
 * { ... };
 * @endcode
 *
 * Derived classes must implement the following interfaces:
 * - registerCallback()
 *
 * Derived classes must also implement pure virtual embvm::DriverBase functions:
 * - start_()
 * - stop_()
 *
 * The start_() function must implement the steps for starting the timer hardware.
 * The stop_() function is responsible for stopping the timer hardware.
 #
 # Note that the derived class will need to update the state variable according
 # to its current operational status.
 *
 * # Using a Timer driver
 *
 * Timers should be configured with a period(). If the default one-shot configuration is not
 * desired, the config() can be used to change the timer operational mode.
 *
 * A callback should be registered with the registerCallback() function. When the timer expires,
 * the callback will be invoked. If the timer is configured as a one-shot, the timer will remain
 * expired until start() or restart() is called. If a timer is configured as periodic, the timer
 * will reload the period and re-arm itself.
 *
 * A timer can be restarted (even while running) using the existing period or with a new period.
 *
 * Timers can be stopped while they are running using the stop() function.
 *
 * # Software Timers with embvm::TimerManager<>
 *
 * Timers can be used with the embvm::TimerManager<> class to provide software timer support.
 *
 * @code
 * SimulatorTimer timer;
 * embvm::TimerManager tm(timer);
 * @endcode
 *
 * The embvm::TimerManager will use the timer hardware driver as a master timer, enabling multiple
 * software timers to be created and used while consuming only a single hardware resource.
 *
 */
class Timer : public embvm::DriverBase
{
  protected:
	/** Default constructor.
	 *
	 * Initializes the drive base class with the appropriate typeid.
	 */
	Timer() noexcept : embvm::DriverBase(embvm::DriverType::TIMER) {}

	/// Default destructor.
	~Timer() noexcept;

  public:
	/** Timer Driver Type ID
	 *
	 * @returns Timer type ID.
	 */
	static constexpr embvm::DriverType type() noexcept
	{
		return embvm::DriverType::TIMER;
	}

	/** Get the current timer period.
	 *
	 * @returns the currently configured timer period.
	 */
	timer_period_t period() const noexcept
	{
		return period_;
	}

	/** Set the timer period.
	 *
	 * If the timer is currently running, this call has no effect. Use restart() instead.
	 *
	 * @param period the desired timer period.
	 * @returns the currently configured timer period.
	 */
	timer_period_t period(timer_period_t period) noexcept
	{
		period_ = period;

		return period_;
	}

	/** Register a timer callback.
	 *
	 * The timer callback function is called whenever the timer expires.
	 *
	 * Derived classes must implement this function. Derived classes can choose to support either
	 * a single callback or multiple callbacks.
	 *
	 * @param cb A callback to invoke when the timer expires.
	 */
	virtual void registerCallback(const timer::cb_t& cb) noexcept = 0;

	/** Register a timer callback.
	 *
	 * The timer callback function is called whenever the timer expires.
	 *
	 * Derived classes must implement this function. Derived classes can choose to support either
	 * a single callback or multiple callbacks.
	 *
	 * @param cb A callback to invoke when the timer expires.
	 */
	virtual void registerCallback(timer::cb_t&& cb) noexcept = 0;

	/** Restart the timer with a specific period.
	 *
	 * @param period The desired timer period to reset the timer to use. Accepts
	 * 	a std::chrono value.
	 */
	virtual void restart(const timer_period_t period) noexcept
	{
		stop();

		period_ = period;

		start();
	}

	/** Restart the timer with a specific period.
	 *
	 * @param period The desired timer period to reset the timer to use.
	 *	Accepts an integral count representing the period in microseonds.
	 */
	void restart(const timer_period_t::rep period) noexcept
	{
		restart(timer_period_t(period));
	}

	/// Restart the timer with the currently configured period.
	void restart() noexcept
	{
		restart(period_);
	}

	/** Check the state of the timer.
	 *
	 * @returns the current timer state.
	 */
	virtual timer::state state() const noexcept
	{
		return state_;
	}

	/** Check the configuration of the timer.
	 *
	 * @returns the current timer configuration.
	 */
	virtual timer::config config() const noexcept
	{
		return config_;
	}

	/** Set the timer configuration.
	 *
	 * @param c The desired timer configuration (periodic or oneshot).
	 * @returns the current timer configuration.
	 */
	virtual timer::config config(timer::config c) noexcept
	{
		config_ = c;
		return c;
	}

	/** Read the current timer count.
	 *
	 * @returns the current count of the timer
	 */
	virtual timer_period_t count() const noexcept = 0;

  protected:
	// embvm::DriverBase function that derived classes must implement
	void start_() noexcept override = 0;

	// embvm::DriverBase function that derived classes must implement
	void stop_() noexcept override = 0;

  protected:
	/// The configured timer period.
	timer_period_t period_{0};

	/// The current state of the timer.
	timer::state state_ = timer::state::stopped;

	/// The timer configuratoin.
	timer::config config_ = timer::config::oneshot;
};
} // namespace timer

/// @}
// End group

} // namespace embvm

#endif // TIMER_HPP_
