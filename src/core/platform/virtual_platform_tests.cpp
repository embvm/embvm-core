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

TEST_CASE("Create virtual platform base with const char", "[core/platform/virtual_platform]")
{
	UnitTestPlatform p("Test Platform");

	CHECK(0 == strcmp(p.name_cstr(), "Test Platform"));
	CHECK(EXPECTED_UNIT_TEST_STARTING_DRIVERS == p.driverCount());
}

TEST_CASE("Create virtual platform base with std::string", "[core/platform/virtual_platform]")
{
	std::string name("TestPlatform");
	UnitTestPlatform p(name);

	CHECK(0 == name.compare(p.name()));
	CHECK(EXPECTED_UNIT_TEST_STARTING_DRIVERS == p.driverCount());
}

TEST_CASE("Create virtual platform base with std::string_view", "[core/platform/virtual_platform]")
{
	std::string_view name("TestPlatform");
	UnitTestPlatform p(name);

	CHECK(0 == name.compare(p.name()));
	CHECK(EXPECTED_UNIT_TEST_STARTING_DRIVERS == p.driverCount());
}

TEST_CASE("Find a driver by name", "[core/platform/virtual_platform]")
{
	auto& p = UnitTestPlatform::inst();
	TestDriverBase TestDriver("PlatformSPI", embvm::DriverType::SPI);
	p.registerDriver(TestDriver.name(), &TestDriver);

	auto driver = p.findDriver("PlatformSPI");

	CHECK((EXPECTED_UNIT_TEST_STARTING_DRIVERS + 1) == p.driverCount());
	CHECK(TestDriver == driver);

	p.unregisterDriver(TestDriver.name(), &TestDriver);

	CHECK((EXPECTED_UNIT_TEST_STARTING_DRIVERS) == p.driverCount());
}

TEST_CASE("Find a driver by type", "[core/platform/virtual_platform]")
{
	auto& p = UnitTestPlatform::inst();
	TestDriverBase TestDriver("PlatformSPI", embvm::DriverType::SPI);
	p.registerDriver(TestDriver.name(), &TestDriver);

	auto driver = p.findDriver(embvm::DriverType::SPI);

	CHECK((EXPECTED_UNIT_TEST_STARTING_DRIVERS + 1) == p.driverCount());
	CHECK(TestDriver == driver);

	p.unregisterDriver(TestDriver.name(), &TestDriver);

	CHECK((EXPECTED_UNIT_TEST_STARTING_DRIVERS) == p.driverCount());
}

TEST_CASE("Find all drivers by type", "[core/platform/virtual_platform]")
{
	auto& p = UnitTestPlatform::inst();
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
	p.unregisterDriver(&TestDriver2);
	p.unregisterDriver(TestDriver3.name());
	p.unregisterDriver(TestDriver4.name(), &TestDriver4);

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

TEST_CASE("Hardware platform is bundled in our Virtual Platform",
		  "[core/platform/unit_test_platform]")
{
	auto& p = UnitTestPlatform::inst();

	CHECK(0 == strcmp(p.hwPlatformName_cstr(), "Unit Test HW Platform"));
}

TEST_CASE("Hardware platform drivers are automatically registered",
		  "[core/platform/unit_test_platform]")
{
	auto& p = UnitTestPlatform::inst();

	CHECK(0 < p.driverCount());
}
