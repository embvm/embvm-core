#include <atomic>
#include <catch2/catch.hpp>
#include <simulator/system_clock.hpp>
#include <simulator/timer.hpp>

using namespace embdrv;

static std::atomic<bool> flagged_ = false;
static std::atomic<int> call_count_ = 0;

static void timer_callback(void)
{
	flagged_ = true;
}

static void timer_callback_count(void)
{
	call_count_++;
}

TEST_CASE("System Timer", "[driver/simulator/system_clock]")
{
	SimulatorSystemClock timer;

	auto start = timer.ticks();
	timer.spin(100);
	auto stop = timer.ticks();

	CHECK(0 != timer.frequency());
	CHECK(100 <= stop - start);
}

TEST_CASE("Timer", "[driver/simulator/timer]")
{
	SimulatorTimer t(std::chrono::microseconds(100));

	SECTION("Created checks", "[driver/simulator/timer]")
	{
		CHECK(embvm::timer::state::stopped == t.state());
	}

	SECTION("Start timer", "[driver/simulator/timer]")
	{
		CHECK(embvm::timer::state::stopped == t.state());
		t.start();

		// Sometimes our timer finishes before we return to this check,
		// So expired is allowed
		CHECK(((embvm::timer::state::armed == t.state()) ||
			   (embvm::timer::state::expired == t.state())));
	}

	SECTION("Timer calls CB when expired", "[driver/simulator/timer]")
	{
		flagged_ = false;
		t.registerCallback(timer_callback);
		t.start();
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		CHECK(embvm::timer::state::expired == t.state());
		CHECK(true == flagged_);
	}

	SECTION("Restart timer", "[driver/simulator/timer]")
	{
		flagged_ = false;
		t.registerCallback(timer_callback);
		t.restart();
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		CHECK(embvm::timer::state::expired == t.state());
		CHECK(true == flagged_);
	}

	SECTION("Stop timer", "[driver/simulator/timer]")
	{
		call_count_ = 0;
		t.registerCallback(timer_callback_count);
		t.start();
		t.stop();
		CHECK(embvm::timer::state::stopped == t.state());
		CHECK(1 >= call_count_);
	}

	SECTION("Periodic timer", "[driver/simulator/timer]")
	{
		call_count_ = 0;

		t.config(embvm::timer::config::periodic);
		t.registerCallback(timer_callback_count);

		t.start();
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		t.stop();

		CHECK(embvm::timer::state::stopped == t.state());
		// We don't need it to be perfect... POSIX threads prevent precision
		CHECK(call_count_ >= 2);
	}
}
