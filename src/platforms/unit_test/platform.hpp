// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#ifndef UNIT_TEST_PLATFORM_HPP_
#define UNIT_TEST_PLATFORM_HPP_

#include <platform_options.hpp>
#include <unittest_hw_platform.hpp>

/// This platform exists for use in the framework unit tests
class UnitTestPlatform : public embvm::VirtualPlatformBase<UnitTestPlatform, UnitTestHWPlatform>,
						 public embvm::PlatformDispatcher<PlatformDispatchQueue>,
						 public embvm::PlatformEventManagement<PlatformEventCenter>
{
	using VirtualPlatform = embvm::VirtualPlatformBase<UnitTestPlatform, UnitTestHWPlatform>;

  public:
	/// Default constructor
	UnitTestPlatform() noexcept {}

	/// Default destructor
	~UnitTestPlatform() noexcept {}

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
};

using VirtualPlatform = UnitTestPlatform;

#endif // UNIT_TEST_PLATFORM_HPP_
