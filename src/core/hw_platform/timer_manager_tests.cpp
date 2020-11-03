// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#include "timer_manager.hpp"
#include <atomic>
#include <catch2/catch.hpp>
#include <nop_lock/nop_lock.hpp>
#include <simulator/timer.hpp>

#pragma mark - Helpers -

using namespace embdrv;

static std::atomic<bool> called_ = false;
static std::atomic<unsigned> count_ = 0;

static void cb_called()
{
	called_ = true;
}

static void cb_called_count()
{
	count_++;
}

#pragma mark - Test Cases -

TEST_CASE("Create Timer Manager", "[core/platform/timer_mgr]")
{
	SimulatorTimer timer;
	embvm::TimerManager tm(timer);
	embvm::TimerManager<0, std::mutex> tm2(timer);
}

TEST_CASE("Create Static Timer Manager", "[core/platform/timer_mgr]")
{
	SimulatorTimer timer;
	embvm::TimerManager<10, embutil::nop_lock, embvm::timer::timer_period_t> tm(timer);
}

TEST_CASE("Basic Timer Handle Tests", "[core/platform/timer_mgr]")
{
	SimulatorTimer timer;
	embvm::TimerManager<0, std::mutex> tm(timer);

	SECTION("Create timer handle", "[core/platform/timer_mgr]")
	{
		auto h = tm.allocate();
		CHECK(true == h.valid());
	}

	SECTION("Move invalidates timer handle", "[core/platform/timer_mgr]")
	{
		auto h = tm.allocate();
		auto h2 = std::move(h);

		CHECK(true == h2.valid());
		CHECK(false == h.valid());
	}

	SECTION("Cancelling unscheduled timer", "[core/platform/timer_mgr]")
	{
		auto h = tm.allocate();
		auto canceled = h.cancel();

		CHECK(false == canceled);
	}

	SECTION("Schedule then cancel delay", "[core/platform/timer_mgr]")
	{
		count_ = 0;
		auto h = tm.allocate();

		h.periodicDelay(std::chrono::milliseconds(1), cb_called_count);
		auto canceled = h.cancel();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));

		CHECK(true == canceled);
		CHECK(3 >= count_);
	}
}

TEST_CASE("Timer Handler Scheduling Tests", "[core/platform/timer_mgr]")
{
	SimulatorTimer timer;
	embvm::TimerManager<0, std::mutex> tm(timer);

	SECTION("Schedule async delay", "[core/platform/timer_mgr]")
	{
		called_ = false;
		auto h = tm.allocate();

		h.asyncDelay(std::chrono::milliseconds(1), cb_called);

		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		CHECK(true == called_);
	}

	SECTION("Schedule mutiple async delays", "[core/platform/timer_mgr]")
	{
		count_ = 0;
		auto h = tm.allocate();
		auto h2 = tm.allocate();
		auto h3 = tm.allocate();

		h.asyncDelay(std::chrono::milliseconds(3), cb_called_count);
		h2.asyncDelay(std::chrono::milliseconds(1), cb_called_count);
		h3.asyncDelay(std::chrono::milliseconds(2), cb_called_count);

		std::this_thread::sleep_for(std::chrono::milliseconds(5));

		CHECK(3 == count_);
	}

	SECTION("Schedule periodic delay", "[core/platform/timer_mgr]")
	{
		count_ = 0;
		auto h = tm.allocate();

		h.periodicDelay(std::chrono::milliseconds(1), cb_called_count);
		std::this_thread::sleep_for(std::chrono::milliseconds(10));

		// I don't care that it's perfect - just that it gets rescheduled
		CHECK(2 <= count_);
	}
}

TEST_CASE("Static Basic Timer Handle Tests", "[core/platform/timer_mgr]")
{
	SimulatorTimer timer;
	embvm::TimerManager<10, std::mutex, embvm::timer::timer_period_t> tm(timer);

	SECTION("Create timer handle", "[core/platform/timer_mgr]")
	{
		auto h = tm.allocate();
		CHECK(true == h.valid());
	}

	SECTION("Move invalidates timer handle", "[core/platform/timer_mgr]")
	{
		auto h = tm.allocate();
		auto h2 = std::move(h);

		CHECK(true == h2.valid());
		CHECK(false == h.valid());
	}

	SECTION("Cancelling unscheduled timer", "[core/platform/timer_mgr]")
	{
		auto h = tm.allocate();
		auto canceled = h.cancel();

		CHECK(false == canceled);
	}

	SECTION("Schedule then cancel delay", "[core/platform/timer_mgr]")
	{
		count_ = 0;
		auto h = tm.allocate();

		h.periodicDelay(std::chrono::milliseconds(1), cb_called_count);
		auto canceled = h.cancel();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));

		CHECK(true == canceled);
		CHECK(3 >= count_);
	}
}

TEST_CASE("Static Timer Handler Scheduling Tests", "[core/platform/timer_mgr]")
{
	SimulatorTimer timer;
	embvm::TimerManager<10, std::mutex> tm(timer);

	SECTION("Schedule async delay", "[core/platform/timer_mgr]")
	{
		called_ = false;
		auto h = tm.allocate();

		h.asyncDelay(std::chrono::milliseconds(1), cb_called);

		std::this_thread::sleep_for(std::chrono::milliseconds(2));

		CHECK(true == called_);
	}

	SECTION("Schedule mutiple async delays", "[core/platform/timer_mgr]")
	{
		count_ = 0;
		auto h = tm.allocate();
		auto h2 = tm.allocate();
		auto h3 = tm.allocate();

		h.asyncDelay(std::chrono::milliseconds(3), cb_called_count);
		h2.asyncDelay(std::chrono::milliseconds(1), cb_called_count);
		h3.asyncDelay(std::chrono::milliseconds(2), cb_called_count);
		std::this_thread::sleep_for(std::chrono::milliseconds(5));

		CHECK(3 == count_);
	}

	SECTION("Schedule periodic delay", "[core/platform/timer_mgr]")
	{
		count_ = 0;
		auto h = tm.allocate();

		h.periodicDelay(std::chrono::milliseconds(1), cb_called_count);
		std::this_thread::sleep_for(std::chrono::milliseconds(10));

		// I don't care that it's perfect - just that it gets rescheduled
		CHECK(2 <= count_);
	}
}
