// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#ifndef BOOT_SEQUENCER_HPP_
#define BOOT_SEQUENCER_HPP_

#include <type_traits>

extern int main();

/// @addtogroup FrameworkCore
/// @{

/** @defgroup FrameworkBoot Framework Boot Sequencing
 *
 * Components to control the system/program boot process.
 *
 * @ingroup FrameworkCore
 * @{
 */

/// Prototype for our function entry point; can also be an asm symbol
extern "C" void entry();

/// Each platform declares this to control the boot sequence
/// @related embvm::BootSequencer
extern "C" void bootSystem();

/// We declare this function here because we don't want to include crt.h and
/// Add a dependency on our libc for the test code
extern "C" void CRTStartup();

template<class TPlatform>
void main_thread_func() noexcept
{
	// Now that the runtime has been initialized + constructors called, we can
	// create our platform instance and initialize it
	TPlatform& platform = TPlatform::inst();

	platform.initProcessor();

	platform.initHWPlatform();

	platform.init();

	main();
}

/// @}
/// @}
// Endgroup

namespace embvm
{
/// @addtogroup FrameworkCore
/// @{
/// @addtogroup FrameworkBoot
/// @{

/** Boot sequencer with customizable boot strategy
 *
 * The boot sequencer is a static class which handles booting logic supplied
 * by a strategy function. You do not need to *declare* an instance. Instead,
 * define a `PlatformBootSequencer` in your platform's platform.hpp file:
 *
 * @code
 * using PlatformBootSequencer = embvm::BootSequencer<embvm::DefaultBootStrategy<VirtualPlatform>>;
 * @endcode
 *
 * The framework will take care of the rest.
 *
 * @tparam TBootStrategy The function which describes the platform's boot
 * sequence. See DefaultBootStrategy() for an example.
 */
template<void TBootStrategy()>
class BootSequencer
{
  public:
	/// Deleted Default Constructor
	BootSequencer() = delete;

	/// Deleted Default Destructor
	~BootSequencer() = delete;

	/// Deleted copy constructor
	BootSequencer(const BootSequencer&) = delete;

	/// Deleted copy assignment operator
	const BootSequencer& operator=(const BootSequencer&) = delete;

	/// Deleted move constructor
	BootSequencer(BootSequencer&&) = delete;

	/// Deleted move assignment operator
	BootSequencer& operator=(BootSequencer&&) = delete;

	/// Execute the boot strategy to boot the system
	static inline void boot() noexcept
	{
		TBootStrategy();
	}
};

/** Default Framework Boot Strategy
 *
 * 1. call earlyInitHook();
 * 2. Setup the C/C++ runtime
 * 3. Setup the OS
 * 4. Setup the processor
 * 5. Setup the hardware platform
 * 6. Setup the platform
 * 7. Jump to main
 *
 * @related embvm::BootSequencer
 */
template<class TPlatform>
inline void DefaultBootStrategy() noexcept
{
	TPlatform::earlyInitHook();

	CRTStartup();

	TPlatform::initOS();

	// Now that the runtime has been initialized + constructors called, we can
	// create our platform instance and initialize it
	TPlatform& platform = TPlatform::inst();

	platform.initProcessor();

	platform.initHWPlatform();

	platform.init();

	main();
}

/** Continue Setup in Main Thread Framework Boot Strategy
 *
 * 1. call earlyInitHook();
 * 2. Setup the C/C++ runtime
 * 3. Setup the OS, which will terminate the current call chain and
 *	start a new thread. This new thread will continue execution:
 * 		1. Setup the processor
 * 		2. Setup the hardware platform
 * 		3. Setup the platform
 * 		4. Jump to main
 *
 * @related embvm::BootSequencer
 */
template<class TPlatform>
inline void BootOSWithMainThreadStrategy() noexcept
{
	TPlatform::earlyInitHook();

	CRTStartup();

	TPlatform::initOS(&main_thread_func<TPlatform>);

	__builtin_unreachable();
}

/// @}
/// @}
// End group

} // namespace embvm

/** OSX Simulator Boot Strategy
 *
 * Boots framework pieces, but lets OSX take care of CRT & OS initialization during
 * the dynamic loading process.
 *
 * 1. call earlyInitHook();
 * 2. Setup the processor
 * 3. Setup the hardware platform
 * 4. Setup the platform
 * 5. Jump to main
 *
 * @related embvm::BootSequencer
 * @ingroup FrameworkBoot
 */
template<class TPlatform>
inline void OSXSimulatorBootStrategy() noexcept
{
	TPlatform::earlyInitHook();

	// Now that the runtime has been initialized + constructors called, we can
	// create our platform instance and initialize it
	TPlatform& platform = TPlatform::inst();

	platform.initProcessor();

	platform.initHWPlatform();

	platform.init();

	main();
}

#endif // BOOT_SEQUENCER_HPP_
