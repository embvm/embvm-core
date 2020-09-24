#ifndef TEMPLATE_PLATFORM_HPP_
#define TEMPLATE_PLATFORM_HPP_

#include <boot/boot_sequencer.hpp>
#include <os.hpp>
#include <platform/virtual_platform.hpp>
#include <platform_logger.hpp>
#include <platform_options.hpp>

/** TODO for new platforms:
 *
 * - Include the header to the appropriate hardware platform
 * - Rename the class to reflect your platform
 * - Rename TEMPLATE_PLATFORM_HPP_ to something different
 * - Update the class name in platform.cpp
 * - Specify relevant options in platform_options.hpp
 * - Specify the logger in platform_logger.hpp
 * - Update the boot sequencer based on the platform's boot strategy
 */

class TemplatePlatform final
	: public embvm::VirtualPlatformBase<TemplatePlatform, PlatformDriverRegistry>
{
	using PlatformBase = embvm::VirtualPlatformBase<TemplatePlatform, PlatformDriverRegistry>;

  public:
#pragma mark - Base Platform Functions

	static void earlyInitHook_() noexcept;
	static void initOS_() noexcept;
	void init_() noexcept;
	void initProcessor_() noexcept;
	void initHWPlatform_() noexcept;

	/// Default constructor supplies a default name for the simulator platform
	TemplatePlatform() noexcept : PlatformBase("nRF52 FWDemo Platform") {}
	~TemplatePlatform() = default;

#pragma mark - Custom Platform Functions -

  public:
  private:
#pragma mark - Data Members -
  private:
};

using VirtualPlatform = TemplatePlatform;

// Substitute your platform's boot strategy here. Keep the platform called PlatformBootSequencer.
using PlatformBootSequencer = embvm::BootSequencer<embvm::DefaultBootStrategy<VirtualPlatform>>;

#endif // TEMPLATE_PLATFORM_HPP_
