/*
 * Copyright © 2018 Embedded Artistry LLC.
 */

#include "dispatch.hpp"
#include "interrupt_queue.hpp"
#include <atomic>
#include <catch2/catch.hpp>
#include <chrono>
#include <cstdint>
#include <interrupt_lock/interrupt_lock.hpp>
#include <posix_event_flags.hpp>
#include <thread>

#pragma mark - Helpers -

#define INTERRUPTS_DISABLED (1 << 0)
#define INTERRUPTS_ENABLED (1 << 1)
#define RETRIES_MAX 10

static std::atomic<int> flag = 0;
static std::atomic<int> functions_dispatched = 0;
static std::atomic<uint8_t> interrupt_flag;

using namespace embutil;

struct TestInterruptQueueLockPolicy
{
	using TReturn = bool;

	static void enable_interrupts(bool enable)
	{
		(void)enable;

		interrupt_flag |= INTERRUPTS_ENABLED;
	}

	/// Disable interrupts and return the current setting
	static TReturn disable_interrupts()
	{
		interrupt_flag |= INTERRUPTS_DISABLED;

		return true;
	}
};

using IRQLock = InterruptLock<TestInterruptQueueLockPolicy>;

static void test_call()
{
	flag = 1;
}

static void dispatch_check()
{
	functions_dispatched = functions_dispatched + 1;
}

#pragma mark - Test Cases -

TEST_CASE("Create interrupt queue with standalone dispatch function", "[utility/interrupt_queue]")
{
	const size_t qsize = 10;
	InterruptQueue<IRQLock, qsize> q;

	SECTION("Queue starts empty")
	{
		CHECK(0 == q.queue_size());
	}

	SECTION("Queue Capacity Matches Definition")
	{
		CHECK(qsize == q.capacity());
	}

	SECTION("Queue threads matches definition")
	{
		CHECK(1 == q.thread_count());
	}
}

TEST_CASE("Use interrupt queue with dispatch queue", "[utility/interrupt_queue]")
{
	const size_t qsize = 10;
	flag = 0;
	InterruptQueue<IRQLock, qsize> q;

	SECTION("Queue starts empty")
	{
		CHECK(0 == q.queue_size());
	}

	SECTION("Queue Capacity Matches Definition")
	{
		CHECK(qsize == q.capacity());
	}

	SECTION("Queue threads matches definition")
	{
		CHECK(1 == q.thread_count());
	}

	SECTION("Dispatch works")
	{
		uint8_t tries = 0;

		q.dispatch(test_call);

		do
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			tries++;
		} while(q.queue_size() > 0 && tries < RETRIES_MAX);

		CHECK(1 == flag);
	}
}

TEST_CASE("Add operation to queue, interrupts are disabled/enabled", "[utility/interrupt_queue]")
{
	InterruptQueue<IRQLock, 10> q;

	q.dispatch(test_call);

	CHECK(static_cast<bool>(interrupt_flag & INTERRUPTS_DISABLED));
	CHECK(static_cast<bool>(interrupt_flag & INTERRUPTS_ENABLED));
}

TEST_CASE("Add operation to queue, operations run", "[utility/interrupt_queue]")
{
	InterruptQueue<IRQLock, 10> q;
	flag = 0;
	uint8_t tries = 0;

	q.dispatch(test_call);

	do
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		tries++;
	} while(q.queue_size() > 0 && tries < RETRIES_MAX);

	CHECK(1 == flag);
}

TEST_CASE("Add multiple operations to queue, all run", "[utility/interrupt_queue]")
{
	const size_t qsize = 10;
	InterruptQueue<IRQLock, qsize> q;
	functions_dispatched = 0;
	uint8_t tries = 0;

	for(size_t i = 0; i < qsize; i++)
	{
		q.dispatch(dispatch_check);
	}

	do
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		tries++;
	} while(q.queue_size() > 0 && tries < RETRIES_MAX);

	CHECK(qsize == functions_dispatched);
}
