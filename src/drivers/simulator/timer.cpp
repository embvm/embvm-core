// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#include "timer.hpp"
#include <ratio>

using namespace embdrv;

SimulatorTimer::~SimulatorTimer() noexcept
{
	// This sleep allows for any detached timers to exit before we destroy the mutex
	std::this_thread::sleep_for(std::chrono::milliseconds(1));
	stop();
}

void SimulatorTimer::start_() noexcept
{
	std::unique_lock<std::mutex> lock(mutex_);
	quit_ = false;
	state_ = embvm::timer::state::armed;
	lock.unlock();

	time_base_ = std::chrono::duration_cast<embvm::timer::timer_period_t>(
		std::chrono::steady_clock::now().time_since_epoch());
	timer_thread_ = std::thread(&SimulatorTimer::timer_thread, this);
}

void SimulatorTimer::stop_() noexcept
{
	std::unique_lock<std::mutex> lock(mutex_);
	quit_ = true;
	state_ = embvm::timer::state::stopped;
	lock.unlock();
	cv_.notify_all();

	// Because our simulator does not have a true interrupt context,
	// We need to handle the restart() case when inside the Timer Manager interrupt
	// which would involve the timer thread trying to join itself. In that case, we just detach
	if(timer_thread_.joinable())
	{
		if(std::this_thread::get_id() == timer_thread_.get_id())
		{
			timer_thread_.detach();
		}
		else
		{
			timer_thread_.join();
		}
	}
}

void SimulatorTimer::timer_thread() noexcept
{
	std::unique_lock<std::mutex> lock(mutex_);
	do
	{
		cv_.wait_for(lock, period_);

		if(!quit_)
		{
			state_ = embvm::timer::state::expired;
			lock.unlock();

			invokeCallback(cb_);

			lock.lock();
		}

	} while(!quit_ && config_ == embvm::timer::config::periodic);

	lock.unlock();
}

void SimulatorTimer::enableInterrupts() noexcept {}

void SimulatorTimer::disableInterrupts() noexcept {}
