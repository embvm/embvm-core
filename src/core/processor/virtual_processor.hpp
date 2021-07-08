// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#ifndef VIRTUAL_PROCESSOR_HPP_
#define VIRTUAL_PROCESSOR_HPP_

#include <string>

namespace embvm
{
/// @addtogroup FrameworkProcessor
/// @{

/** Virtual Processor Base
 *
 * This class provides the common interfaces and behaviors that virtual processors
 * must implement. Some functionality is common to all platforms (name functions).
 *
 * Functions whose names are appended with `_` are meant to be supplied by the derived hardware
 *platform:
 *	- earlyInitHook_()
 *	- init_()
 *	- reset_()
 *
 * Derived classes may supply additional functions as required. The functions above are the
 * common required functions that all hardware platforms must supply.
 *
 * This class uses the @ref docs/development/patterns/crtp.md ["CRTP pattern"] rather
 * than virtual inheritance. To derive from this class, do the following:
 *	@code
 *	class nRF52840 : public VirtualProcessorBase<nRF52840>
 *	{ ... };
 *	@endcode
 *
 * @tparam TProcessor the derived processor implementation (CRTP pattern)
 */
template<typename TProcessor>
class VirtualProcessorBase
{
  public:
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
	static inline void earlyInitHook() noexcept
	{
		TProcessor::earlyInitHook_();
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
	inline void init() noexcept
	{
		static_cast<TProcessor*>(this)->init_();
	}

	/** Reset the chip
	 *
	 * The reset_() function must be implemented by the derived class.
	 *
	 * A soft reset is intended to reset the program to the beginning without
	 * performing a full power cycle.
	 */
	inline void reset() noexcept
	{
		static_cast<TProcessor*>(this)->reset_();
	}

	/// Deleted copy constructor
	VirtualProcessorBase(const VirtualProcessorBase&) = delete;

	/// Deleted copy assignment operator
	const VirtualProcessorBase& operator=(const VirtualProcessorBase&) = delete;

	/// Deleted move constructor
	VirtualProcessorBase(VirtualProcessorBase&&) = delete;

	/// Deleted move assignment operator
	VirtualProcessorBase& operator=(VirtualProcessorBase&&) = delete;

  protected:
	/// Default constructor
	VirtualProcessorBase() noexcept = default;

	/// Default destructor
	~VirtualProcessorBase() noexcept = default;
};

/// @}
// End group
} // namespace embvm

#endif // VIRTUAL_PROCESSOR_H__
