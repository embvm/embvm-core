/*
 * Copyright © 2018 Embedded Artistry LLC.
 */

#include "dispatch.hpp"
#include <atomic>
#include <catch2/catch.hpp>
#include <chrono>
#include <thread>

#pragma mark - Helpers -

static std::atomic<size_t> flag = 0;

static void test_flag(void)
{
	flag = 1;
}

static void test_count(void)
{
	flag++;
}

#pragma mark - Test Cases -

TEST_CASE("Create static dispatch queue", "[utility/dispatch/static]")
{
	const size_t qsize = 10;
	const size_t num_threads = 1;
	embutil::StaticDispatchQueue<qsize, num_threads> q("TestQueue");

	SECTION("Queue starts empty")
	{
		CHECK(0 == q.queue_size());
	}

	SECTION("Queue Capacity non-zero")
	{
		CHECK(10 == q.capacity());
	}

	SECTION("Queue threads matches definition")
	{
		CHECK(num_threads == q.thread_count());
	}
}

TEST_CASE("Dispatch function runs after added to static queue", "[utility/dispatch/static]")
{
	const size_t qsize = 128;
	flag = 0;
	embutil::StaticDispatchQueue<qsize> q("TestQueue");

	q.dispatch(test_flag);

	std::this_thread::sleep_for(std::chrono::milliseconds(1));

	CHECK(flag == 1);
}

TEST_CASE("Dispatch lambda runs after added to static queue", "[utility/dispatch/static]")
{
	const size_t qsize = 128;
	flag = 0;
	embutil::StaticDispatchQueue<qsize> q("TestQueue");

	q.dispatch([] { flag = 2; });

	std::this_thread::sleep_for(std::chrono::milliseconds(1));

	CHECK(flag == 2);
}

TEST_CASE("Fill static dispatch queue then empty", "[utility/dispatch/static]")
{
	const size_t qsize = 128;
	const size_t num_threads = 1;
	flag = 0;
	embutil::StaticDispatchQueue<qsize, num_threads> q("TestQueue");

	for(size_t i = 0; i < q.capacity(); i++)
	{
		q.dispatch(test_count);
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(1));

	CHECK(flag == q.capacity());
}
