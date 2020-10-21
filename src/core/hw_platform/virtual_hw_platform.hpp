#ifndef VIRTUAL_HW_PLATFORM_H_
#define VIRTUAL_HW_PLATFORM_H_

#include <driver/driver_registry.hpp>
#include <string>
#if __has_include(<hw_platform_options.hpp>)
#include <hw_platform_options.hpp>
#endif

namespace embvm
{
/** Virtual Hardware Platform Base
 *
 * This class provides the common interfaces and behaviors that virtual hardware platforms
 * must implement. Some functionality is common to all platforms (name functions).
 *
 * Functions whose names are appended with `_` are meant to be supplied by the derived hardware
 * platform:
 *	- earlyInitHook_()
 *	- init_()
 *	- initProcessor_()
 *	- soft_reset_()
 *	- hard_reset_()
 *	- shutdown_()
 *
 * Derived classes may supply additional functions as required. The functions above are the
 * common required functions that all hardware platforms must supply.
 *
 * This class uses the @ref docs/development/patterns/crtp.md ["CRTP pattern"] rather
 * than virtual inheritance. To derive from this class, do the following:
 *	@code
 *	class FWDemoSimulatorHWPlatform : public VirtualHwPlatformBase<FWDemoSimulatorHWPlatform>
 *	{ ... };
 *	@endcode
 *
 * @tparam THWPlatform the derived HW platform implementation (CRTP pattern)
 * @tparam TDriverRegistry The type of the platform's DriverRegistry. DriverRegistry type is
 * 	specified to enable consumers to specify the exact DriverRegistry type/strategy in the
 *	platform layer, since the HW platform doesn't need to know anything about the memory
 *	allocation scheme.
 *	If your platform defines a hw_platform_options.hpp header, then the base class will
 *	automatically pick up your configured registry type.
 *	@code
 *	class UnitTestHWPlatform
 *	: public embvm::VirtualHwPlatformBase<UnitTestHWPlatform>
 *	@endcode
 *  If this header is missing, or you wish to override the platform default for whatever
 *  reason, you can manually specify the type.
 *	@code
 *	class UnitTestHWPlatform
 *	: public embvm::VirtualHwPlatformBase<UnitTestHWPlatform, embvm::DynamicDriverRegistry>
 *	@endcode
 *
 * @ingroup FrameworkHwPlatform
 */
#if __has_include(<hw_platform_options.hpp>)
template<typename THWPlatform, class TDriverRegistry = PlatformDriverRegistry>
#else
template<typename THWPlatform, class TDriverRegistry>
#endif
class VirtualHwPlatformBase
{
  public:
	/** Create a named virtual hardware platform
	 *
	 * @param name C-string containing the hardware platform name
	 */
	explicit VirtualHwPlatformBase(const char* name) noexcept : name_(name) {}

	/** Create a named virtual hardware platform
	 *
	 * @param name std::string containing the hardware platform name
	 * 	Note: VirtualHwPlatformBase uses a std::string_view,
	 *	so the std::string must remain valid throughout the lifetime of VirtualHwPlatformBase.
	 */
	explicit VirtualHwPlatformBase(const std::string& name) noexcept : name_(name) {}

	/** Create a named virtual hardware platform
	 *
	 * @param name std::string_view containing the hardware platform name
	 * 	Note: VirtualHwPlatformBase uses a std::string_view,
	 *	so the std::string_view must remain valid throughout the lifetime of VirtualHwPlatformBase.
	 */
	explicit VirtualHwPlatformBase(const std::string_view& name) noexcept : name_(name) {}

	/// Default destructor
	~VirtualHwPlatformBase() = default;

	/// Deleted copy constructor
	VirtualHwPlatformBase(const VirtualHwPlatformBase&) = delete;

	/// Deleted copy assignment operator
	const VirtualHwPlatformBase& operator=(const VirtualHwPlatformBase&) = delete;

	/// Deleted move constructor
	VirtualHwPlatformBase(VirtualHwPlatformBase&&) = delete;

	/// Deleted move assignment operator
	VirtualHwPlatformBase& operator=(VirtualHwPlatformBase&&) = delete;

	/// Returns the Virtual HW Platform's name
	constexpr const std::string_view& name() const noexcept
	{
		return name_;
	}

	/// Returns the platform name as a cstring for C API compatibility
	constexpr const char* name_cstr() const noexcept
	{
		return name_.data();
	}

	/** Perform any special initialization steps
	 *
	 * The earlyInitHook_() function must be implemented by the derived class as
	 * a static function.
	 *
	 * These functions run before the C Run-time setup functions are called, and normal
	 * functionality (e.g. OS concepts) are not available.
	 *
	 * Early initialization steps represent any super early code that needs to execute.
	 * For example, DRAM might need to be initialized before the boot process can relocate
	 * memory to its proper location in memory.
	 *
	 * These functions are declared static to prevent you from easily using
	 * drivers within their confines
	 */
	static void earlyInitHook() noexcept
	{
		THWPlatform::earlyInitHook_();
	}

	/** Initialize the processor
	 *
	 * The initProcessor_() function must be implemented by the derived class.
	 *
	 * Perform any steps necessary for initializing the processor & processor
	 * peripheral devices for use.
	 *
	 * Note that this is only intended to be processor initialization. Platform
	 * 	initialization nis handled by the init() function.
	 *
	 * This function will likely invoke the VirtualProcessor::init() function,
	 * but the order and actual initialization process is left to the derived class.
	 */
	void initProcessor() noexcept
	{
		static_cast<THWPlatform*>(this)->initProcessor_();
	}

	/** Initialize the hardware platform
	 *
	 * The init_() function must be implemented by the derived class.
	 *
	 * Perform any hardware platform initialization steps, such as preparing
	 *	peripherals for use, setting up callbacks, etc.
	 */
	void init() noexcept
	{
		static_cast<THWPlatform*>(this)->init_();
	}

	/** Perform a soft reset of the system (chip reset)
	 *
	 * The soft_reset_() function must be implemnted by the derived class.
	 *
	 * A soft reset is intended to reset the program to the beginning without
	 * performing a full power cycle.
	 *
	 * A good strategy is to just call virtual processor's reset() function, if
	 * one is provided.
	 */
	void soft_reset() noexcept
	{
		static_cast<THWPlatform*>(this)->soft_reset_();
	}

	/** Perform a hard reset of the system (power cycle)
	 *
	 * The hard_reset_() function must be implemented by the derived class.
	 *
	 * A hard reset is intended to perform a full power cycle of the system.
	 *
	 * Platforms which do not support a hard reset should default to calling soft_reset().
	 */
	void hard_reset() noexcept
	{
		static_cast<THWPlatform*>(this)->hard_reset_();
	}

	/** Shutdown the system
	 *
	 * The shutdown_() function must be implemented by the derived class.
	 *
	 * If your platform supports shutdown capability, take the proper steps to initiate a shutdown.
	 *
	 * If the platform does not support shutdown capability, implement your shutdown_() function as
	 * an assert to notify users trying to call this API. You could also leave the shutdown_()
	 * function undefined. Anyone who calls the shutdown() function on a platform with an undefined
	 * shutdown_() function will encounter a compiler error.
	 *
	 * The shutdown function is not expected to return.
	 */
	void shutdown [[noreturn]] () noexcept
	{
		static_cast<THWPlatform*>(this)->shutdown_();
	}

#pragma mark - Driver Registry Functions -

	/** Hardware Platform API for registering a new device driver
	 *
	 * Register a device driver with the platform's driver registry.
	 *
	 * This call forwards the information to the DriverRegistry instance.
	 *
	 * @param name The name (used as a key) the driver will be registered under.
	 * @param driver Pointer to the embvm::DriverBase object. A pointer is used because
	 *	there are any number of potential derived classes which will be tracked.
	 *	To prevent slicing, a pointer to the base class is stored.
	 */
	inline void registerDriver(const std::string_view& name, embvm::DriverBase* driver) noexcept
	{
		driver_registry_.add(name.data(), driver);
	}

	/** Hardware Platform API for unregistering a new device driver
	 *
	 * Unregister a device driver with the platform's driver registry.
	 *
	 * This call forwards the information to the DriverRegistry instance.
	 *
	 * @param name The name of the driver to remove.
	 * @param driver Pointer to the embvm::DriverBase object being removed.
	 */
	inline void unregisterDriver(const std::string_view& name, embvm::DriverBase* driver) noexcept
	{
		driver_registry_.remove(name.data(), driver);
	}

	/** Hardware Platform API for unregistering a new device driver by key.
	 *
	 * Unregister a device driver with the platform's driver registry using the key.
	 *
	 * This call forwards the information to the DriverRegistry instance.
	 *
	 * @param name The name of the driver to remove.
	 */
	inline void unregisterDriver(const std::string_view& name) noexcept
	{
		driver_registry_.remove(name.data());
	}

	/** Hardware Platform API for unregistering a new device driver by value.
	 *
	 * Unregister a device driver with the platform's driver registry using the driver pointer.
	 *
	 * This call forwards the information to the DriverRegistry instance.
	 *
	 * @param driver Pointer to the embvm::DriverBase object being removed.
	 */
	inline void unregisterDriver(embvm::DriverBase* driver) noexcept
	{
		driver_registry_.remove(driver);
	}

	/** Access a device driver in the registry by name
	 *
	 * Find a driver by name.
	 *
	 * This call forwards the information to the DriverRegistry instance.
	 *
	 * @returns An optional_ref to the embvm::DriverBase instance. If no instance is found,
	 *	the optional reference will be invalid. The caller must cast to the appropriate type.
	 */
	inline auto findDriver(const std::string_view& name) noexcept
	{
		return driver_registry_.find(name.data());
	}

	/** Access a device driver in the registry by type
	 *
	 * Find a driver by type. If multiple drivers are found for a type, the first one found will be
	 *returned.
	 *
	 * This call forwards the information to the DriverRegistry instance.
	 *
	 * @returns An optional_ref to the embvm::DriverBase instance. If no instance is found,
	 *	the optional reference will be invalid. The caller must cast to the appropriate type.
	 */
	inline auto findDriver(embvm::DriverType_t type) noexcept
	{
		return driver_registry_.find(type);
	}

	/** Access a device driver in the registry by type, cast as the appropriate base class.
	 *
	 * If multiple drivers are found for a type, the first one found will be returned.
	 * The type will be returned as the appropriate base class (instead of embvm::DriverBase).
	 *
	 * This call forwards the information to the DriverRegistry instance.
	 *
	 * @tparam TDriverClass The class of driver being requested (embvm::i2c::master, SystemClock).
	 * @returns an type_safe::optional_ref cast to the TDriverClass type. If the driver
	 *	was not found, the optional_ref will be empty.
	 */
	template<class TDriverClass>
	inline auto findDriver() noexcept
	{
		return driver_registry_.template find<TDriverClass>();
	}

	/** Access a device driver in the registry by name, cast as the appropriate base class
	 *
	 * The type will be returned as the appropriate base class (instead of embvm::DriverBase).
	 *
	 * This call forwards the information to the DriverRegistry instance.
	 *
	 * @tparam TDriverClass The class of driver being requested (embvm::i2c::master, SystemClock).
	 * @returns an type_safe::optional_ref cast to the TDriverClass type. If the driver
	 *	was not found, the optional_ref will be empty.
	 */
	template<class TDriverClass>
	inline auto findDriver(const std::string_view& name) noexcept
	{
		return driver_registry_.template find<TDriverClass>(name.data());
	}

	/** Get a list of all device drivers in the registry by type.
	 *
	 * The type will be returned as the appropriate base class (instead of embvm::DriverBase).
	 *
	 * This call forwards the information to the DriverRegistry instance.
	 *
	 * @param type The type of driver being requested (embvm::i2c::master, SystemClock).
	 * @returns A list of embvm::DriverBase instances. If no matching types are found,
	 *	an empty list will be returned. The caller must cast to the appropriate type.
	 */
	inline auto findAllDrivers(embvm::DriverType_t type) noexcept
	{
		return driver_registry_.findAll(type);
	}

	/** Get a list of all device drivers in the registry by type, cast as the appropriate base
	 *class.
	 *
	 * The type will be returned as the appropriate base class (instead of embvm::DriverBase).
	 *
	 * This call forwards the information to the DriverRegistry instance.
	 *
	 * @tparam TDriverClass The class of driver being requested (embvm::i2c::master, SystemClock).
	 * @returns A list of driver instances cast as TDriverClass types. If no matching types are
	 *found, an empty list will be returned.
	 */
	template<class TDriverClass>
	inline auto findAllDrivers() noexcept
	{
		return driver_registry_.template findAll<TDriverClass>();
	}

	/** Get the count of drivers registered with the platform.
	 *
	 * returns Number of drivers currently registered with the platform DriverRegistry.
	 */
	inline size_t driverCount() const noexcept
	{
		return driver_registry_.count();
	}

  private:
	const std::string_view name_;
	TDriverRegistry driver_registry_{};
};

} // namespace embvm

#endif // VIRTUAL_HW_PLATFORM_H_
