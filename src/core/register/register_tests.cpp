// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#include "register.hpp"
#include <catch2/catch_test_macros.hpp>

using namespace embutil;

TEST_CASE("Create a register class", "[utility/register]")
{
	uint32_t addr = 100;
	sfr<> reg(addr);

	CHECK(100 == reg.address());
}

TEST_CASE("Read from register via dereference", "[utility/register]")
{
	uint32_t test_val = 0xDEADBEEF;

	sfr<> reg(&test_val);

	CHECK(reinterpret_cast<uintptr_t>(&test_val) == reg.address());
	CHECK(test_val == *reg);
}

TEST_CASE("Read from register via opT", "[utility/register]")
{
	uint32_t test_val = 0xDEADBEEF;
	sfr<> reg(&test_val);

	uint32_t x = reg;

	CHECK(x == test_val);
}

TEST_CASE("Read from register via load", "[utility/register]")
{
	uint32_t test_val = 0xDEADBEEF;
	sfr<> reg(&test_val);

	CHECK(test_val == reg.load());
}

TEST_CASE("Write to register via dereference", "[utility/register]")
{
	uint32_t test_val = 0xDEADBEEF;
	sfr<> reg(&test_val);

	*reg = 0xFEEDBEEF;

	CHECK(0xFEEDBEEF == test_val);
}

TEST_CASE("Write to register via copy", "[utility/register]")
{
	uint32_t test_val = 0xDEADBEEF;
	sfr<> reg(&test_val);

	reg = 0xFEEDBEEF;
	(void)reg; // silence cppcheck

	CHECK(0xFEEDBEEF == test_val);
}

TEST_CASE("Write to register via store", "[utility/register]")
{
	uint32_t test_val = 0xDEADBEEF;
	sfr<> reg(&test_val);

	reg.store(0xFEEDBEEF);

	CHECK(0xFEEDBEEF == test_val);
}

TEST_CASE("Create read-only register", "[utility/register]")
{
	uint32_t test_val = 0xDEADBEEF;
	sfr<ro> reg(&test_val);

	CHECK(0xDEADBEEF == reg);
}

TEST_CASE("Create write-only register", "[utility/register]")
{
	uint32_t test_val = 0xDEADBEEF;
	sfr<wo> reg(&test_val, 0xFEEDBEEF);

	CHECK(0xFEEDBEEF == test_val);
}

TEST_CASE("Compile-time register specialization", "[utility/register]")
{
	sfr<rw, uint32_t, 0x80000000> reg;

	CHECK(0x80000000 == reg.address());
}
