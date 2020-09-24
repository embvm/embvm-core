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
