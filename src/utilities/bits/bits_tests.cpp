// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#include "bits.hpp"
#include <catch2/catch.hpp>

using namespace embutil;

TEST_CASE("alignment tests", "[utility/bits/align]")
{
	const uint8_t x = 8;
	STATIC_REQUIRE(true == is_aligned(x, 8));
	CHECK(true == is_aligned(&x, 1));

	uint32_t y;
	CHECK(true == is_aligned(&y, 4));

	y = 7;
	CHECK(false == is_aligned(y, 8));

	struct foo
	{
		// cppcheck-suppress unusedStructMember
		uint32_t a;
		// cppcheck-suppress unusedStructMember
		uint32_t b;
		// cppcheck-suppress unusedStructMember
		char c;
	};

	foo z;
	CHECK(true == is_aligned(z, alignof(foo)));
}

TEST_CASE("is power of 2", "[utility/bits]")
{
	STATIC_REQUIRE(true == is_power_2(0));
	STATIC_REQUIRE(true == is_power_2(1));
	STATIC_REQUIRE(true == is_power_2(2));
	STATIC_REQUIRE(true == is_power_2(4));
	STATIC_REQUIRE(false == is_power_2(3));
	STATIC_REQUIRE(false == is_power_2(1023));
}

TEST_CASE("countbits/set", "[utility/bits]")
{
	STATIC_REQUIRE(0 == countbits<0>::set);
	STATIC_REQUIRE(1 == countbits<1>::set);
	STATIC_REQUIRE(2 == countbits<3>::set);
	STATIC_REQUIRE(3 == countbits<7>::set);
	STATIC_REQUIRE(4 == countbits<15>::set);
}

TEST_CASE("countbits/significant_bits", "[utility/bits]")
{
	STATIC_REQUIRE(0 == countbits<0>::significant_bits);
	STATIC_REQUIRE(1 == countbits<1>::significant_bits);
	STATIC_REQUIRE(2 == countbits<2>::significant_bits);
	STATIC_REQUIRE(2 == countbits<3>::significant_bits);
	STATIC_REQUIRE(3 == countbits<7>::significant_bits);
	STATIC_REQUIRE(4 == countbits<8>::significant_bits);
	STATIC_REQUIRE(4 == countbits<15>::significant_bits);
}

TEST_CASE("Extract bitfield", "[utility/bits]")
{
	auto val = 0b1101'0011;
	// cppcheck-suppress constArgument
	CHECK(EXTRACT_BITFIELD(val, 7, 5) == 0b110);
}

TEST_CASE("Bitmask", "[utility/bits]")
{
	STATIC_REQUIRE(0b0110'0001 == bitmask<uint8_t>(6, 5, 0));
}
