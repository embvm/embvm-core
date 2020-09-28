#include "os.hpp"
#include <aligned_malloc.h>
#include <atomic>
#include <catch2/catch.hpp>
#include <chrono>
#include <thread>

using namespace os;

#define TIMEOUT_MAX 25

static std::atomic<void*> set_input = nullptr;

static void thread_func(void* input)
{
	set_input = input;
}

static void thread_func_free(void* input)
{
	aligned_free(input);
}

static void thread_to_terminate [[noreturn]] (void* input)
{
	(void)(input);
	while(1)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

static void wait_for_thread_to_complete(posix::Thread& thread)
{
	uint8_t timeout_count_ = 0;
	while(embvm::thread::state::completed != thread.state())
	{
		timeout_count_++;
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	CHECK(timeout_count_ <= TIMEOUT_MAX);
}

TEST_CASE("Create posix thread", "[posix/os/thread]")
{
	posix::Thread thread("Test", thread_func, nullptr, embvm::thread::priority::normal, 0, nullptr);
	wait_for_thread_to_complete(thread);
	CHECK(set_input == nullptr);

	// Check input
	posix::Thread thread2("Test", thread_func, &thread, embvm::thread::priority::normal, 0,
						  nullptr);

	wait_for_thread_to_complete(thread2);

	CHECK(set_input == &thread);

	posix::Thread thread3("Test", thread_func, nullptr, embvm::thread::priority::realtime, 0,
						  nullptr);
	posix::Thread thread4("Test", thread_func, nullptr, embvm::thread::priority::lowest, 32 * 1024,
						  nullptr);

	// Using the size suggested by ThreadSanitizer
	constexpr size_t thread_alloc_size = 131072;

	// Pthread will fail if our stack is not aligned to a page boundary
	auto stack = aligned_malloc(PTHREAD_STACK_MIN / 2, thread_alloc_size);
	posix::Thread thread5("Test", thread_func_free, stack, embvm::thread::priority::lowest,
						  thread_alloc_size, stack);

	thread5.join();
	CHECK(embvm::thread::state::completed == thread5.state());

	posix::Thread thread6("Test", thread_to_terminate, nullptr);
	thread6.terminate();
	thread6.join();
	CHECK(embvm::thread::state::terminated == thread6.state());
}

TEST_CASE("Create posix mutex", "[posix/os/mutex]")
{
	auto mutex = posix::Mutex();
	mutex.lock();
	CHECK(false == mutex.trylock());
	mutex.unlock();
	CHECK(true == mutex.trylock());
	mutex.unlock();

	auto mutex2 = posix::Mutex(embvm::mutex::type::normal);
	mutex2.lock();
	mutex2.unlock();

	auto mutex3 = posix::Mutex(embvm::mutex::type::recursive);
	mutex3.lock();
	mutex3.unlock();

	auto mutex4 = posix::Mutex(embvm::mutex::type::defaultType, embvm::mutex::mode::none);
	mutex4.lock();
	mutex4.unlock();

	auto mutex5 =
		posix::Mutex(embvm::mutex::type::defaultType, embvm::mutex::mode::priorityInherit);
	mutex5.lock();
	mutex5.unlock();

	auto mutex6 = posix::Mutex(embvm::mutex::type::defaultType, embvm::mutex::mode::protect);
	mutex6.lock();
	mutex6.unlock();
}

TEST_CASE("Create posix semaphore", "[posix/os/semaphore]")
{
	auto sem = posix::Semaphore(embvm::semaphore::mode::counting, 2);
	CHECK(2 == sem.count());
	sem.take();
	sem.take();
	CHECK(0 == sem.count());
	sem.give();
	sem.give();

	auto sem2 = posix::Semaphore(embvm::semaphore::mode::binary);
	CHECK(1 == sem2.count());
	sem2.take();
	CHECK(0 == sem2.count());
	bool r = sem2.take(std::chrono::nanoseconds(1));
	CHECK(false == r);
	sem2.give();
}

TEST_CASE("Create posix message queue", "[posix/os/semaphore]")
{
	auto mq = posix::MessageQueue<uint32_t>(10);
	uint32_t x = 32;

	CHECK(0 == mq.size());
	CHECK(true == mq.empty());
	CHECK(false == mq.full());

	mq.push(x);
	CHECK(1 == mq.size());

	auto y = mq.pop();
	CHECK(x == y);
	CHECK(0 == mq.size());

	y = mq.pop(std::chrono::milliseconds(1));
	CHECK(y.has_value() == false);
}

TEST_CASE("Create posix event flags", "[posix/os/event_flags]")
{
	posix::EventFlag e;

	e.set(0xff);
	auto ret = e.get(0xff);
	CHECK(ret == 0xff);

	// Test "Clear on exit"
	e.set(0xff);
	ret = e.get(0xff, embvm::eventflag::option::AND, false);
	CHECK(ret == 0xff);
	ret = e.get(0xff, embvm::eventflag::option::AND, true);
	CHECK(ret == 0xff);

	// We start this test with the flags already cleared - expecting failure
	ret = e.get(0xff, embvm::eventflag::option::AND, false, std::chrono::milliseconds(1));
	CHECK(ret == 0);
}
