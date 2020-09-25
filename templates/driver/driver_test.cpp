#include "driver_registry.hpp"
#include <atomic>
#include <cstring>
#include <driver/active_i2c.hpp>
#include <driver/active_spi.hpp>
#include <platform.hpp>
#include <catch2/catch.hpp>
#include <unit_test/driver.hpp> // Unit test driver for abstract class

using namespace embvm;
using namespace test;

#pragma mark - Helper Functions -

static std::atomic<bool> callback_called = false;
static std::atomic<uint8_t> call_count = 0;

static void spi_callback(embvm::spi::op_t input, embvm::comm::status status)
{
	(void)input;
	(void)status;
	callback_called = true;
}

#pragma mark - Test Cases -

TEST_CASE("Create driver base class with const char", "[core/driver]")
{
	TestDriverBase d("Test Base");

	CHECK(false == d.started());
	CHECK(0 == strcmp("Test Base", d.name_cstr()));
	CHECK(DriverType::Undefined == d.DriverType());
}

TEST_CASE("Create driver base class with std::string", "[core/driver]")
{
	std::string name("Test String Base");
	TestDriverBase d(name);

	CHECK(false == d.started());
	CHECK(0 == name.compare(d.name()));
	CHECK(DriverType::Undefined == d.DriverType());
}

TEST_CASE("Create driver base class with std::string_view", "[core/driver]")
{
	std::string_view name("Test String Base");
	TestDriverBase d(name);

	CHECK(false == d.started());
	CHECK(0 == name.compare(d.name()));
	CHECK(DriverType::Undefined == d.DriverType());
}

TEST_CASE("Driver Type is set during construction", "[core/driver]")
{
	std::string_view name("Test String Base");
	TestDriverBase d(name, DriverType::SPI);

	CHECK(false == d.started());
	CHECK(0 == name.compare(d.name()));
	CHECK(DriverType::SPI == d.DriverType());
}

TEST_CASE("Start then stop driver", "[core/driver]")
{
	TestDriverBase d("Test Base");

	d.start();

	CHECK(true == d.started());

	d.stop();

	CHECK(false == d.started());
}

TEST_CASE("Create dynamic driver registry", "[core/driver]")
{
	DynamicDriverRegistry<> driver_registry;

	CHECK(0 == driver_registry.count());
	CHECK(0 < driver_registry.capacity());
}

TEST_CASE("Create static driver registry", "[core/driver]")
{
	StaticDriverRegistry<32> driver_registry;

	CHECK(0 == driver_registry.count());
	CHECK(32 == driver_registry.capacity());
}

TEST_CASE("TestDriverBase is automatically added to registery", "[core/driver]")
{
	auto& p = UnitTestPlatform::inst();
	TestDriverBase d("Test base");

	CHECK((EXPECTED_UNIT_TEST_STARTING_DRIVERS + 1) == p.driverCount());
}

TEST_CASE("Find driver from registry by name", "[core/driver]")
{
	auto& p = UnitTestPlatform::inst();
	TestDriverBase d("Test base");
	TestDriverBase d2("Test base2");

	CHECK(d == p.findDriver("Test base"));
	CHECK(d2 == p.findDriver("Test base2"));
}

TEST_CASE("Find one driver from registry by type", "[core/driver]")
{
	auto& p = UnitTestPlatform::inst();
	TestDriverBase d("Test base2", DriverType::SPI);

	auto d_found = p.findDriver(DriverType::SPI);

	CHECK(d_found);
	CHECK(d == d_found);
}

TEST_CASE("Find one driver from registry by template method", "[core/driver]")
{
	auto& p = UnitTestPlatform::inst();
	i2cTestDriver d;

	auto d_found = p.findDriver<embvm::i2c::master>();
	auto d2_found = p.findDriver(DriverType::I2C);

	CHECK(&d_found.value() == &d2_found.value());
	CHECK(d_found);
}

TEST_CASE("Find one driver from registry by template method via name", "[core/driver]")
{
	auto& p = UnitTestPlatform::inst();
	i2cTestDriver d("i2c0");

	auto d_found = p.findDriver<embvm::i2c::master>("i2c0");

	CHECK(&d_found.value() == &d);
	CHECK(d_found);
}

TEST_CASE("Find fails if no driver of that type is registered", "[core/driver]")
{
	auto& p = UnitTestPlatform::inst();

	auto d1_found = p.findDriver(DriverType::SPI);

	CHECK(!d1_found);
}

TEST_CASE("Find all drivers from registry by given type", "[core/driver]")
{
	auto& p = UnitTestPlatform::inst();
	TestDriverBase d("Test base", DriverType::SPI);
	TestDriverBase d2("Test base2", DriverType::SPI);

	auto found_list = p.findAllDrivers(DriverType::SPI);

	CHECK(2 <= found_list.size());

	for(const auto& t : found_list)
	{
		CHECK(t->DriverType() == DriverType::SPI);
		CHECK(((t == &d) || (t == &d2)));
	}
}

TEST_CASE("Find all drivers from registry by template method", "[core/driver]")
{
	auto& p = UnitTestPlatform::inst();
	i2cTestDriver d("Test");
	i2cTestDriver d2("Test2");

	auto found_list = p.findAllDrivers<embvm::i2c::master>();

	CHECK(2 <= found_list.size());

	for(const auto& t : found_list)
	{
		CHECK(t->DriverType() == DriverType::I2C);
		CHECK(((t == &d) || (t == &d2)));
	}
}

TEST_CASE("Find all returns empty list with no matches", "[core/driver]")
{
	auto& p = UnitTestPlatform::inst();

	auto found_list = p.findAllDrivers(DriverType::SPI);

	CHECK(0 == found_list.size());
}

TEST_CASE("i2c driver tests", "[core/driver/i2c]")
{
	i2cTestDriver d;
	d.configure(embvm::i2c::baud::standard);

	SECTION("Check default values", "[core/driver/i2c]")
	{
		CHECK(i2cTestDriver::type() == DriverType::I2C);
		CHECK(0 == strcmp(d.name_cstr(), "Unidentified I2C Driver"));
		CHECK(embvm::i2c::baud::standard == d.baudrate());
		CHECK(embvm::i2c::status::ok == d.busStatus());
	}

	SECTION("Set baudrate", "[core/driver/i2c]")
	{
		d.baudrate(embvm::i2c::baud::fast);
		CHECK(embvm::i2c::baud::fast == d.baudrate());
	}

	SECTION("Configure updates baudrate", "[core/driver/i2c]")
	{
		d.configure(embvm::i2c::baud::fast);
		CHECK(embvm::i2c::baud::fast == d.baudrate());
	}

	SECTION("Configure updates pullups", "[core/driver/i2c]")
	{
		d.configure(embvm::i2c::baud::standard, embvm::i2c::pullups::internal);
		CHECK(embvm::i2c::pullups::internal == d.pullups());
	}

	SECTION("Transfer test #1", "[core/driver/i2c]")
	{
		embvm::i2c::op_t op;
		auto status = d.transfer(op);

		CHECK(embvm::i2c::status::ok == status);
	}

	SECTION("Sweep test", "[core/driver/i2c]")
	{
		embvm::i2c::master::sweep_list_t list;
		d.sweep(list, [&]() {
			CHECK(128 == list.size());
			for(uint8_t i = 0; i < 128; i++)
			{
				CHECK(i == list[i]);
			}
		});
	}
}

TEST_CASE("Create an active I2C Driver Object", "[core/driver/i2c]")
{
	i2cTestDriver d;
	embvm::i2c::activeMaster<> ActiveD(d);
}

TEST_CASE("SPI driver tests", "[core/driver/spi]")
{
	spiTestDriver d;

	CHECK(0 == strcmp(d.name_cstr(), "Unidentified SPI Driver"));

	SECTION("Check default values", "[core/driver/spi]")
	{
		CHECK(embvm::spi::mode::mode0 == d.mode());
		CHECK(embvm::spi::order::msbFirst == d.order());
		CHECK(125000 == d.baudrate());
		CHECK(embvm::comm::status::ok == d.busStatus());
	}

	SECTION("Set baudrate", "[core/driver/spi]")
	{
		d.baudrate(1000000);
		CHECK(1000000 == d.baudrate());
	}

	SECTION("Configure updates baudrate", "[core/driver/spi]")
	{
		d.configure(1000000);
		CHECK(1000000 == d.baudrate());
	}

	SECTION("Mode is updated after setting", "[core/driver/spi]")
	{
		d.mode(embvm::spi::mode::mode3);
		CHECK(embvm::spi::mode::mode3 == d.mode());
	}

	SECTION("Data order is updated after setting", "[core/driver/spi]")
	{
		d.order(embvm::spi::order::lsbFirst);
		CHECK(embvm::spi::order::lsbFirst == d.order());
	}
}

TEST_CASE("SPI driver transfer tests", "[core/driver/spi]")
{
	spiTestDriver d;

	SECTION("tx_buffer as nullptr sends dummy bytes", "[core/driver/spi]")
	{
		uint8_t buffer = 0;
		embvm::spi::op_t op = {nullptr, nullptr, 1};

		auto err = d.transfer(op);

		CHECK(embvm::comm::status::ok == err);
		CHECK(true == d.checkTxBuffer(&buffer, 1));
	}

	SECTION("tx_buffer as non-nullptr sends data", "[core/driver/spi]")
	{
		uint8_t input = 0xAB;

		embvm::spi::op_t op = {&input, nullptr, 1};

		auto err = d.transfer(op);

		CHECK(embvm::comm::status::ok == err);
		CHECK(true == d.checkTxBuffer(&input, 1));
	}

	SECTION("We can read bytes over SPI", "[core/driver/spi]")
	{
		uint8_t expected[2] = {0xBE, 0xAE};
		uint8_t actual[2];
		d.appendToRxBuffer(expected, sizeof(expected));

		embvm::spi::op_t op = {nullptr, actual, 2};

		auto err = d.transfer(op);

		CHECK(embvm::comm::status::ok == err);
		for(size_t i = 0; i < sizeof(expected); i++)
		{
			CHECK(actual[i] == expected[i]);
		}
	}

	SECTION("We can write and read bytes over SPI", "[core/driver/spi]")
	{
		uint8_t input[2] = {0xAA, 0xBB};
		uint8_t expected[2] = {0xBE, 0xAE};
		uint8_t actual[2];
		embvm::spi::op_t op = {input, actual, 2};

		d.appendToRxBuffer(expected, sizeof(expected));

		auto err = d.transfer(op);

		CHECK(embvm::comm::status::ok == err);
		CHECK(true == d.checkTxBuffer(input, sizeof(input)));
		for(size_t i = 0; i < sizeof(expected); i++)
		{
			CHECK(actual[i] == expected[i]);
		}
	}
}

TEST_CASE("SPI Callback is called after transaction", "[core/driver/spi]")
{
	spiTestDriver d;
	callback_called = false;
	uint8_t input[2] = {0xAA, 0xBB};
	uint8_t expected[2] = {0xBE, 0xAE};
	uint8_t actual[2];
	embvm::spi::op_t op = {input, actual, 2};

	d.appendToRxBuffer(expected, sizeof(expected));

	auto err = d.transfer(op, spi_callback);

	CHECK(embvm::comm::status::ok == err);
	CHECK(true == d.checkTxBuffer(input, sizeof(input)));
	for(size_t i = 0; i < sizeof(expected); i++)
	{
		CHECK(actual[i] == expected[i]);
	}

	CHECK(true == callback_called);
}

TEST_CASE("Active Object enqueues and runs multiple operations", "[core/driver/spi]")
{
	spiTestDriver driver_base;
	embvm::spi::activeMaster<> d(driver_base);
	call_count = 0;
	embvm::spi::op_t op = {nullptr, nullptr, 2};

	auto cb = [](embvm::spi::op_t input, embvm::comm::status status) {
		(void)input;
		(void)status;
		call_count++;
	};

	d.enqueue({op, cb});
	d.enqueue({op, cb});

	std::this_thread::sleep_for(std::chrono::milliseconds(1));

	CHECK(0 == d.queuedCount());
	CHECK(2 == call_count);
}

TEST_CASE("SPI dispatch tests", "[core/driver/spi]")
{
	auto& p = UnitTestPlatform::inst();
	spiTestDriver d(p.getBoundDispatch());

	callback_called = false;
	uint8_t input[2] = {0xAA, 0xBB};
	uint8_t expected[2] = {0xBE, 0xAE};
	uint8_t actual[2];
	embvm::spi::op_t op = {input, actual, 2};

	d.appendToRxBuffer(expected, sizeof(expected));

	auto err = d.transfer(op, spi_callback);

	std::this_thread::sleep_for(std::chrono::milliseconds(1));

	CHECK(embvm::comm::status::ok == err);
	CHECK(true == d.checkTxBuffer(input, sizeof(input)));
	for(size_t i = 0; i < sizeof(expected); i++)
	{
		CHECK(actual[i] == expected[i]);
	}

	CHECK(true == callback_called);
}

TEST_CASE("GPIO tests", "[core/driver/gpio]")
{
	UnitTestGPIOInput g;

	SECTION("Check defaults", "[core/driver/gpio]")
	{
		CHECK(embvm::gpio::pull::none == g.pull());
		CHECK(embvm::gpio::direction::in == g.direction());
		CHECK(false == g.get());
	}

	SECTION("Set pull", "[core/driver/gpio]")
	{
		UnitTestGPIOInputWithPullup g2;
		CHECK(embvm::gpio::pull::pullup == g2.pull());
		CHECK(true == g2.get());
	}
}
