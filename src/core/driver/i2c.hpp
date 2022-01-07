// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#ifndef I2C_DRIVER_HPP_
#define I2C_DRIVER_HPP_

#include "driver.hpp"
#include <cstdint>
#include <driver/communication_bus.hpp>
#include <etl/vector.h>
#include <inplace_function/inplace_function.hpp>

// Concepts: i2c Bus, I2c master, I2C slave
#if 0 // APIs under consideration
	/// Send one-byte write to slave
	virtual void sendByte(i2c::addr_t addr, uint8_t data) = 0;

	/// Receive one byte from slave
	virtual void receiveByte(i2c::addr_t addr, uint8_t& data) = 0;

	/// Write a byte with a command code or slave register address
	virtual void writeByte(i2c::addr_t addr, uint8_t cmd, uint8_t data) = 0;

	/// Read a byte with a command code or slave register address
	virtual void readByte(i2c::addr_t addr, uint8_t cmd, uint8_t& data) = 0;

	/// Write a stream of bytes
	virtual void writeBytes(i2c::addr_t addr, uint8_t cmd, uint8_t* bytes, size_t len) = 0;

	/// Read a stream of bytes. Return number of bytes actually read from slave (<= len)
	virtual size_t readBytes(i2c::addr_t addr, uint8_t cmd, uint8_t* bytes, size_t len) = 0;
#endif

namespace embvm
{
/// @addtogroup FrameworkDriver
/// @{

/// Definitions, functions, and classes related to I2C communication bus devices.
namespace i2c
{
/// Maximum size of the I2C master callback functor object.
static constexpr size_t I2C_MASTER_REQD_STATIC_FUNCTION_SIZE = 96;

/// I2C address storage type.
using addr_t = uint8_t;
struct op_t;

/// Added to slave address to indicate a read command.
static constexpr uint8_t ReadBit = 0x01;

/** I2C operation types.
 *
 * The operation enum defines the type of transfer that the op_t structure
 * is to perform.
 */
enum class operation : uint8_t
{
	stop = 0, /// Generate a stop condition
	restart, /// Generate a bus restart

	/// Write to slave
	/// Performs the following sequence: start - address - write - stop
	write,

	/// Read from slave
	/// Performs the following sequence: start - address - read - stop
	read,

	/// write to slave and then read from slave
	/// Performs the following sequence: start - address - write - restart - address - read -
	/// stop
	writeRead,

	/// start a write to the slave and but do not issue a stop
	/// Performs the following sequence: start - address - write
	writeNoStop,

	/// continue a write to the slave and do not stop the transaction
	/// Performs the following sequence: write
	continueWriteNoStop,

	/// continue a write to the slave and then stop the transaction
	/// Performs the following sequence: write - stop
	continueWriteStop,

	/// Send only address to see if there is an ACK
	/// Performs the following sequence: start - address
	ping,
};

/// Represents the state of the I2C bus.
enum class state : uint8_t
{
	/// The bus is idle and ready to receive work.
	idle = 0,
	/// The bus is currently busy executing a transaction.
	busy,
	/// A bus error occurred and needs to be corrected.
	error,
};

/** I2C bus status.
 *
 * This enumeration represents the status of an I2C bus. This type extends the default
 * embvm::comm::status definition with additional I2C-specific statuses.
 */
enum class status
{
	/// The operation completed successfully.
	ok = 0,
	/// The operation was enqueued for later processing.
	enqueued,
	/// The device cannot handle the request at this time; try again.
	busy,
	/// An error occurred with this transaction.
	error,
	/// The success/failure status of the transaction could not be determined.
	unknown,

	// Additional i2c errors

	/// Addr was transmitted, NACK received.
	addrNACK,

	/// Data was transmitted, NACK received.
	dataNACK,

	/// Bus error, such as missing start/stop condition.
	bus,
};

/// Valid I2C bus baudrate options.
/// Numeric values are in Hz.
enum class baud
{
	lowSpeed = 10000,
	standard = 100000,
	// TODO: add 250K? supported by Nordic
	fast = 400000
};

/// Pull-up configuration options for an I2C master device.
enum class pullups : uint8_t
{
	/// External pullups exist for SDA and SCL.
	external = 0,
	/// Use internal pull-ups on SDA and SCL (if supported by the device).
	internal
};

/** I2C operation definition.
 *
 * The I2C bus operates in half-duplex mode, meaning that reads and writes do not occur
 * simultaneously. The operation type represents a variety of operations which can be
 * performed by an I2C device. The transmit options (tx_buffer and tx_size) are required by any
 * operations using "write". The receive options (rx_buffer and rx_size) are required by any
 * operations using "read".
 */
struct op_t
{
	/// Slave device address (7-bit).
	addr_t address{0};

	/// Operation to perform.
	operation op{operation::stop};

	/// Pointer to the transmit buffer.
	/// Does not need to be specified for read-only commands.
	const uint8_t* tx_buffer{nullptr};

	/// Number of bytes to transmit.
	/// Does not need to be specified for read-only commands.
	size_t tx_size{0};

	/// Pointer to the receive buffer.
	/// Does not need to be specified for write-only commands.
	uint8_t* rx_buffer{nullptr};

	/// Number of bytes to receive.
	/// Does not need to be specified for write-only commands.
	size_t rx_size{0};
};

/// Convenience alias for the declaration of the I2C commBus.
using commBus =
	commBus<i2c::op_t, i2c::baud, i2c::I2C_MASTER_REQD_STATIC_FUNCTION_SIZE, i2c::status>;

/** This class provides the I2C master interface definition.
 *
 * This class defines common interfaces and types for I2C master devices.
 *
 * ## Implementing a i2c::master Driver
 *
 * To implement your own I2C master device, simply inherit from this class type:
 *
 * @code
 * class aardvarkI2CMaster final : public i2c::master
 * {...}
 * @endcode
 *
 * Derived classes must implement the following pure virtual functions to complete I2C master
 * behavior:
 * - configure_(), to configure the target hardware for operation
 *
 * Derived classes must also implement pure virtual functions required by the comm bus, such as:
 * - transfer_(), which initiates a bus transfer
 * - baudrate_(), which configures the baudrate
 *
 * ## Using a i2c::master Driver
 *
 * i2c::master usage follows this general pattern:
 *	1. Declare a i2c::master device
 *  2. Configure the device
 *	3. Initialize a transfer
 *
 * ### Declare a i2c::master device
 *
 * Derived classes will have their own specific constructor schemes. In all cases you will use
 * a derived class for the declaration.
 *
 * @code
 * aardvarkAdapter aardvark{aardvarkMode::I2C};
 * aardvarkI2CMaster d{aardvark, "i2c0"};
 * @endcode
 *
 * ### Configure a i2c::master device
 *
 * After the device has been declared, the object can be used to configure
 * the I2C bus. Initial configuration will happen with the configure() function,
 * which sets up the I2C device with the target baudrate and pull-up option.
 *
 * ### Initialize a Transfer
 *
 * All transfers require an op structure instance with the proper settings:
 *
 * @code
 * i2c::op_t t;
 * t.op = i2c::operation::writeRead;
 * t.address = address_;
 * t.tx_size = sizeof(uint16_t);
 * t.tx_buffer = reinterpret_cast<const uint8_t*>(reg_buf);
 * t.rx_size = rx_size;
 * t.rx_buffer = rx_buffer;
 * @endcode
 *
 * You should also prepare a callback function to handle the transfer completion:
 *
 * @code
 * void i2c_callback(i2c::op_t input, i2c::status status)
 * {
 * ...
 * }
 * @endcode
 *
 * To initiate a transfer, call transfer() with the op and callback:
 *
 * @code
 * auto err = d.transfer(t, i2c_callback);
 * @endcode
 *
 * When the transfer completes, the callback function will be called with a copy
 * of the op struct and the result of the transfer.
 *
 * ### Using with a Dispatch Queue
 *
 * commBus devices support dispatching callbacks. You can construct a i2c::master instance with
 * a dispatcher function:
 *
 * @code
 * embutil::DynamicDispatchQueue<> dispatch;
 * i2c0(dispatch.getBoundDispatch());
 * @endcode
 *
 * If a dispatcher has been configured, callbacks will be invoked on the dispatch thread instead
 * of the i2c::master's local thread. We recommend using dispatchers when possible for improved
 * responsiveness.
 */
class master : public embvm::DriverBase, public i2c::commBus
{
  public:
	/// Represents the type of the callback operation.
	using cb_t = stdext::inplace_function<void(i2c::op_t, i2c::status)>;
	using sweep_list_t = etl::vector<uint8_t, 128>;
	using sweep_cb_t = stdext::inplace_function<void(void)>;

  protected:
	/** Default constructor.
	 *
	 * Initializes the i2c::master device with an optional dispatcher function
	 *
	 * @param dispatcher The functor which callbacks will be dispatched to. This will often
	 *	be paired with a dispatch queue. Example:
	 *
	 *	```
	 * 	embutil::DynamicDispatchQueue<> dispatch;
	 *	i2c0(dispatch.getBoundDispatch());
	 *	```
	 */
	explicit master(const i2c::commBus::DispatcherFunc& dispatcher = nullptr) noexcept
		: embvm::DriverBase(embvm::DriverType::I2C), i2c::commBus(i2c::baud::standard, dispatcher)
	{
	}

	/// Default destructor
	~master() noexcept = default;

  public:
	// TODO: refactor this call - it doesn't make sense...
	// Why do we have configure take in pullup settings, and also call pullups?
	// We probably just need to have 1) configure, or 2) baudrate+pullups.
	// Maybe this stuff is even just set by the constructor...
	/** Configure the I2C bus.
	 *
	 * Configures the I2C bus for operation and sets the target baud rate and pull-ups.
	 *
	 * Derived classes must implement the configure_() function to set up the
	 * target hardware.
	 *
	 * @param baud The target baudrate (as an i2c::baud enumeration rather than integral Hz).
	 *	TODO: Tolerance - allowed relative tolerance for the resulting baudrate
	 * @param pull The pullup setting, which defaults to external pull-ups (in hardware).
	 */
	virtual void configure(i2c::baud baud, i2c::pullups pull = i2c::pullups::external) noexcept
	{
		configure_(pull);
		baudrate(baud);
		pullups(pull);
	}

	/** I2C Driver Type ID
	 *
	 * @returns I2C type ID.
	 */
	static constexpr auto type() noexcept -> embvm::DriverType
	{
		return embvm::DriverType::I2C;
	}

	/** Check the I2C bus status.
	 *
	 * @returns the status of the I2C bus as an i2c::state enumeration.
	 */
	[[nodiscard]] auto state() const noexcept -> i2c::state
	{
		return state_;
	}

	/** Configure pull-ups.
	 *
	 * @param pullups The target pull-up setting.
	 * @returns The configured pull-up setting.
	 */
	auto pullups(i2c::pullups pullups) noexcept -> i2c::pullups
	{
		pullups_ = setPullups_(pullups);

		return pullups_;
	}

	/** Check the pull-up configuration.
	 *
	 * @returns The configured pull-up setting.
	 */
	[[nodiscard]] auto pullups() const noexcept -> i2c::pullups
	{
		return pullups_;
	}

	// TODO: this doesn't work with enqueuing & callback model... need a new method?
	// need to have sweep itself return a callback
	/** Perform an I2C bus sweep to identify active devices.
	 *
	 * The sweep function pings all I2C addresses. Devices which ACK are stored in a list
	 * and returned via callback.
	 *
	 * @param[in,out] found_list Caller's memory which will contain the successfully found ping
	 *addresses
	 * @param[in] cb The callback which will be called to indicate that the sweep is complete.
	 *	After the cb is called, found_list is valid and can be used by the caller.
	 */
	void sweep(sweep_list_t& found_list, const sweep_cb_t& cb) noexcept;

  protected:
	// embvm::DriverBase function for derived class to implement.
	void start_() noexcept override = 0;

	// embvm::DriverBase function for derived class to implement.
	void stop_() noexcept override = 0;

	/** Configure the I2C bus for operation.
	 *
	 * This function is implemented by the derived class. It is responsible for
	 * configuring the target hardware for operation.
	 *
	 * @param pullups The target pull-up configuration.
	 */
	virtual void configure_(i2c::pullups pullups) noexcept = 0;

	// commBus function for derived class to implement.
	auto baudrate_(i2c::baud baud) noexcept -> i2c::baud override = 0;

	// commBus function for derived class to implement.
	auto transfer_(const i2c::op_t& op, const cb_t& cb) noexcept -> i2c::status override = 0;

	/** Configure pull-ups.
	 *
	 * This function is implemented by the derived class. It is responsible for
	 * configuring the target hardware to use the requested pullup setting.
	 *
	 * @param pullups The target pull-up setting.
	 * @returns The configured pull-up setting.
	 */
	virtual auto setPullups_(i2c::pullups pullups) noexcept -> i2c::pullups = 0;

	/// Tracks the active pull-up configuration.
	i2c::pullups pullups_ = i2c::pullups::external;
	/// Tracks the status of the I2C bus.
	i2c::state state_ = i2c::state::idle;
};

} // namespace i2c

/// @}
// End group

} // namespace embvm

#endif // I2C_DRIVER_HPP_
