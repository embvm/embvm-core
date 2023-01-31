// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#include "boot_sequencer.hpp"
#include <catch2/catch_test_macros.hpp>
#include <platform.hpp>

/// Useful for things like Catch where we want to test things but we don't want
/// to invoke the main() function twice
template<class TPlatform>
void DoNotCallMainBootStrategy()
{
	TPlatform::earlyInitHook();

	// Now that the runtime has been initialized + constructors called, we can
	// create our platform instance and initialize it
	TPlatform& platform = TPlatform::inst();

	platform.initProcessor();

	platform.initHWPlatform();

	platform.init();
}

TEST_CASE("Create Boot Sequencer", "[core/boot]")
{
	embvm::BootSequencer<DoNotCallMainBootStrategy<UnitTestPlatform>>::boot();
}
