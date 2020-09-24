#ifndef HAL_DRIVER_H__
#define HAL_DRIVER_H__

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

  protected:
	embutil::IRQDispatcherFunc_t dispatcher_; // TODO: can we make this a reference somehow?
};

} // namespace embvm

/// @}
// End Group

#endif // HAL_DRIVER_H__
