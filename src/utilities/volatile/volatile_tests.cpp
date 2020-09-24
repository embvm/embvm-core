#include "volatile.hpp"
#include <test.hpp>

using namespace embutil;

TEST_CASE("Load/store non-volatile", "[utilities/volatile]")
{
	uint8_t t = 0;
	auto x = volatile_load(&t);
	CHECK(x == t);
	x++;
	volatile_store(&t, x);
	CHECK(t == 1);
}

TEST_CASE("Load/store volatile", "[utilities/volatile]")
{
	volatile uint8_t t = 0;
	auto x = volatile_load(&t);
	CHECK(x == t);
	x++;
	volatile_store(&t, x);
	CHECK(t == 1);
}
