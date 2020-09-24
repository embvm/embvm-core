#ifndef UNIT_TEST_PLATFORM_HPP_
#define UNIT_TEST_PLATFORM_HPP_

#include <platform_options.hpp>
#include <unittest_hw_platform.hpp>

/// This platform exists for use in the framework unit tests
class UnitTestPlatform
	: public embvm::VirtualPlatformBase<UnitTestPlatform, PlatformDriverRegistry>,
	  public embvm::PlatformDispatcher<PlatformDispatchQueue>,
	  public embvm::PlatformEventManagement<PlatformEventCenter>
{
	using VirtualPlatform = embvm::VirtualPlatformBase<UnitTestPlatform, PlatformDriverRegistry>;

  public:
	// Note: Unit test constructors are public for testing purposes

	/// Default constructor with a supplied platform name
	UnitTestPlatform() noexcept : VirtualPlatform("UnitTestPlatform") {}

	/// Specify unit test platform by const char* to test the platform base APIs
	explicit UnitTestPlatform(const char* name) noexcept : VirtualPlatform(name) {}

	/// Specify unit test platform by std::string to test the platform base APIs
	explicit UnitTestPlatform(const std::string& name) noexcept : VirtualPlatform(name) {}

	/// Specify unit test platform by std::string_view to test the platform base APIs
	explicit UnitTestPlatform(const std::string_view& name) noexcept : VirtualPlatform(name) {}

	~UnitTestPlatform() = default;

	static void earlyInitHook_() noexcept
	{
		UnitTestHWPlatform::earlyInitHook();
	}

	void init_() noexcept {}

	void initProcessor_() noexcept
	{
		hw_platform_.initProcessor();
	}

	void initHWPlatform_() noexcept
	{
		hw_platform_.init();
	}

	auto hwPlatformName() noexcept
	{
		return hw_platform_.name();
	}

	auto hwPlatformName_cstr() noexcept
	{
		return hw_platform_.name_cstr();
	}

  private:
	UnitTestHWPlatform hw_platform_;
};

using VirtualPlatform = UnitTestPlatform;

#endif // UNIT_TEST_PLATFORM_HPP_
