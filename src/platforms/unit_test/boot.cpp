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
