// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#ifndef SPI_DRIVER_HPP_
#define SPI_DRIVER_HPP_

#include "driver.hpp"
#include <cstdint>
#include <driver/communication_bus.hpp>
#include <inplace_function/inplace_function.hpp>

// TODO: we have spi master, spi slave
// TODO: add baud tolerance option to configure

namespace embvm
{
/// @addtogroup FrameworkDriver
/// @{

/// Definitions, functions, and classes related to SPI communication bus devices.
namespace spi
{
/// SPI baudrate is specified as an integral value with units Hz.
using baud_t = uint32_t;

using status = embvm::comm::status;

/// Maximum size of the SPI master callback functor object.
static constexpr size_t SPI_MASTER_REQD_STATIC_FUNCTION_SIZE = 80;

/// Default baudrate, which is used in the default constructor.
static constexpr spi::baud_t DEFAULT_SPI_BAUD = UINT32_C(125000);

/// SPI bus operational modes.
enum class mode : uint8_t
{
	/// Clock polarity normal, samples on rising edge
	mode0 = 0,
	/// Clock polarity normal, samples on falling edge
	mode1 = 1,
	/// Clock polarity inverted, samples on rising edge
	mode2 = 2,
	/// Clock polarity inverted, samples on falling edge
	mode3 = 3
};

/** SPI bus transmission order options.
 *
 * This setting determines whether the bus transmit MSB or LSB first.
 *
 * msbFirst is the most common setting.
 */
enum class order : uint8_t
{
	msbFirst = 0,
	lsbFirst
};

/** SPI operation definition.
 *
 * The SPI bus is full-duplex and always transmits and receives simultaneously.
 *
 * A TX operation involves a valid tx_buffer pointer with rx_buffer set to nullptr.
 *
 * An RX operation involves a valid rx_buffer pointer with tx_buffer set to nullptr.
 *	When tx_buffer is set to nullptr, the driver should clock out empty bytes.
 *
 * An equal number of bytes are transmitted and received during a SPI transaction.
 * The transfer size is controlled by the length argument.
 */
struct op_t
{
	/** Pointer to buffer holding transmit bytes.
	 * Can be set to nullptr to transmit empty bytes.
	 */
	uint8_t* tx_buffer{nullptr};
	/** Pointer to buffer holding receive bytes.
	 * Can be set to nullptr to ignore reception.
	 */
	uint8_t* rx_buffer{nullptr};
	/// Number of bytes to transfer over the bus.
	size_t length{0};
};

/// Convenience alias for the declaration of the SPI commBus.
using commBus = commBus<spi::op_t, spi::baud_t, spi::SPI_MASTER_REQD_STATIC_FUNCTION_SIZE>;

/** This class provides the SPI master interface definition.
 *
 * This class defines common interfaces and types for SPI master devices.
 *
 * ## Implementing a spi::master Driver
 *
 * To implement your own SPI master device, simply inherit from this class type:
 *
 * @code
 * class aardvarkSPIMaster final : public spi::master
 * {...}
 * @endcode
 *
 * Derived classes must implement the following pure virtual functions to complete SPI master
 * behavior:
 * - setOrder_(), to set byte ordering
 * - setMode_(), to set bus mode
 * - configure_(), to configure the target hardware for operation
 *
 * Derived classes must also implement pure virtual functions required by the comm bus, such as:
 * - transfer_(), which initiates a bus transfer
 * - baudrate_(), which configures the baudrate
 *
 * ## Using a spi::master Driver
 *
 * spi::master usage follows this general pattern:
 *	1. Declare a spi::master device
 *  2. Configure the device
 *	3. Initialize a transfer
 *
 * ### Declare a spi::master device
 *
 * Derived classes will have their own specific constructor schemes. In all cases you will use
 * a derived class for the declaration.
 *
 * @code
 * aardvarkAdapter aardvark{aardvarkMode::SPI};
 * aardvarkSPIMaster d{aardvark, "spi0"};
 * @endcode
 *
 * ### Configure a spi::master device
 *
 * After the device has been declared, the object can be used to configure
 * the SPI bus. Initial configuration will happen with the configure() function,
 * which sets up the SPI device with the target baudrate.
 *
 * The mode() function is used to change the SPI bus mode (spi::mode). The
 * order() function can be used to change the order bytes are sent over the bus (spi::order).
 *
 * ### Initialize a Transfer
 *
 * All transfers require an op structure instance with the proper settings:
 *
 * @code
 * uint8_t input[2] = {0xAA, 0xBB};
 * uint8_t output[2];
 * spi::op_t op = {input, output, 2};
 * @endcode
 *
 * You should also prepare a callback function to handle the transfer completion:
 *
 * @code
 * void spi_callback(spi::op_t input, embvm::comm::status status)
 * {
 * ...
 * }
 * @endcode
 *
 * To initiate a transfer, call transfer() with the op and callback:
 *
 * @code
 * auto err = d.transfer(op, spi_callback);
 * @endcode
 *
 * When the transfer completes, the callback function will be called with a copy
 * of the op struct and the result of the transfer.
 *
 * ### Using with a Dispatch Queue
 *
 * commBus devices support dispatching callbacks. You can construct a spi::master instance with
 * a dispatcher function:
 *
 * @code
 * embutil::DynamicDispatchQueue<> dispatch;
 * spi0(dispatch.getBoundDispatch());
 * @endcode
 *
 * If a dispatcher has been configured, callbacks will be invoked on the dispatch thread instead
 * of the spi::master's local thread. We recommend using dispatchers when possible for improved
 * responsiveness.
 */
class master : public embvm::DriverBase, public spi::commBus
{
  protected:
	/** Default constructor.
	 *
	 * Initializes the spi::master device with an optional dispatcher.
	 *
	 * @param dispatcher The functor which callbacks will be dispatched to. This will often
	 *	be paired with a dispatch queue. Example:
	 *
	 *	```
	 * 	embutil::DynamicDispatchQueue<> dispatch;
	 *	spi0(dispatch.getBoundDispatch());
	 *	```
	 */
	explicit master(const spi::commBus::DispatcherFunc& dispatcher = nullptr) noexcept
		: embvm::DriverBase(embvm::DriverType::SPI), spi::commBus(spi::DEFAULT_SPI_BAUD, dispatcher)
	{
	}

	/// Default destructor.
	~master() noexcept;

  public:
	/** Configure the SPI bus.
	 *
	 * Configures the SPI bus for operation and sets the target baud rate.
	 *
	 * Derived classes must implement the configure_() function to set up the
	 * target hardware.
	 *
	 * @param baud The target baudrate, in Hz.
	 *	TODO: Tolerance - allowed relative tolerance for the resulting baudrate
	 */
	virtual void configure(spi::baud_t baud) noexcept
	{
		configure_();
		baudrate(baud);
	}

	/** Get the current SPI bus mode.
	 *
	 * @returns The currently configured mode.
	 */
	spi::mode mode() const noexcept
	{
		return mode_;
	}

	/** Set the SPI bus mode.
	 *
	 * Derived classes must implement the setMode_() function,
	 * which is responsible for configuring the target hardware.
	 *
	 * @param mode The desired SPI bus mode.
	 * @returns The new SPI bus mode.
	 */
	spi::mode mode(spi::mode mode) noexcept
	{
		setMode_(mode);
		mode_ = mode;

		return mode_;
	}

	/** Get the current SPI bus byte order.
	 *
	 * @returns The currently configured byte order.
	 */
	spi::order order() const noexcept
	{
		return order_;
	}

	/** Set the SPI bus byte order.
	 *
	 * Derived classes must implement the setOrder_() function, which is responsible for
	 * configuring the target hardware.
	 *
	 * @param order The desired byte ordering.
	 * @returns The new byte ordering.
	 */
	spi::order order(spi::order order) noexcept
	{
		setOrder_(order);
		order_ = order;

		return order_;
	}

	/** SPI Driver Type ID
	 *
	 * @returns SPI type ID.
	 */
	static constexpr embvm::DriverType type() noexcept
	{
		return embvm::DriverType::SPI;
	}

  protected:
	/** Set the SPI bus mode.
	 *
	 * This function is implemented by the derived class. It is responsible for
	 * configuring the target hardware to use the new mode.
	 *
	 * @param mode The desired byte ordering.
	 */
	virtual void setMode_(spi::mode mode) noexcept = 0;

	/** Set the SPI bus byte order.
	 *
	 * This function is implemented by the derived class. It is responsible for
	 * configuring the target hardware to use the new byte ordering.
	 *
	 * @param order The desired byte ordering.
	 */
	virtual void setOrder_(spi::order order) noexcept = 0;

	/** Configure the SPI bus for operation.
	 *
	 * This function is implemented by the derived class. It is responsible for
	 * configuring the target hardware for operation.
	 */
	virtual void configure_() noexcept = 0;

	// embvm::DriverBase function for derived class to implement.
	void start_() noexcept override = 0;

	// embvm::DriverBase function for derived class to implement.
	void stop_() noexcept override = 0;

	// commBus function for derived class to implement.
	spi::baud_t baudrate_(spi::baud_t baud) noexcept override = 0;

	// commBus function for derived class to implement.
	embvm::comm::status transfer_(const spi::op_t& op, const cb_t& cb) noexcept override = 0;

  protected:
	/// Stores the active SPI mode configuration.
	spi::mode mode_ = spi::mode::mode0;

	/// Stores the active SPI byte ordering.
	spi::order order_ = spi::order::msbFirst;
};

} // namespace spi

/// @}
// End group

} // namespace embvm

#endif // SPI_DRIVER_HPP_
