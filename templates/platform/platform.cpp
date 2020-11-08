#include "platform.hpp"

void TemplatePlatform::earlyInitHook_() noexcept
{
	// At a minimum, you probably want to call the early init hook for your hardware platform.
	TemplateHWPlatform::earlyInitHook()

	// Does your platform require any initialization functions to be called
	// prior to starting the OS and bringing up the system?
	// For example, initializing the heap:
	// malloc_addblock(&__HeapBase, reinterpret_cast<uintptr_t>(&__HeapLimit) -
	//								 reinterpret_cast<uintptr_t>(&__HeapBase));
}

void TemplatePlatform::initOS_() noexcept
{
	// Do you need to initialize the OS with any specific functions?
}

void TemplatePlatform::initHWPlatform_() noexcept
{
	// At a minimum, you probably want to call:
	hw_platform_.init();

	// You may also want to do any other platform-specific initialization of the hardware platform
	// Here's an example:
#if 0
	auto& tof0 = hw_platform_.tof0_inst();
	auto& screen0 = hw_platform_.screen0_inst();

	tof0.registerReadCallback([&](uint16_t v) {
		snprintf(tof_string_, 32, "ToF Range: %u mm\n", v);
		screen0.clear();
		screen0.printString(0, 0, tof_string_);
		screen0.printString(0, 32, tof_mode_string_);
		screen0.display();
	});
#endif
}

void TemplatePlatform::initProcessor_() noexcept
{
	// At a minimum, you probably want to call:
	hw_platform_.initProcessor();
}

void TemplatePlatform::init_() noexcept
{
	// What should be initialized before main() is called?
	// A common example is setting the logger clock:
#if 0
	auto clk = VirtualPlatform::template findDriver<embvm::clk::SystemClock>();
	if(clk)
	{
		PlatformLogger::inst().setClock(clk.value());
	}
	else
	{
		logdebug("No system clock instance found. Proceeding without timestamps.\n");
	}
#endif
}
