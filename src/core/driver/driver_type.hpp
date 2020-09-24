#ifndef DRIVER_TYPE_HPP_
#define DRIVER_TYPE_HPP_

#include <cstdint>

namespace embvm
{
/// @addtogroup FrameworkDriver
/// @{

/** Driver type used by framework interfaces.
 *
 * The driver type ID is stored as a uint32_t rather than a DriverType to work with custom
 * user-defined types. Enforcing a DriverType return value would prevent clients
 * from defining and using their own custom types enums and would create template headaches.
 */
using DriverType_t = uint32_t;

/** Default framework driver type definitions
 *
 * Default framework driver type definitions. These definitions are defined because
 * the framework provides driver interfaces for these types. Additional types can be
 * defined by clients as needed.
 *
 * To add your own driver types, create an enum in your project code and use DRIVER_EXTENSION_START
 * as the first value
 */
enum DriverType
{
	/// The type of the driver is undefined, no assumptions can be made about it.
	Undefined = 0,
	/// Represents a GPIO peripheral
	GPIO,
	/// Represents a hardware timer peripheral
	HARDWARE_TIMER,
	/// Represents an I2C communication bus
	I2C,
	/// Represents an LED driver
	LED,
	/// Represents a SPI communication bus
	SPI,
	/// Represents a master system clock, which can be a hardware device or software interface
	SYSTEM_CLOCK,
	/// Represents a Time of Flight Range-finding sensor
	TIME_OF_FLIGHT,
	/// Represents a software timer
	SOFTWARE_TIMER,
	/// Represents a hardware timer
	TIMER,
	/// Represents a basic display without a range of colors, such as a small 2-color OLED display
	BASIC_DISPLAY,
	/// To extend with your own driver types, start the count using this value
	DRIVER_EXTENSION_START
};

/// @}
// End group

} // namespace embvm

#endif // DRIVER_TYPE_HPP_
