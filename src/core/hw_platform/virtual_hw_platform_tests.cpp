#include "../../hw_platforms/unit_test/unittest_hw_platform.hpp"
#include "virtual_hw_platform.hpp"
#include <catch2/catch.hpp>

using namespace test;

TEST_CASE("Create Unit Test HW Platform with default name")
{
	UnitTestHWPlatform p;

	CHECK(0 == strcmp(p.name_cstr(), "Unit Test HW Platform"));
	CHECK(EXPECTED_UNIT_TEST_STARTING_DRIVERS == p.driverCount());
}

TEST_CASE("Find a driver by name using hw platform", "[core/platform/virtual_hardware_platform]")
{
	UnitTestHWPlatform p;
	;
	TestDriverBase TestDriver("PlatformSPI", embvm::DriverType::SPI);
	p.registerDriver(TestDriver.name(), &TestDriver);

	auto driver = p.findDriver("PlatformSPI");

	CHECK((EXPECTED_UNIT_TEST_STARTING_DRIVERS + 1) == p.driverCount());
	CHECK(TestDriver == driver);

	p.unregisterDriver(TestDriver.name(), &TestDriver);

	CHECK((EXPECTED_UNIT_TEST_STARTING_DRIVERS) == p.driverCount());
}

TEST_CASE("Find a driver by type using hw platform", "[core/platform/virtual_hardware_platform]")
{
	UnitTestHWPlatform p;
	;
	TestDriverBase TestDriver("PlatformSPI", embvm::DriverType::SPI);
	p.registerDriver(TestDriver.name(), &TestDriver);

	auto driver = p.findDriver(embvm::DriverType::SPI);

	CHECK((EXPECTED_UNIT_TEST_STARTING_DRIVERS + 1) == p.driverCount());
	CHECK(TestDriver == driver);

	p.unregisterDriver(TestDriver.name(), &TestDriver);

	CHECK((EXPECTED_UNIT_TEST_STARTING_DRIVERS) == p.driverCount());
}

TEST_CASE("Find all drivers by type using hw platform", "[core/platform/virtual_hardware_platform]")
{
	UnitTestHWPlatform p;
	;
	TestDriverBase TestDriver("spi0", embvm::DriverType::SPI);
	TestDriverBase TestDriver2("spi1", embvm::DriverType::SPI);
	TestDriverBase TestDriver3("spi2", embvm::DriverType::SPI);
	TestDriverBase TestDriver4("dummy0");

	p.registerDriver(TestDriver.name(), &TestDriver);
	p.registerDriver(TestDriver2.name(), &TestDriver2);
	p.registerDriver(TestDriver3.name(), &TestDriver3);
	p.registerDriver(TestDriver4.name(), &TestDriver4);

	auto driver_list = p.findAllDrivers(embvm::DriverType::SPI);

	CHECK((EXPECTED_UNIT_TEST_STARTING_DRIVERS + 4) == p.driverCount());
	CHECK(3 == driver_list.size());

	for(const auto& d : driver_list)
	{
		CHECK(embvm::DriverType::SPI == d->DriverType());
	}

	p.unregisterDriver(TestDriver.name(), &TestDriver);
	p.unregisterDriver(TestDriver2.name());
	p.unregisterDriver(&TestDriver3);
	p.unregisterDriver(&TestDriver4);

	CHECK((EXPECTED_UNIT_TEST_STARTING_DRIVERS) == p.driverCount());
}
