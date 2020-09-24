/*
 * Copyright © 2018 Embedded Artistry LLC.
 */

#include "dispatch.hpp"
#include <atomic>
#include <chrono>
#include <test.hpp>
#include <thread>

using namespace embutil;

#pragma mark - Helpers -

static std::atomic<int> flag = 0;

static void test_flag(void)
{
	flag = 1;
}

static void test_count(void)
{
	flag = flag + 1;
}

#pragma mark - Test Cases -

TEST_CASE("Create dynamic dispatch queue", "[utility/dispatch/dynamic]")
{
	const size_t num_threads = 1;
	embutil::DynamicDispatchQueue<> q("TestQueue", num_threads);

	SECTION("Queue starts empty")
	{
		CHECK(0 == q.queue_size());
	}

	SECTION("Queue threads matches definition")
	{
		CHECK(num_threads == q.thread_count());
	}
}

TEST_CASE("Dynamic Dispatch function adds to queue", "[utility/dispatch/dynamic]")
{
	const size_t num_threads = 0;
	flag = 0;
	embutil::DynamicDispatchQueue<> q("TestQueue", num_threads);
	q.dispatch(test_flag);

	CHECK(1 == q.queue_size());
}

TEST_CASE("Fill dynamic dispatch queue", "[utility/dispatch/dynamic]")
{
	const size_t num_threads = 0;
	embutil::DynamicDispatchQueue<> q("TestQueue", num_threads);

	for(size_t i = 0; i < 10; i++)
	{
		q.dispatch(test_flag);
	}

	CHECK(10 == q.queue_size());
}

TEST_CASE("Dynamic Dispatch function runs after added to queue", "[utility/dispatch/dynamic]")
{
	flag = 0;
	embutil::DynamicDispatchQueue<> q("TestQueue");

	q.dispatch(test_flag);

	std::this_thread::sleep_for(std::chrono::milliseconds(1));

	CHECK(flag == 1);
}

TEST_CASE("Dynamic Dispatch lambda runs after added to queue", "[utility/dispatch/dynamic]")
{
	flag = 0;
	embutil::DynamicDispatchQueue<> q("TestQueue");

	q.dispatch([] { flag = 2; });

	std::this_thread::sleep_for(std::chrono::milliseconds(1));

	CHECK(flag == 2);
}

TEST_CASE("Add to dynamic dispatch queue then empty", "[utility/dispatch/dynamic]")
{
	const size_t num_threads = 1;
	flag = 0;
	embutil::DynamicDispatchQueue<> q("TestQueue", num_threads);

	for(size_t i = 0; i < 10; i++)
	{
		q.dispatch(test_count);
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(1));

	CHECK(flag == 10);
}
