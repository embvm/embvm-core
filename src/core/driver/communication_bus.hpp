// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#ifndef COMMUNICATION_BUS_HPP_
#define COMMUNICATION_BUS_HPP_

#include <cstdint>
#include <inplace_function/inplace_function.hpp>

namespace embvm
{
/// @addtogroup FrameworkDriver
/// @{

/// Definitions, functions, and classes related to communication busses.
namespace comm
{
/** Generic communication bus status.
 *
 * This enumeration represents the status of a communication bus. These states
 * are generally found in a variety of communication busses. If they do not apply
 * to the bus you are developing, feel free to ignore them.
 *
 * You can create your own bus status enumeration for use with the commBus class instead
 * of using the default. However, the types represented here must be included because they are used
 * automatically by the driver. See i2c.hpp for an example.
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
	unknown
};

} // namespace comm

// TODO: update to a function queue for storage
/** Base class interface for communication bus driver types (SPI, I2C, etc.).
 *
 * This class defines common interfaces and types for communication bus drivers, such as SPI, I2C,
 * I2S.
 *
 * ## Implementing a Communication Bus Driver
 *
 * To implement your own communication bus driver, derive from this class using template parameters
 * that represent the implementation of your bus type. For example:
 *
 * ```
 * namespace spi {
 *	static constexpr size_t SPI_MASTER_REQD_STATIC_FUNCTION_SIZE = (sizeof(void*) * 12);
 *  using baud_t = uint32_t;
 *
 *  struct op_t
 *	{
 *		uint8_t* tx_buffer;
 *		uint8_t* rx_buffer;
 *		size_t length;
 *		comm::status status;
 *	};
 *
 *	using cb_t = stdext::inplace_function<void(op_t)>;
 *
 *  using commBus = commBus<embvm::spi::op_t, embvm::spi::baud_t,
 *embvm::spi::SPI_MASTER_REQD_STATIC_FUNCTION_SIZE>;
 * };
 *
 * class embvm::spi::master : public embvm::DriverBase, public embvm::spi::commBus
 * { ... }
 * ```
 *
 * Derived classes must implement the following pure virtual functions to complete comm bus
 * behavior:
 * - transfer_(), which initiates a bus transfer
 * - baudrate_(), which configures the baudrate
 *
 * Derived classes may implement additional functionality, especially related to initialization
 * and configuration of the bus. However, all exchange of data should be handled within the
 * transfer() function.
 *
 * **Note:** Derived classes are responsible for calling callback() once a transfer is completed.
 *
 * ## Using a Communication Bus Driver
 *
 * Communication busses are based around the concept of a transfer(). A transfer operation
 * is defined using the TOperation type.
 *
 * To initiate a transfer, you need to define a TOperation object. Because the framework uses
 * asynchronous transfers, a callback function (cb_t) is also needed so the completion and result
 * of the transfer can be reported. The callback can be any functor.
 *
 * The TOperation object and the callback function are passed into transfer(). When the operation
 * completes, the derived driver will invoke callback() with a copy of the TOperation object and
 * th resulting status.
 *
 * Communication bus drivers are non-copyable and non-moveable.
 *
 * @tparam TOperation Defines the operation type for the bus. This type represents the data
 *	that will be enqueued by the driver and used to carry out a transfer. The operation is
 *	also passed to a callback upon completion of a transfer. The operation type is often a
 *	structure, for example:
 *	```
 *	struct op_t
 *	{
 *		uint8_t* tx_buffer;
 *		uint8_t* rx_buffer;
 *		size_t length;
 *		comm::status status;
 *	};
 *	```
 * @tparam TBaudrate The baudrate storage type. The baudrate type defaults to uint32_t.
 *	Baudrate can be set to another type, such as embvm::i2c::baud, which is an `enum class`.
 * @tparam TDispatchFunctorSize The storage space for statically-allocated functor objects. This
 *	template parameter is used in the TDispatchFunctor declaration. This can be made larger if
 *	required by the storage needs for your status/callback type.
 * @tparam TStatus The comm bus status type. Defaults to comm::status, but can be overridden to
 * 	support custom bus statuses.
 */
template<typename TOperation, typename TBaudrate = uint32_t,
		 size_t TDispatchFunctorSize = sizeof(void*) * 4, typename TStatus = comm::status>
class commBus
{
	/**
	 * The functor declaration which will be used in declaring a DispatcherFunc. This represents the
	 * statically-allocated functor that stores a std::bind of the callback, operation, and status.
	 */
	using TDispatchFunctor = stdext::inplace_function<void(), TDispatchFunctorSize>;

  public:
	/// Represents the type of the callback operation.
	using cb_t = stdext::inplace_function<void(TOperation, TStatus)>;

	/// Alias for the dispatcher function's storage type.
	using DispatcherFunc = stdext::inplace_function<void(TDispatchFunctor&&)>;

  public:
	/** Default constructor.
	 *
	 * Initializes the comm bus status.
	 */
	commBus() noexcept
	{
		set_status_ok_();
	}

	/** Construct the bus with a baudrate.
	 *
	 * Initializes the comm bus status and sets the baudrate to a default value.
	 *
	 * @param baud The default baudrate for the bus.
	 */
	explicit commBus(TBaudrate baud) noexcept : baud_(baud)
	{
		set_status_ok_();
	}

	/** Construct the bus with a baudrate and dispatcher.
	 *
	 * Initializes the comm bus status and sets the baudrate to a default value. Also sets the
	 * dispatcher function, which will forward callbacks to a different thread of control.
	 *
	 * @param baud The default baudrate for the bus.
	 * @param dispatcher The functor which callbacks will be dispatched to. This will often
	 *	be paired with a dispatch queue. Example:
	 *
	 *	```
	 * 	embutil::DynamicDispatchQueue<> dispatch;
	 *	spi0(115200, dispatch.getBoundDispatch());
	 *	```
	 */
	explicit commBus(TBaudrate baud, DispatcherFunc dispatcher) noexcept
		: baud_(baud), dispatcher_(std::move(dispatcher))
	{
		set_status_ok_();
	}

	/// Deleted copy constructor
	commBus(const commBus&) = delete;

	/// Deleted copy assignment operator
	const commBus& operator=(const commBus&) = delete;

	/// Deleted move constructor
	commBus(commBus&&) = delete;

	/// Deleted move assignment operator
	commBus& operator=(commBus&&) = delete;

	/** Initiate a bus transfer.
	 *
	 * Initiates a transfer of information across a comm bus. The transfer function should handle
	 * read, write, and read-write operations. Other operations may also be supported by derived
	 * classes.
	 *
	 * The transfer request is forwarded the derived class, which must implement the pure virtual
	 * function transfer_().
	 *
	 * This call should not block. Transfer requests are asynchronous and are not guaranteed to
	 * return immediately with a result.
	 *
	 * If the derived driver class returns a status code other than TStatus::enqueued or
	 * TStatus::busy, the callback will be immediately invoked. If the operation was enqueued, the
	 * final status will be reported when the callback is called.
	 *
	 * @param op A bus transfer is defined by an operation (op). The derived class will use the
	 *	data supplied in the op to configure the bus and transfer the data.
	 * @param cb Optional callback parameter. When set, the callback will be invoked upon completion
	 *	of the transfer.
	 * @returns The status of the bus transfer.
	 */
	virtual TStatus transfer(TOperation& op, const cb_t& cb = nullptr) noexcept
	{
		auto status = transfer_(op, cb);

		// TODO: remove this
		if(status != TStatus::enqueued && status != TStatus::busy)
		{
			callback(op, status, cb);
		}

		return status;
	}

	/** Get the current bus status.
	 *
	 * @returns The current bus status.
	 */
	TStatus busStatus() const noexcept
	{
		return bus_status_;
	}

	/** Get the current baudrate.
	 *
	 * @returns The currently configured baudrate.
	 */
	TBaudrate baudrate() const noexcept
	{
		return baud_;
	}

	/** Set a new baudrate.
	 *
	 * Since each comm bus driver is different, the baudrate update request is forwarded to
	 * the derived class, which must implement the pure virtual function baudrate_().
	 *
	 * TODO: tolerance check
	 *
	 * @param baud The new baudrate for the bus.
	 * @returns The baudrate the bus is configured to use. The returned baudrate may not match the
	 * 	input under the following conditions:
	 *	- Input baudrate was invalid (no update)
	 *	- Input baudrate was rounded by the bus driver
	 */
	TBaudrate baudrate(TBaudrate baud) noexcept
	{
		baud_ = baudrate_(baud);

		return baud_;
	}

	// TODO: virtual bool send(const std::uint8_t) const;
	// TODO: virtual bool recv(std::uint8_t&);

  protected:
	/// Default destructor.
	~commBus() = default;

	/** Handle callbacks.
	 *
	 * Invokes callback functions, either by pushing to a dispatch thread or by
	 * directly invoking the callback on the same thread of control.
	 *
	 * @param op The operation which was completed. This will be passed to the callback.
	 * @param status The result of the operation.
	 * @param cb The callback function to invoke.
	 */
	void callback(const TOperation& op, TStatus status, const cb_t& cb) noexcept
	{
		if(dispatcher_)
		{
			dispatcher_(std::bind(cb, op, status));
		}
		else if(cb)
		{
			cb(op, status);
		}
	}

	/** The derived comm class's transfer implementation.
	 *
	 * Derived classes override this transfer function to handle specific
	 * transfer operations for each device.
	 *
	 * Callback is passed in for drivers which enqueue operations or use AO model.
	 * The base class handles the callback, so there is no need to worry about invoking
	 * the callback from a client driver.
	 *
	 * Just mark your callback `(void)callback` if you aren't using it.
	 *
	 * To indicate that a transfer has been enqueued for later processing, return
	 * `TStatus::enqueued`
	 */
	virtual TStatus transfer_(const TOperation& op, const cb_t& callback) noexcept = 0;

	/** The derived comm class's baudrate update implementation.
	 *
	 * Derived classes override this baudrate function to handle baudrate change requests.
	 *
	 * @param baud The new baudrate for the bus.
	 * @returns The baudrate the bus is configured to use. The returned baudrate may not match the
	 * 	input under the following conditions:
	 *	- Input baudrate was invalid (no update)
	 *	- Input baudrate was rounded by the bus driver
	 */
	virtual TBaudrate baudrate_(TBaudrate baud) noexcept = 0;

	constexpr void set_status_ok_() noexcept
	{
		if constexpr(std::is_integral<TStatus>::value)
		{
			bus_status_ = 0;
		}
		else
		{
			bus_status_ = TStatus::ok;
		}
	}

  protected:
	/// Stores the current bus status.
	// TODO: make atomic?
	TStatus bus_status_;

	/// Stores the currently configured baudrate.
	TBaudrate baud_;

	/** Stores a function which callbacks will be dispatched to.
	 *
	 * The dispatcher function receives callbacks and ops and executes them on a separate
	 * thread of control from the caller of callback().
	 *
	 * If no dispatcher is configured by the user, the callback will be executed in
	 * the same thread of control.
	 */
	const DispatcherFunc dispatcher_;
};

/// @}
// End group

} // namespace embvm

#endif // COMMUNICATION_BUS_HPP_
