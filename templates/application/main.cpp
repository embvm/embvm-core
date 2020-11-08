#include <platform.hpp>

volatile bool abort_program_ = false;

int main()
{
	auto& platform = VirtualPlatform::inst();

	while(!abort_program_)
	{
		// spin
	}

	return 0;
}
