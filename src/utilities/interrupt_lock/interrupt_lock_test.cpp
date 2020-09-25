/*
 * Copyright © 2018 Embedded Artistry LLC.
 */

#include "interrupt_lock.hpp"
#include <chrono>
#include <cstdint>
#include <catch2/catch.hpp>
#include <thread>
#include <volatile/volatile.hpp>

using namespace embutil;

#pragma mark - Helpers -

#define INTERRUPTS_DISABLED (1 << 0)
#define INTERRUPTS_ENABLED (1 << 1)

static uint8_t interrupt_flag;

struct TestInterruptLockPolicy
{
	using TReturn = bool;

	static void enable_interrupts(bool enable)
	{
		(void)enable;

		auto t = embutil::volatile_load(&interrupt_flag);
		t |= INTERRUPTS_ENABLED;
		embutil::volatile_store(&interrupt_flag, t);
	}

	/// Disable interrupts and return the current setting
	static TReturn disable_interrupts()
	{
		auto t = embutil::volatile_load(&interrupt_flag);
		t |= INTERRUPTS_DISABLED;
		embutil::volatile_store(&interrupt_flag, t);

		return true;
	}
};

#pragma mark - Test Cases -

TEST_CASE("Basic Lock disables interrupts", "[utility/interrupt_lock/basic]")
{
	InterruptLock<TestInterruptLockPolicy> lock;
	interrupt_flag = 0;

	lock.lock();

	CHECK(embutil::volatile_load(&interrupt_flag) == INTERRUPTS_DISABLED);
}

TEST_CASE("Basic Unlock enables interrupts", "[utility/interrupt_lock/basic]")
{
	InterruptLock<TestInterruptLockPolicy> lock;

	lock.lock();

	interrupt_flag = 0;

	lock.unlock();

	CHECK(embutil::volatile_load(&interrupt_flag) == INTERRUPTS_ENABLED);
}
