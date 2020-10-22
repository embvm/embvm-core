#include "../../hw_platforms/unit_test/unittest_hw_platform.hpp"
#include "virtual_hw_platform.hpp"
#include <catch2/catch.hpp>

using namespace test;

TEST_CASE("Create Unit Test HW Platform")
{
	UnitTestHWPlatform p;

	CHECK(EXPECTED_UNIT_TEST_STARTING_DRIVERS == p.driverCount());
}

TEST_CASE("Find a driver by name using hw platform", "[core/platform/virtual_hardware_platform]")
{
	UnitTestHWPlatform p;
	;
	TestDriverBase TestDriver(embvm::DriverType::SPI);
	p.registerDriver("PlatformSPI", &TestDriver);

	auto driver = p.findDriver("PlatformSPI");

	CHECK((EXPECTED_UNIT_TEST_STARTING_DRIVERS + 1) == p.driverCount());
	CHECK(TestDriver == driver);

	p.unregisterDriver(&TestDriver);

	CHECK((EXPECTED_UNIT_TEST_STARTING_DRIVERS) == p.driverCount());
}

TEST_CASE("Find a driver by type using hw platform", "[core/platform/virtual_hardware_platform]")
{
	UnitTestHWPlatform p;
	;
	TestDriverBase TestDriver(embvm::DriverType::SPI);
	p.registerDriver("PlatformSPI", &TestDriver);

	auto driver = p.findDriver(embvm::DriverType::SPI);

	CHECK((EXPECTED_UNIT_TEST_STARTING_DRIVERS + 1) == p.driverCount());
	CHECK(TestDriver == driver);

	p.unregisterDriver("PlatformSPI");

	CHECK((EXPECTED_UNIT_TEST_STARTING_DRIVERS) == p.driverCount());
}

TEST_CASE("Find all drivers by type using hw platform", "[core/platform/virtual_hardware_platform]")
{
	UnitTestHWPlatform p;

	TestDriverBase TestDriver(embvm::DriverType::SPI);
	TestDriverBase TestDriver2(embvm::DriverType::SPI);
	TestDriverBase TestDriver3(embvm::DriverType::SPI);
	TestDriverBase TestDriver4{};

	p.registerDriver("spi0", &TestDriver);
	p.registerDriver("spi1", &TestDriver2);
	p.registerDriver("spi2", &TestDriver3);
	p.registerDriver("dummy0", &TestDriver4);

	auto driver_list = p.findAllDrivers(embvm::DriverType::SPI);

	CHECK((EXPECTED_UNIT_TEST_STARTING_DRIVERS + 4) == p.driverCount());
	CHECK(3 == driver_list.size());

	for(const auto& d : driver_list)
	{
		CHECK(embvm::DriverType::SPI == d->DriverType());
	}

	p.unregisterDriver("spi0");
	p.unregisterDriver("spi1");
	p.unregisterDriver(&TestDriver3);
	p.unregisterDriver(&TestDriver4);

	CHECK((EXPECTED_UNIT_TEST_STARTING_DRIVERS) == p.driverCount());
}
