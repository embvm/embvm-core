// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#include <boot/boot_sequencer.hpp>
#include <platform.hpp> // For boot sequencer type

extern "C" __attribute__((weak)) void entry()
{
	bootSystem();
}

extern "C" void bootSystem()
{
	PlatformBootSequencer::boot();
}
