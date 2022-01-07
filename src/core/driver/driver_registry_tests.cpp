// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#include "driver_registry.hpp"
#include <atomic>
#include <catch2/catch.hpp>
#include <cstring>
#include <unit_test/driver.hpp> // Unit test driver for abstract class
#include <unit_test/i2c.hpp>

using namespace embvm;
using namespace test;

#pragma mark - Helper Functions -

TEST_CASE("Create dynamic driver registry", "[core/driver_registry]")
{
	DynamicDriverRegistry<> driver_registry;

	CHECK(0 == driver_registry.count());
	CHECK(0 < driver_registry.capacity());
}

TEST_CASE("Create static driver registry", "[core/driver_registry]")
{
	StaticDriverRegistry<32> driver_registry;

	CHECK(0 == driver_registry.count());
	CHECK(32 == driver_registry.capacity());
}

TEST_CASE("Add to and remove from static driver registry", "[core/driver_registry]")
{
	StaticDriverRegistry<32> driver_registry;
	TestDriverBase d{};

	driver_registry.add("Test base", &d);

	CHECK(1 == driver_registry.count());

	driver_registry.remove(&d);

	CHECK(0 == driver_registry.count());
}

#ifdef TEST_ASSERT_CASES
// Note that this test will assert
TEST_CASE("Add to full static driver registry", "[core/driver_registry]")
{
	StaticDriverRegistry<2> driver_registry;
	TestDriverBase d{};
	TestDriverBase d2{};
	TestDriverBase d3{};

	driver_registry.add("Test base", &d);
	driver_registry.add("Test base2", &d2);
	driver_registry.add("Test base3", &d3);

	CHECK(2 == driver_registry.count());
}
#endif

TEST_CASE("TestDriverBase is added to and removed from registery", "[core/driver_registry]")
{
	DynamicDriverRegistry<> driver_registry;
	TestDriverBase d{};

	driver_registry.add("Test base", &d);

	CHECK(1 == driver_registry.count());

	driver_registry.remove("Test base", &d);

	CHECK(0 == driver_registry.count());
}

TEST_CASE("Remove driver by key", "[core/driver_registry]")
{
	DynamicDriverRegistry<> driver_registry;
	TestDriverBase d{};

	driver_registry.add("Test base", &d);

	CHECK(1 == driver_registry.count());

	driver_registry.remove("Test base");

	CHECK(0 == driver_registry.count());
}

TEST_CASE("Remove driver by value", "[core/driver_registry]")
{
	DynamicDriverRegistry<> driver_registry;
	TestDriverBase d{};

	driver_registry.add("Test base", &d);

	CHECK(1 == driver_registry.count());

	driver_registry.remove(&d);

	CHECK(0 == driver_registry.count());
}

TEST_CASE("Find driver from registry by name", "[core/driver_registry]")
{
	DynamicDriverRegistry<> driver_registry;
	TestDriverBase d{};
	TestDriverBase d2{};

	driver_registry.add("Test base", &d);
	driver_registry.add("Test base2", &d2);

	CHECK(2 == driver_registry.count());
	CHECK(&d == driver_registry.find("Test base").value());
	CHECK(&d2 == driver_registry.find("Test base2").value());

	driver_registry.remove(&d);
	driver_registry.remove(&d2);

	CHECK(0 == driver_registry.count());
}

TEST_CASE("Find one driver from registry by type", "[core/driver_registry]")
{
	DynamicDriverRegistry<> driver_registry;
	TestDriverBase d(DriverType::SPI);

	driver_registry.add("Test base", &d);

	auto d_found = driver_registry.find(DriverType::SPI);

	CHECK(d_found);
	CHECK(&d == *d_found);
}

TEST_CASE("Find one driver from registry by template method", "[core/driver_registry]")
{
	DynamicDriverRegistry<> driver_registry;
	i2cTestDriver d;

	driver_registry.add("i2c0", &d);

	auto d_found = driver_registry.find<embvm::i2c::master>();
	auto d2_found = driver_registry.find(DriverType::I2C);

	CHECK(d_found);
	CHECK(d2_found);
	CHECK(*d_found == *d2_found);
}

TEST_CASE("Find one driver from registry by template method via name", "[core/driver_registry]")
{
	DynamicDriverRegistry<> driver_registry;
	i2cTestDriver d{};

	driver_registry.add("i2c0", &d);

	auto d_found = driver_registry.find<embvm::i2c::master>("i2c0");

	CHECK(d_found);
	CHECK(*d_found == &d);
}

TEST_CASE("Find all drivers from registry by given type", "[core/driver_registry]")
{
	DynamicDriverRegistry<> driver_registry;
	TestDriverBase d(DriverType::SPI);
	TestDriverBase d2(DriverType::SPI);

	driver_registry.add("Test base", &d);
	driver_registry.add("Test base2", &d2);

	auto found_list = driver_registry.findAll(DriverType::SPI);

	CHECK(2 <= found_list.size());

	for(const auto& t : found_list)
	{
		CHECK(t->DriverType() == DriverType::SPI);
		CHECK(((t == reinterpret_cast<embvm::DriverBase*>(&d)) ||
			   (t == reinterpret_cast<embvm::DriverBase*>(&d2))));
	}
}

TEST_CASE("Find all drivers from registry by template method", "[core/driver_registry]")
{
	DynamicDriverRegistry<> driver_registry;
	i2cTestDriver d{};
	i2cTestDriver d2{};

	driver_registry.add("Test", &d);
	driver_registry.add("Test2", &d2);

	auto found_list = driver_registry.findAll<embvm::i2c::master>();

	CHECK(2 <= found_list.size());

	for(const auto& t : found_list)
	{
		CHECK(t->DriverType() == DriverType::I2C);
		CHECK(((t == reinterpret_cast<embvm::DriverBase*>(&d)) ||
			   (t == reinterpret_cast<embvm::DriverBase*>(&d2))));
	}
}

TEST_CASE("Find Driver fails if no driver of that type is registered", "[core/driver_registry]")
{
	DynamicDriverRegistry<> driver_registry;

	auto d1_found = driver_registry.find(DriverType::SPI);

	CHECK(!d1_found);
}

TEST_CASE("Find all returns empty list with no matches", "[core/driver_registry]")
{
	DynamicDriverRegistry<> driver_registry;

	auto found_list = driver_registry.findAll(DriverType::SPI);

	CHECK(0 == found_list.size());
}
