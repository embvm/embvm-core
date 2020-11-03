// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#include "function_queue.hpp"
#include <catch2/catch.hpp>

using namespace embutil;

#pragma mark - Helper Functions -

static uint8_t test_counter;
static uint8_t test_counter2;

static void test_func()
{
	test_counter++;
}

static void test_func2()
{
	test_counter2++;
}

#pragma mark - Test Cases -

TEST_CASE("Create functional queue", "[utility/function_queue]")
{
	StaticFunctionQueue<128> fq;
	CHECK(0 == fq.size());
	CHECK(true == fq.empty());
	CHECK(128 == fq.capacity());
}

TEST_CASE("Push and pop function via move", "[utility/function_queue]")
{
	StaticFunctionQueue<128> fq;
	stdext::inplace_function<void()> func = []() {};
	stdext::inplace_function<void(), 4> func2 = []() {};

	fq.push(std::move(func));
	fq.push(std::move(func2));
	CHECK(0 < fq.size());
	fq.popAndExec();
	fq.popAndExec();
	CHECK(0 == fq.size());
}

TEST_CASE("Push and pop function via copy", "[utility/function_queue]")
{
	embutil::StaticFunctionQueue<128> fq;
	test_counter = 0;
	test_counter2 = 0;

	fq.push(test_func);
	fq.push(test_func2);
	fq.push(test_func);
	fq.push(test_func2);
	CHECK(0 < fq.size());

	fq.popAndExec();
	fq.popAndExec();
	fq.popAndExec();
	fq.popAndExec();
	CHECK(0 == fq.size());
	CHECK(2 == test_counter);
	CHECK(2 == test_counter2);
}

TEST_CASE("Push, manually exec from front", "[utility/function_queue]")
{
	embutil::StaticFunctionQueue<128> fq;
	test_counter = 0;

	fq.push(test_func);

	auto f = fq.front();
	fq.pop();

	f->exec();

	CHECK(0 == fq.size());
	CHECK(1 == test_counter);
}

TEST_CASE("Test use with std::function", "[utility/function_queue]")
{
	StaticFunctionQueue<1024, 128, std::function<void()>> fq;
	test_counter = 0;
	test_counter2 = 0;

	fq.push(test_func);
	fq.push(test_func2);
	fq.push(test_func);
	fq.push(test_func2);
	CHECK(0 < fq.size());

	fq.popAndExec();
	fq.popAndExec();
	fq.popAndExec();
	fq.popAndExec();
	CHECK(0 == fq.size());
	CHECK(2 == test_counter);
	CHECK(2 == test_counter2);
}
