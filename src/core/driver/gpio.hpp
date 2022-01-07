// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#ifndef GPIO_HPP_
#define GPIO_HPP_

#include "driver.hpp"

namespace embvm
{
/// @addtogroup FrameworkDriver
/// @{

/// Definitions, functions, and classes related to GPIO devices.
namespace gpio
{
/// GPIO mode configuration options
enum class mode
{
	/// GPIO input
	input = 0,
	/// GPIO output
	output,
	/// Configure this pin in special-purpose mode.
	/// e.g. non-standard IO mode such as PWM, SPI CS line
	special,
	MAX_MODE
};

/** GPIO port ID helper representation
 *
 * This is a "portable" definition for a GPIO port (e.g. PORTA registers)
 * that use can use as a template parameter for implementation classes, since
 * you may not want to expose the register definitions publicly in a head.
 *
 */
enum port : uint8_t
{
	A = 0,
	B,
	C,
	D,
	E,
	F,
	G,
	H,
	I,
	J,
	K,
	L,
	MAX_PORT,
};

/** GPIO slew rate (speed) configuration options.
 *
 * This enumeration represents the configuration options for the GPIO slew rate (speed) settings.
 * While not attached to a standard GPIO interface, this type can be used in derived drivers
 * that do support setting slew rate for a pin.
 */
enum class slew
{
	/// Slow (standard) state transitions.
	slow = 0,
	/// High-speed state transitions.
	fast,
	/// Indicates final value.
	MAX_SLEW_RATE,
};

/** GPIO internal pull configuration options.
 *
 * This enumeration represents the configuration options for a GPIO's internal pull settings.
 * While not attached to a standard GPIO interface, this type can be used in derived drivers
 * that do support setting internal pullup/pulldown resistors.
 *
 * Note that external pull-ups or pull-downs may still be used if gpio::pull::none is selected.
 * This is not an indication that NO pullups/downs are used, but rather that no internal resistor
 * setting is used.
 */
enum class pull
{
	/// No internal pull option is used.
	none,
	/// Internal pull-up is configured.
	pullup,
	/// Internal pull-down is enabled.
	pulldown,
	/// Indicates final value.
	MAX_PULL
};

/** This class defines the common GPIO interfaces.
 *
 * The gpio::base class defines interfaces which are common to all GPIO objects.
 * This type is used to interface with the GPIO controller itself, giving you
 * full access to the device.
 *
 * These restrict the interfaces that are available to users, ensuring that you
 * can't accidentally set an input pin to an output or vice versa.
 *
 * Derived classes must implement pure virtual embvm::DriverBase functions:
 * - start_()
 * - stop_()
 */
class base : public embvm::DriverBase
{
  protected:
	/** Default constructor.
	 *
	 * Initializes the driver base with the proper typeid.
	 */
	base() noexcept : embvm::DriverBase(embvm::DriverType::GPIO)
	{
		// empty constructor
	}

  public:
	/** GPIO Driver Type ID
	 *
	 * @returns GPIO type ID.
	 */
	static constexpr auto type() noexcept -> embvm::DriverType
	{
		return embvm::DriverType::GPIO;
	}

	/** Set the current GPIO pin state.
	 *
	 * Derived classes must implement the set() function.
	 *
	 * @precondition The GPIO driver is started.
	 * @sideeffect The GPIO pin is set ot the desired state.
	 *
	 * @param v The desired GPIO state. True for logic 1, false for logic 0.
	 */
	virtual void set(bool v) noexcept = 0;

	/** Get the current GPIO pin state.
	 *
	 * Derived classes must implement the get() function.
	 *
	 * @precondition The GPIO driver is started.
	 *
	 * @returns true if the GPIO is set (logic 1), false otherwise (logic 0).
	 */
	virtual auto get() noexcept -> bool = 0;

	/** Toggle the pin state.
	 *
	 * Changes the pin state from high to low, or low to high.
	 *
	 * @precondition The GPIO driver is started
	 * @sideffect The GPIO output changes from high to low, or low to high.
	 */
	virtual void toggle() noexcept = 0;

	virtual void setMode(embvm::gpio::mode mode) = 0;

	/** Get the Current Mode
	 *
	 * @returns the currently configured GPIO mode
	 */
	virtual auto mode() -> embvm::gpio::mode = 0;

  protected:
	/// Default destructor.
	~base() noexcept = default;

	// embvm::DriverBase function for derived class to implement.
	void start_() noexcept override = 0;

	// embvm::DriverBase function for derived class to implement.
	void stop_() noexcept override = 0;
};

} // namespace gpio

/// @}
// End group

} // namespace embvm

#endif // GPIO_HPP_
