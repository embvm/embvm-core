// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#include "volatile.hpp"
#include <catch2/catch.hpp>

using namespace embutil;

TEST_CASE("Load/store non-volatile", "[utilities/volatile]")
{
	uint8_t t = 0;
	auto x = volatile_load(&t);
	CHECK(x == t);
	x++;
	volatile_store(&t, x);
	CHECK(t == 1);
}

TEST_CASE("Load/store volatile", "[utilities/volatile]")
{
	volatile uint8_t t = 0;
	auto x = volatile_load(&t);
	CHECK(x == t);
	x++;
	volatile_store(&t, x);
	CHECK(t == 1);
}
