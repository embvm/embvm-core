// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#include "scope_guard.hpp"
#include <catch2/catch_test_macros.hpp>

using namespace embutil;

static volatile bool test_flag_ = false;

static void test_guard()
{
	test_flag_ = true;
}

TEST_CASE("Create a scope_guard class", "[utility/scope_guard]")
{
	auto guard = makeScopeGuard([]() {});

	CHECK(false == guard.isReleased());
}

TEST_CASE("Release a scope_guard class", "[utility/scope_guard]")
{
	auto guard = makeScopeGuard([]() {});

	guard.release();
	CHECK(true == guard.isReleased());
}

TEST_CASE("Scope guard is called on scope exit", "[utility/scope_guard]")
{
	test_flag_ = false;

	{
		auto guard = makeScopeGuard(&test_guard);
		(void)guard; // silence cppcheck
	}

	CHECK(true == test_flag_);
}

TEST_CASE("Scope guard is not called on scope exit after release", "[utility/scope_guard]")
{
	test_flag_ = false;

	{
		auto guard = makeScopeGuard(&test_guard);
		guard.release();
	}

	CHECK(false == test_flag_);
}
