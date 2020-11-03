// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#include "gpio.hpp"

using namespace test;

void UnitTestGPIO::start_() noexcept {}

void UnitTestGPIO::stop_() noexcept {}

#pragma mark - GPIO With Pullup -

bool UnitTestGPIO::get() noexcept
{
	return value_; // This is only really valid for testing
}

void UnitTestGPIO::set(bool v) noexcept
{
	value_ = v;
}

void UnitTestGPIO::toggle() noexcept
{
	value_ = !value_;
}

void UnitTestGPIO::setMode(embvm::gpio::mode mode) noexcept
{
	mode_ = mode;
}

embvm::gpio::mode UnitTestGPIO::mode() noexcept
{
	return mode_;
}
