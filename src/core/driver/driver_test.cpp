// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#include <atomic>
#include <catch2/catch.hpp>
#include <cstring>
#include <driver/active_i2c.hpp>
#include <driver/active_spi.hpp>
#include <platform.hpp>
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

TEST_CASE("Create driver base class", "[core/driver]")
{
	TestDriverBase d{};

	CHECK(false == d.started());
	CHECK(DriverType::Undefined == d.DriverType());
}

TEST_CASE("Driver Type is set during construction", "[core/driver]")
{
	TestDriverBase d(DriverType::SPI);

	CHECK(false == d.started());
	CHECK(DriverType::SPI == d.DriverType());
}

TEST_CASE("Start then stop driver", "[core/driver]")
{
	TestDriverBase d{};

	d.start();

	CHECK(true == d.started());

	d.stop();

	CHECK(false == d.started());
}

TEST_CASE("i2c driver tests", "[core/driver/i2c]")
{
	i2cTestDriver d;
	d.configure(embvm::i2c::baud::standard);

	SECTION("Check default values", "[core/driver/i2c]")
	{
		CHECK(i2cTestDriver::type() == DriverType::I2C);
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
		uint8_t tx_buffer[] = {0x0, 0x1, 0x2};
		uint8_t rx_buffer[3];
		uint8_t rx_expected[] = {0xa, 0xb, 0x3};

		embvm::i2c::op_t op = {.address = 0x29,
							   .op = embvm::i2c::operation::writeRead,
							   .tx_size = sizeof(tx_buffer),
							   .tx_buffer = tx_buffer,
							   .rx_size = sizeof(rx_buffer),
							   .rx_buffer = rx_buffer};

		d.clearTxBuffer();
		d.clearRxBuffer();
		d.appendToRxBuffer(rx_expected, sizeof(rx_expected));

		auto status = d.transfer(op);

		CHECK(d.checkTxBuffer(tx_buffer, sizeof(tx_buffer)));
		CHECK(0 == memcmp(rx_buffer, rx_expected, sizeof(rx_buffer)));
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
	UnitTestGPIO g;

	SECTION("Check input", "[core/driver/gpio]")
	{
		g.setMode(embvm::gpio::mode::input);
		CHECK(embvm::gpio::mode::input == g.mode());
		CHECK(false == g.get());
	}

	SECTION("Check output", "[core/driver/gpio]")
	{
		g.setMode(embvm::gpio::mode::output);
		CHECK(embvm::gpio::mode::output == g.mode());
		CHECK(false == g.get());
		g.set(true);
		CHECK(true == g.get());
		g.toggle();
		CHECK(false == g.get());
	}
}
