#include "gpio.hpp"

using namespace test;

bool UnitTestGPIOInput::get() noexcept
{
	switch(pull())
	{
		case embvm::gpio::pull::none:
			return false;
		case embvm::gpio::pull::pullup:
			return true;
		case embvm::gpio::pull::pulldown:
			return false;
	}

	return false;
}

embvm::gpio::pull UnitTestGPIOInput::pull_(embvm::gpio::pull p) noexcept
{
	return p;
}

void UnitTestGPIOInput::start_() noexcept {}

void UnitTestGPIOInput::stop_() noexcept {}

#pragma mark - GPIO With Pullup -

bool UnitTestGPIOInputWithPullup::get() noexcept
{
	switch(pull())
	{
		case embvm::gpio::pull::none:
			return false;
		case embvm::gpio::pull::pullup:
			return true;
		case embvm::gpio::pull::pulldown:
			return false;
	}

	return false;
}

embvm::gpio::pull UnitTestGPIOInputWithPullup::pull_(embvm::gpio::pull p) noexcept
{
	p_ = p;
	return p_;
}

void UnitTestGPIOInputWithPullup::start_() noexcept
{
	pull_(embvm::gpio::pull::pullup);
	CHECK(p_ == embvm::gpio::pull::pullup);
}

void UnitTestGPIOInputWithPullup::stop_() noexcept {}
