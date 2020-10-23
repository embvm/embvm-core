#ifndef VIRTUAL_PLATFORM_HPP_
#define VIRTUAL_PLATFORM_HPP_

#include <driver/driver.hpp>
#include <string>

namespace embvm
{
/** Base class for Platform definitions
 *
 * Platforms should derive from this base class, and optionally from
 * other platform base classes which provide optional functionality
 * (see embvm::PlatformEventManagement<>, embvm::PlatformDispatcher<>).
 *
 * @tparam TPlatform The derived CRTP class which defines the consumer's platform.
 * @tparam THWPlatform The type of the hardware platform class to use with this platform.
 *	A private local variable hw_platform_ is declared, and a protected function hw_platform()
 *	can be used as well to access the hardware platform APIs.
 * @ingroup FrameworkPlatform
 */
template<typename TPlatform, typename THWPlatform>
class VirtualPlatformBase
{
  public:
	/** Get the global platform instance.
	 *
	 * Static declaration of the VirtualPlatformBase instance.
	 * We use this function to avoid the static initializatio order 'fiasco'.
	 * See more: https://isocpp.org/wiki/faq/ctors#static-init-order
	 */
	static TPlatform& inst() noexcept
	{
		static TPlatform inst_;
		return inst_;
	}

	/// Deleted copy constructor
	VirtualPlatformBase(const VirtualPlatformBase&) = delete;

	/// Deleted copy assignment operator
	const VirtualPlatformBase& operator=(const VirtualPlatformBase&) = delete;

	/// Deleted move constructor
	VirtualPlatformBase(VirtualPlatformBase&&) = delete;

	/// Deleted move assignment operator
	VirtualPlatformBase& operator=(VirtualPlatformBase&&) = delete;

	/** Call early-initialization code.
	 *
	 * Call early initialization code, which is to be run before C runtime initialization,
	 * memory relocations, OS initialization, etc. This call is used to handle early setup
	 * code that the rest of the initialization process might depend on, such as DRAM
	 * initialization.
	 *
	 * This function forwards the call to the derived class for the actual
	 * implementation.
	 */
	static inline void earlyInitHook() noexcept
	{
		TPlatform::earlyInitHook_();
	}

	/** Initialize the OS.
	 *
	 * Performs steps necessary to initialize the OS for use by the platform.
	 * Control of this function is placed in the platform layer to allow
	 * for platform-specific customization & initialization of the OS,
	 * since each OS has different startup requirements
	 *
	 * This function forwards the call to the derived class for the actual
	 * implementation.
	 */
	static inline void initOS() noexcept
	{
		TPlatform::initOS_();
	}

	/** Initialize the with a function to use as the main thread
	 *
	 * Performs steps necessary to initialize the OS for use by the platform.
	 * Control of this function is placed in the platform layer to allow
	 * for platform-specific customization & initialization of the OS,
	 * since each OS has different startup requirements
	 *
	 * This function forwards the call to the derived class for the actual
	 * implementation.
	 *
	 * @param main_thread a function pointer to use as the main thread routine.
	 *	This version of initOS should be used for OSes such as FreeRTOS which
	 *	kill execution of the current thread when the scheduler is started.
	 */
	static inline void initOS(void (*main_thread)()) noexcept
	{
		TPlatform::initOS_(main_thread);
	}

	/** Initialize the platform.
	 *
	 * This function is responsible for initializing the platform.
	 *
	 * This function forwards the call to the derived class for the actual
	 * implementation.
	 */
	inline void init() noexcept
	{
		static_cast<TPlatform*>(this)->init_();
	}

	/** Initialize the processor.
	 *
	 * Performs steps necessary to initialize the processor for use by the platform.
	 * In many cases, this call should just invoke the processor's init() function.
	 * Control of this function is placed in the platform layer to allow
	 * for platform-specific customization & initialization of the processor in
	 * situations where non-default values are used.
	 *
	 * This function forwards the call to the derived class for the actual
	 * implementation.
	 */
	inline void initProcessor() noexcept
	{
		static_cast<TPlatform*>(this)->initProcessor_();
	}

	/** Initialize the hardware platform.
	 *
	 * Performs steps necessary to initialize the hardware platform for use by the platform.
	 * In many cases, this call should just invoke the hardware platform's init() function.
	 * Control of this function is placed in the platform layer to allow
	 * for platform-specific customization & initialization of the hardware platform in
	 * situations where non-default values are used.
	 *
	 * This function forwards the call to the derived class for the actual
	 * implementation.
	 */
	inline void initHWPlatform() noexcept
	{
		static_cast<TPlatform*>(this)->initHWPlatform_();
	}

#pragma mark - Driver Registry Functions -

	/** Platform-level API for registering a new device driver
	 *
	 * Register a device driver with the platform's driver registry.
	 *
	 * This call forwards the information to the Hardware Platform.
	 *
	 * @param name The name (used as a key) the driver will be registered under.
	 * @param driver Pointer to the embvm::DriverBase object. A pointer is used because
	 *	there are any number of potential derived classes which will be tracked.
	 *	To prevent slicing, a pointer to the base class is stored.
	 */
	inline void registerDriver(const std::string_view& name,
							   embvm::DriverBase* const driver) noexcept
	{
		hw_platform_.registerDriver(name.data(), driver);
	}

	/** Platform-level API for unregistering a new device driver
	 *
	 * Unregister a device driver with the platform's driver registry.
	 *
	 * This call forwards the information to the Hardware Platform.
	 *
	 * @param name The name of the driver to remove.
	 * @param driver Pointer to the embvm::DriverBase object being removed.
	 */
	inline void unregisterDriver(const std::string_view& name,
								 embvm::DriverBase* const driver) noexcept
	{
		hw_platform_.unregisterDriver(name.data(), driver);
	}

	/** Platform-level API for unregistering a new device driver by key.
	 *
	 * Unregister a device driver with the platform's driver registry using the name (key).
	 *
	 * This call forwards the information to the Hardware Platform.
	 *
	 * @param name The name of the driver to remove.
	 */
	inline void unregisterDriver(const std::string_view& name) noexcept
	{
		hw_platform_.unregisterDriver(name.data());
	}

	/** Platform-level API for unregistering a new device driver by value.
	 *
	 * Unregister a device driver with the platform's driver registry using the driver pointer.
	 *
	 * This call forwards the information to the Hardware Platform.
	 *
	 * @param driver Pointer to the embvm::DriverBase object being removed.
	 */
	inline void unregisterDriver(embvm::DriverBase* const driver) noexcept
	{
		hw_platform_.unregisterDriver(driver);
	}

	/** Access a device driver in the registry by name
	 *
	 * Find a driver by name.
	 *
	 * This call forwards the information to the Hardware Platform.
	 *
	 * @returns An optional_ref to the embvm::DriverBase instance. If no instance is found,
	 *	the optional reference will be invalid. The caller must cast to the appropriate type.
	 */
	inline auto findDriver(const std::string_view& name) noexcept
	{
		return hw_platform_.findDriver(name.data());
	}

	/** Access a device driver in the registry by type
	 *
	 * Find a driver by type. If multiple drivers are found for a type, the first one found will be
	 *returned.
	 *
	 * This call forwards the information to the Hardware Platform.
	 *
	 * @returns An optional_ref to the embvm::DriverBase instance. If no instance is found,
	 *	the optional reference will be invalid. The caller must cast to the appropriate type.
	 */
	inline auto findDriver(const embvm::DriverType_t type) noexcept
	{
		return hw_platform_.findDriver(type);
	}

	/** Access a device driver in the registry by type, cast as the appropriate base class.
	 *
	 * If multiple drivers are found for a type, the first one found will be returned.
	 * The type will be returned as the appropriate base class (instead of embvm::DriverBase).
	 *
	 * This call forwards the information to the Hardware Platform.
	 *
	 * @tparam TDriverClass The class of driver being requested (embvm::i2c::master, SystemClock).
	 * @returns an type_safe::optional_ref cast to the TDriverClass type. If the driver
	 *	was not found, the optional_ref will be empty.
	 */
	template<class TDriverClass>
	inline auto findDriver() noexcept
	{
		return hw_platform_.template findDriver<TDriverClass>();
	}

	/** Access a device driver in the registry by name, cast as the appropriate base class
	 *
	 * The type will be returned as the appropriate base class (instead of embvm::DriverBase).
	 *
	 * This call forwards the information to the Hardware Platform.
	 *
	 * @tparam TDriverClass The class of driver being requested (embvm::i2c::master, SystemClock).
	 * @returns an type_safe::optional_ref cast to the TDriverClass type. If the driver
	 *	was not found, the optional_ref will be empty.
	 */
	template<class TDriverClass>
	inline auto findDriver(const std::string_view& name) noexcept
	{
		return hw_platform_.template findDriver<TDriverClass>(name.data());
	}

	/** Get a list of all device drivers in the registry by type.
	 *
	 * The type will be returned as the appropriate base class (instead of embvm::DriverBase).
	 *
	 * This call forwards the information to the Hardware Platform.
	 *
	 * @param type The type of driver being requested (embvm::i2c::master, SystemClock).
	 * @returns A list of embvm::DriverBase instances. If no matching types are found,
	 *	an empty list will be returned. The caller must cast to the appropriate type.
	 */
	inline auto findAllDrivers(const embvm::DriverType_t type) noexcept
	{
		return hw_platform_.findAllDrivers(type);
	}

	/** Get a list of all device drivers in the registry by type, cast as the appropriate base
	 *class.
	 *
	 * The type will be returned as the appropriate base class (instead of embvm::DriverBase).
	 *
	 * This call forwards the information to the Hardware Platform.
	 *
	 * @tparam TDriverClass The class of driver being requested (embvm::i2c::master, SystemClock).
	 * @returns A list of driver instances cast as TDriverClass types. If no matching types are
	 *found, an empty list will be returned.
	 */
	template<class TDriverClass>
	inline auto findAllDrivers() noexcept
	{
		return hw_platform_.template findAllDrivers<TDriverClass>();
	}

	/** Get the count of drivers registered with the platform.
	 *
	 * returns Number of drivers currently registered with the platform DriverRegistry.
	 */
	inline size_t driverCount() const noexcept
	{
		return hw_platform_.driverCount();
	}

  protected:
	/// Get the hardware platform instance by function as an alternative of using the variable.
	THWPlatform& hw_platform()
	{
		return hw_platform_;
	}

	// Default constructor
	VirtualPlatformBase() noexcept {}

	/// Default destructor
	~VirtualPlatformBase() noexcept {}

#pragma mark - Data Members -

  protected:
	THWPlatform hw_platform_{};
};

} // namespace embvm

#endif // VIRTUAL_PLATFORM_HPP_
