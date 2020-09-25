#include "time.hpp"
#include <catch2/catch.hpp>
#include <time.h>

using namespace embutil;

// TODO: add time point tests

TEST_CASE("timespecToDuration conversion tests", "[util/time]")
{
	constexpr timespec ts = {0, 100};
	STATIC_REQUIRE(std::chrono::nanoseconds(100) == timespecToDuration(ts));

	timespec ts2 = {3, 100};
	CHECK((std::chrono::seconds(3) + std::chrono::nanoseconds(100)) == timespecToDuration(ts2));
}

TEST_CASE("durationToTimespec conversion tests", "[util/time]")
{
	auto dur = std::chrono::nanoseconds(100);
	auto ts = durationToTimespec(dur);
	CHECK(((ts.tv_sec == 0) && (ts.tv_nsec == 100)));

	dur += std::chrono::seconds(1);
	ts = durationToTimespec(dur);
	CHECK(((ts.tv_sec == 1) && (ts.tv_nsec == 100)));
}

#ifdef UTIL_TIME_INCLUDE_TIMEVAL
TEST_CASE("timevalToDuration conversion tests", "[util/time]")
{
	timeval tv = {0, 100};
	auto dur = timevalToDuration(tv);
	CHECK(std::chrono::microseconds(100) == dur);
}
#endif
