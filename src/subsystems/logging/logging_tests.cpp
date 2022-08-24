// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#include "circular_buffer_logger.hpp"
#include <catch2/catch.hpp>
#include <cstring>
#include <simulator/system_clock.hpp>

using namespace embvm;

TEST_CASE("Create a logger", "[subsystem/logging]")
{
	CircularLogBufferLogger<1024> l;
	CHECK(0 == l.size());
	CHECK(1024 == l.capacity());
	CHECK(true == l.enabled());
	CHECK(false == l.echo());
	CHECK(logger::LOG_LEVEL_LIMIT == l.level());

	auto level = l.level(static_cast<logger::level>(static_cast<int>(l.level()) + 1));
	CHECK(logger::LOG_LEVEL_LIMIT == level);
}

TEST_CASE("Create a logger with a system timer", "[subsystem/logging]")
{
	embdrv::SimulatorSystemClock t;
	CircularLogBufferLogger<1024> l(t);
	CircularLogBufferLogger<1024> l_notimer;

	l.log(logger::debug, "Hello world\n");
	l_notimer.log(logger::debug, "Hello world\n");
	CHECK(l_notimer.size() < l.size());
}

TEST_CASE("Log interface", "[subsystem/logging]")
{
	CircularLogBufferLogger<1024> l;

	// We can log slightly more data than the string length
	auto expected_size = strlen("Test str") + 1;
	l.log(logger::level::warn, "Test str");
	CHECK(expected_size <= l.size());

	// Logging at an unsupported level should result in no size change
	expected_size = l.size();
	l.log(logger::level::verbose, "Gotcha");
	CHECK(expected_size == l.size());
}

TEST_CASE("Log Level to String", "[subsystem/logging]")
{
	auto crit = logger::to_str(logger::level::critical);
	CHECK(std::string_view("critical") == crit);

	auto err = logger::to_str(logger::level::error);
	CHECK(std::string_view("error") == err);

	auto warn = logger::to_str(logger::level::warn);
	CHECK(std::string_view("warning") == warn);

	auto info = logger::to_str(logger::level::info);
	CHECK(std::string_view("info") == info);

	auto debug = logger::to_str(logger::level::debug);
	CHECK(std::string_view("debug") == debug);

	auto verbose = logger::to_str(logger::level::verbose);
	CHECK(std::string_view("verbose") == verbose);

	auto off = logger::to_str(logger::level::off);
	CHECK(std::string_view("off") == off);

	// Check c_str function
	auto crit_cstr = logger::to_c_str(logger::level::critical);
	CHECK(0 == strcmp(crit_cstr, "critical"));
}

TEST_CASE("Log Level to Short String", "[subsystem/logging]")
{
	auto crit = logger::to_short_c_str(logger::level::critical);
	CHECK(0 == strcmp(LOG_LEVEL_CRITICAL_PREFIX, crit));

	auto err = logger::to_short_c_str(logger::level::error);
	CHECK(0 == strcmp(LOG_LEVEL_ERROR_PREFIX, err));

	auto warn = logger::to_short_c_str(logger::level::warn);
	CHECK(0 == strcmp(LOG_LEVEL_WARNING_PREFIX, warn));

	auto info = logger::to_short_c_str(logger::level::info);
	CHECK(0 == strcmp(LOG_LEVEL_INFO_PREFIX, info));

	auto debug = logger::to_short_c_str(logger::level::debug);
	CHECK(0 == strcmp(LOG_LEVEL_DEBUG_PREFIX, debug));

	auto verbose = logger::to_short_c_str(logger::level::verbose);
	CHECK(0 == strcmp(LOG_LEVEL_VERBOSE_PREFIX, verbose));

	auto off = logger::to_short_c_str(logger::level::off);
	CHECK(0 == strcmp("O", off));
}
