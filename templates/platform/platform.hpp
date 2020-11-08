#ifndef TEMPLATE_PLATFORM_HPP_
#define TEMPLATE_PLATFORM_HPP_

#include <boot/boot_sequencer.hpp>
#include <os.hpp>
#include <platform/virtual_platform.hpp>
#include <platform_logger.hpp>
#include <platform_options.hpp>
#include <template_hw_platform.hpp>

/** Checklist for new platforms:
 *
 * - [ ] Replace the template_hw_platform.hpp header with the header for
 *		 the appropriate hardware platform
 * - [ ] Rename TEMPLATE_PLATFORM_HPP_ to something different
 * - [ ] Rename the class to reflect your platform's name
 * 		- [ ] Update the VirtualPlatform alias to refer to your new class name
 * 		- [ ] Update the class name in platform.cpp
 * - [ ] Specify relevant hardware platform options in hw_platform_options.hpp
 * - [ ] Specify relevant OS options in platform_os_options.hpp
 * - [ ] Specify the logger in platform_logger.hpp
 * - [ ] Update the PlatformBootSequencer alias based on the platform's boot strategy
 */

class TemplatePlatform final
	: public embvm::VirtualPlatformBase<TemplatePlatform, TemplateHWPlatform>
{
	using PlatformBase = embvm::VirtualPlatformBase<TemplatePlatform, TemplateHWPlatform>;

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
