#include "template_hw_platform.hpp"

TemplateHWPlatform::TemplateHWPlatform() noexcept {}

TemplateHWPlatform::~TemplateHWPlatform() noexcept {}

void TemplateHWPlatform::earlyInitHook_() noexcept
{
	// Should anything happen for this hardware platform before we initialize the
	// processor, the OS, and the board?
	// Such an example might include setting up DRAM (needed before libc++ calls constructors)
}

void TemplateHWPlatform::soft_reset_() noexcept
{
	// You will likely want to do this:
	processor_.reset();
}

void TemplateHWPlatform::hard_reset_() noexcept
{
	// Default to a soft reset if your system doesn't have hard reset capabilities
	soft_reset_();
}

void TemplateHWPlatform::initProcessor_() noexcept
{
	// You probably want to call this at a bare minimum:
	processor_.init();
}

void TemplateHWPlatform::init_() noexcept
{
	// What should happen during the boot process for your hardware platform?
}
