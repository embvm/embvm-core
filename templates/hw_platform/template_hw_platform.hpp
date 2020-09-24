#ifndef TEMPLATE_HW_PLATFORM_HPP_
#define TEMPLATE_HW_PLATFORM_HPP_

#include <hw_platform/virtual_hw_platform.hpp>

/**
 * TODO when creating a new hardware platform:
 *	- Rename the header guard so it's not TEMPLATE_HW_PLATFORM_HPP_
 *	- Rename the class to match the new HW platform's actual name
 *	- Update the PlatformBase() constructor with your platform's name
 *	- Update the template_hw_platform.cpp to match the new class name
 *	- Rename the files
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
	TemplateHWPlatform() noexcept : PlatformBase("Template HW Platform") {}

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
	embutil::InterruptQueue<std::mutex> irq_bottom_half_;
	embdrv::SimulatorSystemClock sysclock;
	embdrv::aardvarkAdapter aardvark{embdrv::aardvarkMode::GpioI2C};
	embdrv::aardvarkI2CMaster i2c0{aardvark};
	embdrv::aardvarkGPIOOutput<5> gpio5{aardvark};
	embdrv::aardvarkGPIOInput<4> gpio4{aardvark};
	embdrv::aardvarkGPIOInput<3> gpio3{aardvark};
	embvm::led::gpioActiveHigh led0{gpio5};
	embdrv::vl53l1x tof0{i2c0, SPARKFUN_VL53L1X_ADDR};
	embdrv::ssd1306 screen0{i2c0, SPARKFUN_SSD1306_ADDR};

	// Leave unconfigured during declaration, update in init
	embdrv::SimulatorTimer timer0;

	TimerManager_t tm_{timer0};
	TimerHandle_t led_sw_timer_handle;

	SimulatorProcessor processor;
};

#endif // TEMPLATE_HW_PLATFORM_HPP_
