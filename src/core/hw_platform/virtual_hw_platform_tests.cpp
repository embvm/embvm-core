#include "../../hw_platforms/unit_test/unittest_hw_platform.hpp"
#include "virtual_hw_platform.hpp"
#include <catch2/catch.hpp>

TEST_CASE("Create Unit Test HW Platform with default name")
{
	UnitTestHWPlatform p;

	CHECK(0 == strcmp(p.name_cstr(), "Unit Test HW Platform"));
}
