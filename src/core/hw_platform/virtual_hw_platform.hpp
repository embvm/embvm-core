#ifndef VIRTUAL_HW_PLATFORM_H_
#define VIRTUAL_HW_PLATFORM_H_

#include <cassert>
#include <string>

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
 * @ingroup FrameworkHwPlatform
 */
template<typename THWPlatform>
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

  private:
	const std::string_view name_;
};

} // namespace embvm

#endif // VIRTUAL_HW_PLATFORM_H_
