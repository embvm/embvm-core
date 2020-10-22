#include <catch2/catch.hpp>
#include <driver/driver_registry.hpp>
#include <platform.hpp>
#include <unit_test/driver.hpp> // unit test driver for abstract base class

using namespace test;

#pragma mark - Helpers -

static std::atomic<bool> flag = false;

static void test_flag(void)
{
	flag = true;
}

static void subscribeTestCb(embvm::EventBase e)
{
	(void)e;
	flag = true;
}

#pragma mark - Test Cases -

TEST_CASE("Create virtual platform base", "[core/platform/virtual_platform]")
{
	UnitTestPlatform p;

	CHECK(EXPECTED_UNIT_TEST_STARTING_DRIVERS == p.driverCount());
}

TEST_CASE("Find a driver by name", "[core/platform/virtual_platform]")
{
	auto& p = UnitTestPlatform::inst();
	TestDriverBase TestDriver(embvm::DriverType::SPI);
	p.registerDriver("PlatformSPI", &TestDriver);

	auto driver = p.findDriver("PlatformSPI");

	CHECK((EXPECTED_UNIT_TEST_STARTING_DRIVERS + 1) == p.driverCount());
	CHECK(TestDriver == driver);

	p.unregisterDriver(&TestDriver);

	CHECK((EXPECTED_UNIT_TEST_STARTING_DRIVERS) == p.driverCount());
}

TEST_CASE("Find a driver by type", "[core/platform/virtual_platform]")
{
	auto& p = UnitTestPlatform::inst();
	TestDriverBase TestDriver(embvm::DriverType::SPI);
	p.registerDriver("PlatformSPI", &TestDriver);

	auto driver = p.findDriver(embvm::DriverType::SPI);

	CHECK((EXPECTED_UNIT_TEST_STARTING_DRIVERS + 1) == p.driverCount());
	CHECK(TestDriver == driver);

	p.unregisterDriver(&TestDriver);

	CHECK((EXPECTED_UNIT_TEST_STARTING_DRIVERS) == p.driverCount());
}

TEST_CASE("Find all drivers by type", "[core/platform/virtual_platform]")
{
	auto& p = UnitTestPlatform::inst();
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

TEST_CASE("Dispatch from platform interface", "[core/platform/virtual_platform]")
{
	auto& p = UnitTestPlatform::inst();
	flag = false;

	p.dispatch(test_flag);

	std::this_thread::sleep_for(std::chrono::milliseconds(1));

	CHECK(flag == true);
}

TEST_CASE("Subscribe/unsubscribe to event from platform interface",
		  "[core/platform/virtual_platform]")
{
	auto& p = UnitTestPlatform::inst();
	flag = false;

	auto handle = p.subscribeToEvent(embvm::Signal::Event_Invalid, subscribeTestCb);

	CHECK(handle);

	p.unsubscribeFromEvent(handle);

	CHECK(false == handle);
}

TEST_CASE("Signal from platform interface", "[core/platform/unit_test_platform]")
{
	auto& p = UnitTestPlatform::inst();
	flag = false;

	// cppcheck-suppress unreadVariable
	auto handle = p.subscribeToEvent(embvm::Signal::Event_Invalid, subscribeTestCb);

	p.signal(embvm::Signal::Event_Invalid);

	std::this_thread::sleep_for(std::chrono::milliseconds(1));

	CHECK(flag == true);
}

TEST_CASE("Publish event from platform interface", "[core/platform/unit_test_platform]")
{
	auto& p = UnitTestPlatform::inst();
	flag = false;

	// cppcheck-suppress unreadVariable
	auto handle = p.subscribeToEvent(embvm::Signal::Event_Invalid, subscribeTestCb);

	auto e = embvm::EventBase(embvm::Signal::Event_Invalid);

	p.publishEvent(e);

	std::this_thread::sleep_for(std::chrono::milliseconds(1));

	CHECK(flag == true);
}

TEST_CASE("Hardware platform drivers are automatically registered",
		  "[core/platform/unit_test_platform]")
{
	auto& p = UnitTestPlatform::inst();

	CHECK(EXPECTED_UNIT_TEST_STARTING_DRIVERS == p.driverCount());
}
