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
 * The DriverBase provides common interfaces which apply to all drivers. Use of the
 * DriverBase class also handles automatic registration/unregistration with the
 * platform's DriverRegistry. Registration/unregistration occurs during construction/destruction.
 *
 * Derived classes must override the following functions:
 * 	- start_()
 * 	- stop_()
 */
class DriverBase
{
  protected:
	/** Construct a DriverBase with a C-string name.
	 *
	 * The DriverBase constructor handles automatic registration with the DriverRegistry.
	 *
	 * @param name The name of the device driver instance
	 * @param c The device type
	 */
	DriverBase(const char* name, embvm::DriverType_t c = embvm::DriverType::Undefined) noexcept;

	/** Construct a DriverBase with a std::string name.
	 *
	 * The DriverBase constructor handles automatic registration with the DriverRegistry
	 *
	 * @param name The name of the device driver instance.
	 *	Note: DriverBase() uses a std::string_view, so the std::string must remain valid
	 * @param c The device type
	 */
	DriverBase(const std::string& name,
			   embvm::DriverType_t c = embvm::DriverType::Undefined) noexcept;

	/** Construct a DriverBase with a std::string_view name.
	 *
	 * The DriverBase constructor handles automatic registration with the DriverRegistry
	 *
	 * @param name The name of the device driver instance.
	 *	Note: DriverBase() uses a std::string_view, so the std::string_view must remain valid
	 * @param c The device type
	 */
	DriverBase(const std::string_view& name,
			   embvm::DriverType_t c = embvm::DriverType::Undefined) noexcept;

  public:
	/// Deleted copy constructor
	DriverBase(const DriverBase&) = delete;

	/// Deleted copy assignment operator
	const DriverBase& operator=(const DriverBase&) = delete;

	/// Deleted move constructor
	DriverBase(DriverBase&&) = delete;

	/// Deleted move assignment operator
	DriverBase& operator=(DriverBase&&) = delete;

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

	/** Provides a string_view reference of the driver name.
	 *
	 * @returns `std::string_view` ref containing the driver name.
	 */
	constexpr const std::string_view& name() const noexcept
	{
		return name_;
	}

	/** Provides a c-string version of the driver name
	 *
	 * @returns c-string containing the driver name
	 */
	constexpr const char* name_cstr() const noexcept
	{
		return name_.data();
	}

	/// Increment operator is a no-op, but is used for iterator compatibility.
	virtual DriverBase& operator++() noexcept
	{
		return *this;
	}

  protected:
	/// Destructor.
	/// When DriverBase is destructed, the driver is unregistered from the DriverRegistry.
	virtual ~DriverBase() noexcept;

	/// Derived classes override the start_ method to control driver-specific startup behavior.
	/// (Template Method Pattern)
	virtual void start_() noexcept = 0;

	/// Derived classes override the start_ method to control driver-specific startup behavior.
	/// (Template Method Pattern)
	virtual void stop_() noexcept = 0;

	/// Tracks the driver state.
	/// True if the driver has been started, false if it has been stopped or not yet started.
	type_safe::boolean started_ = false;

	/// Name of the driver instance.
	const std::string_view name_;

	/// Type ID of the driver instance.
	const embvm::DriverType_t type_;
};

/// @}
// End group

} // namespace embvm

#endif // DRIVER_HPP_
