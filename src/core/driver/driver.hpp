// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#ifndef DRIVER_HPP_
#define DRIVER_HPP_

#include "driver_type.hpp"
#include <string>
#include <type_safe/boolean.hpp>

namespace embvm
{
/// @addtogroup FrameworkDriver
/// @{

/** Virtual base class for framework drivers.
 *
 * All framework drivers should include this class in their inheritance tree.
 * The DriverBase provides common interfaces which apply to all drivers.
 *
 * Derived classes must override the following functions:
 * 	- start_()
 * 	- stop_()
 */
class DriverBase
{
  public:
	/** Start the driver.
	 *
	 * If the driver is not started, call the start_() function defined by the derived class.
	 * (Template Method Pattern)
	 */
	void start() noexcept
	{
		if(!started_)
		{
			start_();
			started_ = true;
		}
	}

	/** Stop the driver.
	 *
	 * If the driver has been started, call the stop_() function defined by the derived class.
	 * (Template Method Pattern)
	 */
	void stop() noexcept
	{
		if(started_)
		{
			stop_();
			started_ = false;
		}
	}

	/** Restart the driver.
	 *
	 * Calls stop() and then start() on the driver. If the driver is not started,
	 * then it will be started after the start() function is invoked.
	 */
	void restart() noexcept
	{
		stop();
		start();
	}

	/** Check if the driver has been started.
	 *
	 * @returns `true` if the driver is running (started), `false` if not running (stopped).
	 */
	type_safe::boolean started() const noexcept
	{
		return started_;
	}

	/** Returns the registered type ID of the driver.
	 *
	 * When using DriverBase interfaces, clients can retrieve the registered driver type
	 * in order to up-cast to the correct interface.
	 *
	 * The type is returned as a embvm::DriverType_t rather than a embvm::DriverType enum to work
	 * with custom user-defined types. Enforcing a embvm::DriverType return value would prevent
	 * clients from defining and using their own custom types.
	 *
	 * @returns the registered driver type as a embvm::DriverType_t value
	 */
	constexpr embvm::DriverType_t DriverType() const noexcept
	{
		return type_;
	}

	/// Increment operator is a no-op, but is used for iterator compatibility.
	virtual DriverBase& operator++() noexcept
	{
		return *this;
	}

	/// Deleted copy constructor
	DriverBase(const DriverBase&) = delete;

	/// Deleted copy assignment operator
	const DriverBase& operator=(const DriverBase&) = delete;

	/// Deleted move constructor
	DriverBase(DriverBase&&) = delete;

	/// Deleted move assignment operator
	DriverBase& operator=(DriverBase&&) = delete;

  protected:
	/** Construct a DriverBase
	 *
	 * @param c The device type ID
	 */
	DriverBase(embvm::DriverType_t c = embvm::DriverType::Undefined) noexcept : type_(c) {}

	/// Destructor.
	~DriverBase() noexcept = default;

	/// Derived classes override the start_ method to control driver-specific startup behavior.
	/// (Template Method Pattern)
	virtual void start_() noexcept = 0;

	/// Derived classes override the start_ method to control driver-specific startup behavior.
	/// (Template Method Pattern)
	virtual void stop_() noexcept = 0;

	/// Tracks the driver state.
	/// True if the driver has been started, false if it has been stopped or not yet started.
	type_safe::boolean started_ = false;

	/// Type ID of the driver instance.
	const embvm::DriverType_t type_;
};

/// @}
// End group

} // namespace embvm

#endif // DRIVER_HPP_
