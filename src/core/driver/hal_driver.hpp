// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#ifndef HAL_DRIVER_H_
#define HAL_DRIVER_H_

#include <dispatch/interrupt_queue.hpp>

/// @addtogroup FrameworkDriver
/// @{

namespace embvm
{
class HALDriverBase
{
  public:
	void setBottomHalfDispatcher(const embutil::IRQDispatcherFunc_t& dispatcher) noexcept
	{
		dispatcher_ = dispatcher;
	}

	template<typename TCallback>
	void invokeCallback(TCallback& cb) noexcept
	{
		if(cb)
		{
			if(dispatcher_)
			{
				dispatcher_(cb);
			}
			else
			{
				cb();
			}
		}
	}

	template<typename TCallback, typename... TArgs>
	void invokeCallback(TCallback& cb, TArgs&... args) noexcept
	{
		if(cb)
		{
			if(dispatcher_)
			{
				dispatcher_(std::bind(cb, args...));
			}
			else
			{
				cb(args...);
			}
		}
	}

	virtual void enableInterrupts() noexcept = 0;
	virtual void disableInterrupts() noexcept = 0;

  protected:
	HALDriverBase() = default;
	virtual ~HALDriverBase() = default;

	embutil::IRQDispatcherFunc_t dispatcher_{}; // TODO: can we make this a reference somehow?
};

} // namespace embvm

/// @}
// End Group

#endif // HAL_DRIVER_H_
