// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#include <boot/boot_sequencer.hpp>
#include <platform.hpp>

extern "C" void entry()
{
	bootSystem();
}

// Unit test platform simply calls main
extern "C" void bootSystem()
{
	main();
}
