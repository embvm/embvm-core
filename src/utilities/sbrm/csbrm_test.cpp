// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#include "csbrm.hpp"
#include <catch2/catch_test_macros.hpp>

using namespace embutil;

namespace
{
struct CSBRM_tester
{
	bool initialized_ = false;
	bool deleted_ = false;
};

CSBRM_tester* create(CSBRM_tester* t)
{
	t->initialized_ = true;
	return t;
}

void destroy(CSBRM_tester* t)
{
	t->deleted_ = true;
}

} // namespace

TEST_CASE("unique_resource", "[utilities/csbrm]")
{
	CSBRM_tester test_val;
	auto resource = unique_resource<CSBRM_tester>(create, destroy, &test_val);
	CHECK(test_val.initialized_);
	resource.reset();
	CHECK(test_val.deleted_);
}

TEST_CASE("shared_resource", "[utilities/csbrm]")
{
	CSBRM_tester test_val;
	auto resource = shared_resource<CSBRM_tester>(create, destroy, &test_val);
	CHECK(test_val.initialized_);

	auto resource2 = resource;
	resource.reset();
	CHECK(test_val.deleted_ == false);

	resource2.reset();
	CHECK(test_val.deleted_);
}
