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
/// GPIO direction configuration options
enum class direction
{
	/// GPIO input
	in = 0,
	/// GPIO output
	out,
	/// GPIO input-output
	inout,
	/// Configure this pin in special-purpose mode.
	/// e.g. non-standard IO mode such as PWM, SPI CS line
	special,
};

/** GPIO internal pull configuration options.
 *
 * This enumeration represents the configuration options for a GPIO's internal pull settings.
 *
 * External pull-ups or pull-downs may still be used if gpio::pull::none is selected.
 */
enum class pull
{
	/// No internal pull option is used.
	none,
	/// Internal pull-up is configured.
	pullup,
	/// Internal pull-down is enabled.
	pulldown,
};

/** GPIO port ID helper representation
 *
 * This is a "portable" definition for a GPIO port (e.g. PORTA registers)
 * that use can use as a template parameter for implementation classes, since
 * you may not want to expose the register definitions publicly in a head.
 *
 */
enum class port : uint8_t
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
};

/** This class defines the common GPIO interfaces.
 *
 * The gpio::base class defines interfaces which are common to all GPIO objects.
 * This class is not intended to be used directly for implementing GPIO drivers.
 * Instead, additional specializations are provided:
 * - gpio::input
 * - gpio::output
 * - gpio::inputOutput
 *
 * Derived classes must implement pure virtual embvm::DriverBase functions:
 * - start_()
 * - stop_()
 *
 * @tparam TDir The direction of the GPIO pin (e.g. input, output).
 * @tparam TPull The pull setting for the GPIO pin.
 */
template<gpio::direction TDir>
class base : public embvm::DriverBase
{
  protected:
	/** Default constructor.
	 *
	 * Initializes the driver base with the proper typeid.
	 */
	base() noexcept : embvm::DriverBase(embvm::DriverType::GPIO) {}

  public:
	/** Check the direction of the GPIO pin.
	 *
	 * @returns The configured direction of this GPIO pin.
	 */
	gpio::direction direction() const noexcept
	{
		return TDir;
	}

	/** GPIO Driver Type ID
	 *
	 * @returns GPIO type ID.
	 */
	static constexpr embvm::DriverType type() noexcept
	{
		return embvm::DriverType::GPIO;
	}

  protected:
	/// Default destructor.
	~base() noexcept = default;

	// embvm::DriverBase function for derived class to implement.
	void start_() noexcept override = 0;

	// embvm::DriverBase function for derived class to implement.
	void stop_() noexcept override = 0;
};

/** This class defines a GPIO input interface.
 *
 * The gpio::input class defines interfaces which are common to all GPIO input objects.
 *
 * To implement a GPIO input driver, derive from this class:
 * @code
 * template<uint8_t TPinID, gpio::pull TPull = gpio::pull::none>
 * class aardvarkGPIOInput final : public gpio::input<TPull>
 * {...};
 * @endcode
 *
 * Derived classes must implement the following functions:
 *
 * - get()
 * - pull_()
 *
 * Derived class start_() must call the pull_() function to set the
 * pull-up/pull-down appropriately.
 *
 * Derived classes must implement pure virtual embvm::DriverBase functions:
 * - start_()
 * - stop_()
 *
 * @tparam TPull The pull setting for the GPIO pin.
 */
template<gpio::pull TPull = gpio::pull::none>
class input : public base<gpio::direction::in>
{
	/// Convenience alias for the associated gpio::base definition.
	using gpio_base = base<gpio::direction::in>;

  protected:
	/// Default constructor.
	input() noexcept = default;

  public:
	/** Get the current GPIO pin state.
	 *
	 * Derived classes must implement the get() function.
	 *
	 * @returns true if the GPIO is set (logic 1), false otherwise (logic 0).
	 */
	virtual bool get() noexcept = 0;

	/** Get the current pull configuration.
	 *
	 * @return The currently configured GPIO pull setting.
	 */
	gpio::pull pull() const noexcept
	{
		return TPull;
	}

  protected:
	/** Set the pull configuration in hardware.
	 *
	 * Derived classes must implement the pull_(gpio::pull p) function.
	 *
	 * @return The currently configured GPIO pull setting.
	 */
	virtual gpio::pull pull_(gpio::pull p) noexcept = 0;

	/// Default destructor.
	~input() noexcept = default;

	// embvm::DriverBase function for derived class to implement.
	void start_() noexcept override = 0;

	// embvm::DriverBase function for derived class to implement.s
	void stop_() noexcept override = 0;
};

/** This class defines a GPIO output interface.
 *
 * The gpio::output class defines interfaces which are common to all GPIO output objects.
 *
 * To implement a GPIO output driver, derive from this class:
 * @code
 * template<uint8_t TPinID, gpio::pull TPull = gpio::pull::none>
 * class aardvarkGPIOOutPut final : public gpio::output<TPull>
 * {...};
 * @endcode
 *
 * Derived classes must implement the following functions:
 *
 * - set()
 *
 * Derived classes must implement pure virtual embvm::DriverBase functions:
 * - start_()
 * - stop_()
 */
class output : public base<gpio::direction::out>
{
	/// Convenience alias for the associated gpio::base definition.
	using gpio_base = base<gpio::direction::out>;

  protected:
	/// Default constructor
	output() noexcept = default;

  public:
	/** Set the current GPIO pin state.
	 *
	 * Derived classes must implement the set() function.
	 *
	 * @param v The desired GPIO state. True for logic 1, false for logic 0.
	 */
	virtual void set(bool v) noexcept = 0;

  protected:
	/// Default destructor
	~output() noexcept;

	// embvm::DriverBase function for derived class to implement.
	void start_() noexcept override = 0;

	// embvm::DriverBase function for derived class to implement.
	void stop_() noexcept override = 0;
};

/** This class defines a GPIO input/output interface.
 *
 * The gpio::inputOutput class defines interfaces which are common to all GPIO input/output objects.
 *
 * To implement a GPIO input/output driver, derive from this class:
 * @code
 * template<uint8_t TPinID, gpio::pull TPull = gpio::pull::none>
 * class aardvarkGPIOinputOutput final : public gpio::inputOutput<TPull>
 * {...};
 * @endcode
 *
 * Derived classes must implement the following functions:
 *
 * - get()
 * - set()
 * - pull_()
 *
 * Derived class start_() must call the pull_() function to set the
 * pull-up/pull-down appropriately.
 *
 * Derived classes must implement pure virtual embvm::DriverBase functions:
 * - start_()
 * - stop_()
 *
 * @tparam TPull The pull setting for the GPIO pin.
 */
template<gpio::pull TPull = gpio::pull::none>
class inputOutput : public base<gpio::direction::inout>
{
	/// Convenience alias for the associated gpio::base definition.
	using gpio_base = base<gpio::direction::inout>;

  protected:
	/// Default constructor.
	inputOutput() noexcept = default;

  public:
	/** Set the current GPIO pin state.
	 *
	 * Derived classes must implement the set() function.
	 *
	 * @param v The desired GPIO state. True for logic 1, false for logic 0.
	 */
	virtual void set(bool v) noexcept = 0;

	/** Get the current GPIO pin state.
	 *
	 * Derived classes must implement the get() function.
	 *
	 * @returns true if the GPIO is set (logic 1), false otherwise (logic 0).
	 */
	virtual bool get() noexcept = 0;

	/** Get the current pull configuration.
	 *
	 * @return The currently configured GPIO pull setting.
	 */
	gpio::pull pull() const noexcept
	{
		return TPull;
	}

  protected:
	/** Set the pull configuration in hardware.
	 *
	 * Derived classes must implement the pull_() function.
	 *
	 * @return The currently configured GPIO pull setting.
	 */
	virtual gpio::pull pull_(gpio::pull p) noexcept = 0;

	/// Default destructor.
	~inputOutput() noexcept = default;

	// embvm::DriverBase function for derived class to implement.
	void start_() noexcept override = 0;

	// embvm::DriverBase function for derived class to implement.
	void stop_() noexcept override = 0;
};

/// Alias for inheriting from gpio::base with a special function pin
using specialFunction = embvm::gpio::base<embvm::gpio::direction::special>;

} // namespace gpio

/// @}
// End group

} // namespace embvm

#endif // GPIO_HPP_
