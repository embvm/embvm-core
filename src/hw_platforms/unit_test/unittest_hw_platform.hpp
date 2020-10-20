#ifndef UNIT_TEST_HW_PLATFORM_HPP_
#define UNIT_TEST_HW_PLATFORM_HPP_

#include <hw_platform/virtual_hw_platform.hpp>
#include <unit_test/driver.hpp>
#include <unit_test/gpio.hpp> // Unit test driver for abstract class
#include <unit_test/i2c.hpp> // Unit test driver for abstract class
#include <unit_test/spi.hpp> // Unit test driver for abstract class

#define EXPECTED_UNIT_TEST_STARTING_DRIVERS 1

/** Unit Test (Simulated) hardware platform
 *
 * This hardware platform exists to exercise the framework during the unit testing
 * process. This class should not be used directly for any other purpose.
 */
class UnitTestHWPlatform
	: public embvm::VirtualHwPlatformBase<UnitTestHWPlatform>
{
	using Base = embvm::VirtualHwPlatformBase<UnitTestHWPlatform>;

  public:
	/// @brief Default Constructor.
	UnitTestHWPlatform() noexcept : Base("Unit Test HW Platform")
	{
		registerDriver("test0", &driver_);
	}

	/// @brief Default Destructor.
	~UnitTestHWPlatform() = default;

#pragma mark - Inherited Functions -

	static void earlyInitHook_() noexcept {}

	void init_() noexcept {}

	void initProcessor_() noexcept {}

	void soft_reset_() noexcept {}

	void hard_reset_() noexcept
	{
		soft_reset_();
	}

	void shutdown_ [[noreturn]] () noexcept
	{
		assert(0 && "Shutdown not implemented for this platform\n");
	}

  private:
	/// We declare a single test driver to exercise the framework
	test::TestDriverBase driver_ = test::TestDriverBase("Undefined test driver");
};

#endif // UNIT_TEST_HW_PLATFORM_HPP_
