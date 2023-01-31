// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#include "aligned_ptr.hpp"
#include <bits/bits.hpp>
#include <catch2/catch_test_macros.hpp>

using namespace embutil;

TEST_CASE("Unique aligned ptr", "[utilities/aligned_ptr]")
{
	auto ptr = aligned_uptr<uint8_t>(8, 1);
	auto ptr_raw = ptr.get();
	CHECK(is_aligned(ptr_raw, 8));

	auto ptr2 = aligned_uptr<uint8_t>(16, 1);
	auto ptr2_raw = ptr2.get();
	CHECK(is_aligned(ptr2_raw, 16));
}

TEST_CASE("Shared aligned ptr", "[utilities/aligned_ptr]")
{
	auto ptr = aligned_sptr<uint8_t>(8, 1);
	auto ptr_raw = ptr.get();
	CHECK(is_aligned(ptr_raw, 8));

	auto ptr2 = aligned_sptr<uint8_t>(16, 1);
	auto ptr2_raw = ptr2.get();
	CHECK(is_aligned(ptr2_raw, 16));
}
