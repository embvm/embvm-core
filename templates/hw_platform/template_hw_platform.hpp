#ifndef TEMPLATE_HW_PLATFORM_HPP_
#define TEMPLATE_HW_PLATFORM_HPP_

#include <hw_platform/virtual_hw_platform.hpp>

/**
 * Checklist for creating a new hardware platform:
 *	- [ ] Rename the header guard so it's not TEMPLATE_HW_PLATFORM_HPP_
 *	- [ ] Rename the class to match the new HW platform's actual name
 *		- [ ] Update the template_hw_platform.cpp to match the new class name
 *		- [ ] Rename the files to reflect the new class name
 *	- [ ] Adjust the name of the "processor_" variable in the .cpp file
 */

/** New Hardware Platform
 *
 * Describe your hardware platform here. The first sentence should be a summary
 *
 * Talk about the hardware configuratoin of this platform
 *
 * What other usage notes might people need to be aware of? Be sure to talk about them!
 */
class TemplateHWPlatform : public embvm::VirtualHwPlatformBase<TemplateHWPlatform>
{
	using PlatformBase = embvm::VirtualHwPlatformBase<TemplateHWPlatform>;

  public:
	/// @brief Default constructor.
	TemplateHWPlatform() noexcept;

	/// @brief Default destructor.
	~TemplateHWPlatform() noexcept;

#pragma mark - Inherited Functions -

	static void earlyInitHook_() noexcept;
	void init_() noexcept;
	void initProcessor_() noexcept;
	void soft_reset_() noexcept;
	void hard_reset_() noexcept;

#pragma mark - Custom Functions -

	// Implement your custom hw platform functionality here

  private:
	// Declare drivers and other internal variables here
};

#endif // TEMPLATE_HW_PLATFORM_HPP_
