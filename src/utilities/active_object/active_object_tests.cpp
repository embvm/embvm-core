// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#include "active_object.hpp"
#include <catch2/catch_test_macros.hpp>
#include <chrono>

class TestActiveObject final : public embutil::activeObject<TestActiveObject, uint32_t>
{
  public:
	size_t processed_cnt()
	{
		return processed_cnt_;
	}

	void process_(uint32_t e)
	{
		(void)e;
		processed_cnt_++;
	}

  private:
	size_t processed_cnt_ = 0;
};

TEST_CASE("Create AO", "[utility/active_object]")
{
	TestActiveObject ao;

	CHECK(0 == ao.queuedCount());
}

TEST_CASE("Push to AO & Process", "[utility/active_object]")
{
	TestActiveObject ao;

	ao.enqueue(UINT32_C(38));
	ao.enqueue(UINT32_C(38));
	ao.enqueue(UINT32_C(38));
	auto cnt = ao.queuedCount();
	CHECK(0 < cnt);

	std::this_thread::sleep_for(std::chrono::milliseconds(1));

	CHECK(0 == ao.queuedCount());
	CHECK(3 == ao.processed_cnt());
}

TEST_CASE("Push to AO & Shutdown", "[utility/active_object]")
{
	TestActiveObject ao;

	// Shut it down so that we aren't running
	ao.shutdown();

	// Enqueue operations
	ao.enqueue(UINT32_C(38));
	ao.enqueue(UINT32_C(38));
	ao.enqueue(UINT32_C(38));
	ao.enqueue(UINT32_C(38));
	ao.enqueue(UINT32_C(38));
	ao.enqueue(UINT32_C(38));
	ao.enqueue(UINT32_C(38));
	ao.enqueue(UINT32_C(38));
	ao.enqueue(UINT32_C(38));
	ao.enqueue(UINT32_C(38));
	ao.enqueue(UINT32_C(38));

	// Check that they aren't executed
	CHECK(11 == ao.queuedCount());

	std::this_thread::sleep_for(std::chrono::milliseconds(1));

	CHECK(11 == ao.queuedCount());
}
