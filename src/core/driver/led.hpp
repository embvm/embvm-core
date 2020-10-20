#ifndef LED_DRIVER_HPP_
#define LED_DRIVER_HPP_

#include "driver.hpp"
#include "gpio.hpp"

namespace embvm
{
/// @addtogroup FrameworkDriver
/// @{

/// Definitions, functions, and classes related to LED devices.
namespace led
{
/** This class provides LED interface definitions.
 *
 * The led::base class defines the common interfaces for LED drivers. This class
 * can be inherited to provide specific LED implementations, such as GPIO, port-pin,
 * LED IC, etc.
 *
 * # Define an LED driver
 *
 * To create a specialized LED interface, derive from the led::base object:
 *
 * @code
 * class gpio final : public base
 * { ... };
 * @endcode
 *
 * Derived classes must implement the following interfaces:
 * - on()
 * - off()
 * - toggle()
 *
 * Derived classes must also implement pure virtual embvm::DriverBase functions:
 * - start_()
 * - stop_()
 *
 * # Using an LED driver
 *
 * The primary means of working with LED driver types is the on(), off(), and toggle() functions.
 *
 * Any configuration needed for LED driver specializations should be handled in the derived classes
 * (preferably in the start_() and stop_() functions).
 * The generic interface has no knowledge of configuration requirements.
 */
class base : public embvm::DriverBase
{
  protected:
	/** Default constructor.
	 *
	 * Initializes the led::base instance with a generic name.
	 */
	base() noexcept : embvm::DriverBase("Generic LED", embvm::DriverType::LED) {}

	/** Construct an led::base with a C-string name.
	 *
	 * Initializes the led::base instance with a name.
	 *
	 * @param name The name of the LED instance.
	 */
	explicit base(const char* name) noexcept : embvm::DriverBase(name, embvm::DriverType::LED) {}

	/** Construct an led::base with a std::string name.
	 *
	 * Initializes the led::base instance with a name.
	 *
	 * @param name The name of the LED instance.
	 *	led::base() uses a std::string_view, so the std::string must remain valid
	 */
	explicit base(const std::string& name) noexcept
		: embvm::DriverBase(name, embvm::DriverType::LED)
	{
	}

	/** Construct an led::base with a std::string_view name.
	 *
	 * Initializes the led::base instance with a name.
	 *
	 * @param name The name of the LED instance.
	 */
	explicit base(const std::string_view& name) noexcept
		: embvm::DriverBase(name, embvm::DriverType::LED)
	{
	}

  public:
	/** LED Driver Type ID
	 *
	 * @returns LED type ID.
	 */
	static constexpr embvm::DriverType type() noexcept
	{
		return embvm::DriverType::LED;
	}

	/// Turn on the LED.
	/// @post LED is turned on.
	virtual void on() noexcept = 0;

	/// Turn off the LED.
	/// @post LED is turned off.
	virtual void off() noexcept = 0;

	/** Toggle the LED state.
	 *
	 * If the LED is on, turn it off. If the LED is off, turn it on.
	 * The logic for implementing toggle is left to the base class, as there may be
	 * efficient options that depend on the specific implementation.
	 *
	 * Derived classes must implement the toggle() function.
	 *
	 * @post If the LED was off, the LED is now on. If the LED was on, LED is now off.
	 */
	virtual void toggle() noexcept = 0;

  protected:
	/// Default destructor.
	~base() noexcept;

	// embvm::DriverBase function that derived classes must implement
	void start_() noexcept override = 0;

	// embvm::DriverBase function that derived classes must implement
	void stop_() noexcept override = 0;
};

/** This class manages a GPIO object which is attached to an LED.
 *
 * This class provides a specialization of the led::base() object. The led::gpio()
 * class manages a embvm::gpio::output object that is connected to an LED in hardware. All
 * interactions with the LED are forwarded to the embvm::gpio::output.
 *
 * This class is used in the same way as led::base(). The only additonal requirement is that it
 * must be initialized with a embvm::gpio::output object. This driver assumes that is is the sole
 * owner of the embvm::gpio::output object after creation. If other code messes with the
 * embvm::gpio::output state outside of this interface, proper functioning is no longer guaranteed.
 *
 * @tparam TActiveHigh If true, the LED turns on when the GPIO is set high. If false, the LED turns
 *on when the GPIO is set low.
 */
template<bool TActiveHigh = true>
class gpio final : public base
{
  public:
	/** Default constructor.
	 *
	 * Initializes the led::gpio() instance with a GPIO output object to manage.
	 *
	 * @param g The embvm::gpio::output object which the LED driver will manage.
	 */
	explicit gpio(embvm::gpio::output& g) noexcept : g_(g) {}

	/** Construct an led::gpio() with a C-string name.
	 *
	 * Initializes the led::gpio() instance with a GPIO output object to manage and uses
	 * the specified name.
	 *
	 * @param g The embvm::gpio::output object which the LED driver will manage.
	 * @param name The name of the LED instance.
	 */
	explicit gpio(embvm::gpio::output& g, const char* name) noexcept : base(name), g_(g) {}

	/** Construct an led::gpio() with a std::string name.
	 *
	 * Initializes the led::gpio() instance with a GPIO output object to manage and uses
	 * the specified name.
	 *
	 * @param g The embvm::gpio::output object which the LED driver will manage.
	 * @param name The name of the LED instance.
	 */
	explicit gpio(embvm::gpio::output& g, const std::string& name) noexcept : base(name), g_(g) {}

	/** Construct an led::gpio() with a std::string_view name.
	 *
	 * Initializes the led::gpio() instance with a GPIO output object to manage and uses
	 * the specified name.
	 *
	 * @param g The embvm::gpio::output object which the LED driver will manage.
	 * @param name The name of the LED instance.
	 *	led::base() uses a std::string_view, so the std::string must remain valid
	 */
	explicit gpio(embvm::gpio::output& g, const std::string_view& name) noexcept : base(name), g_(g)
	{
	}

	/// Default destructor
	~gpio() = default;

	void start_() noexcept final
	{
		g_.start();
		off();
	}

	void stop_() noexcept final
	{
		g_.stop();
	}

	void on() noexcept final
	{
		g_.set(TActiveHigh);
		on_ = true;
	}

	void off() noexcept final
	{
		g_.set(!TActiveHigh);
		on_ = false;
	}

	void toggle() noexcept final
	{
		if(on_)
		{
			off();
		}
		else
		{
			on();
		}
	}

  private:
	/// The GPIO output object which this LED driver is managing.
	embvm::gpio::output& g_;

	/// State of the LED, required to implement toggle().
	bool on_ = false;
};

/// Alias for declaring an led::gpio that is active high
using gpioActiveHigh = gpio<true>;

/// Alias for declaring an led::gpio that is active low
using gpioActiveLow = gpio<false>;

} // namespace led

/// @}
// End group

} // namespace embvm

#endif // LED_DRIVER_HPP_
