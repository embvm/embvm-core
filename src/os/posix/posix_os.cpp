// clang-format off
#include <time.h> // We need time.h before os.hpp for Linux builds because of system header types...
// clang-format on
#include "os.hpp"
#include <time/time.hpp>

using namespace os::posix;

EventFlag::~EventFlag() noexcept {}

Thread::~Thread() noexcept
{
	if(state_ != embvm::thread::state::terminated && state_ != embvm::thread::state::completed)
	{
		terminate();
	}

	join();
}

#ifdef __APPLE__
Semaphore::~Semaphore() noexcept
{
	dispatch_release(handle_);
}
#else
Semaphore::~Semaphore() noexcept
{
	int r = sem_destroy(&handle_);
	assert(r == 0);
}
#endif

Mutex::~Mutex() noexcept
{
	int r = pthread_mutex_destroy(&handle_);
	assert(r == 0);
}

#pragma mark - this_thread implementations -

void embvm::this_thread::sleep_for(const embvm::os_timeout_t& delay) noexcept
{
	auto ts = embutil::durationToTimespec(delay);

	// We don't have errno support in our libc
	while(nanosleep(&ts, &ts) == -1)
		;
}

embvm::thread::handle_t embvm::this_thread::get_handle() noexcept
{
	return reinterpret_cast<embvm::thread::handle_t>(pthread_self());
}

void embvm::this_thread::yield() noexcept
{
	embvm::this_thread::sleep_for(std::chrono::milliseconds(1));
}
