#ifndef CIRCULAR_BUFFER_LOGGER_
#define CIRCULAR_BUFFER_LOGGER_

#include "logger_base.hpp"
#include <cassert>
#include <driver/system_clock.hpp>
#include <nop_lock/nop_lock.hpp>
#include <optional>
#include <printf.h>
#include <ring_span/ring_span.hpp>

namespace embvm
{
/** Circular log buffer
 *
 * A circular log buffer manages a memoroy buffer as if it were neverending. When the capacity
 * of the buffer is reached, insertions will wrap around to the beginning of the buffer and
 * overwrite old data. This enables seemingly "infinite" memory with a fixed capacity, preferring
 * the newest data be kept.
 *
 * @tparam TBufferSize Defines the size of the circular log buffer.
 * Set to 0 to disable logging completely (for memory constrained systems).
 * @note Size requirement: power-of-2 for optimized queue logic.
 *
 * @tparam TLock the type of lock to use with the LoggerBase. Locking is disabled by default (with
 *	the use of embutil::nop_lock). You can enable locking by declaring this class with a functional
 * 	lock type.
 *	@code
 *	using PlatformLogger =
 *		embvm::PlatformLogger_t<embvm::CircularLogBufferLogger<8 * 1024>, std::mutex>;
 *  @endcode
 *
 * @ingroup LoggingSubsystem
 */
template<size_t TBufferSize = (8 * 1024), typename TLock = embutil::nop_lock>
class CircularLogBufferLogger final : public LoggerBase<TLock>
{
  public:
	/// Default constructor
	CircularLogBufferLogger() : LoggerBase<TLock>() {}

	/** Initialize the circular log buffer with a system clock for timestamp support.
	 *
	 * If a system clock instance is provided to the logger, timestamps will be
	 * appended to each log statement.
	 *
	 * @param clk The system clock instance to use for timestamping.
	 */
	explicit CircularLogBufferLogger(embvm::clk::SystemClock& clk) noexcept : LoggerBase<TLock>(clk)
	{
	}

	/** Initialize the circular log buffer with options
	 *
	 * @param enable If true, log statements will be output to the log buffer. If false,
	 * logging will be disabled and log statements will not be output to the log buffer.
	 * @param l Runtime log filtering level. Levels greater than the target will not be output
	 * to the log buffer.
	 * @param echo If true, log statements will be logged and printed to the console with printf().
	 * If false, log statements will only be added to the log buffer.
	 */
	explicit CircularLogBufferLogger(bool enable, logger::level l = logger::LOG_LEVEL_LIMIT,
									 bool echo = LOG_ECHO_EN_DEFAULT) noexcept
		: LoggerBase<TLock>(enable, l, echo)
	{
	}

	/** Initialize the circular log buffer with options and a system clock for timestamp support.
	 *
	 * If a system clock instance is provided to the logger, timestamps will be
	 * appended to each log statement.
	 *
	 * @param clk The system clock instance to use for timestamping.
	 * @param enable If true, log statements will be output to the log buffer. If false,
	 * logging will be disabled and log statements will not be output to the log buffer.
	 * @param l Runtime log filtering level. Levels greater than the target will not be output
	 * to the log buffer.
	 * @param echo If true, log statements will be logged and printed to the console with printf().
	 * If false, log statements will only be added to the log buffer.
	 */
	explicit CircularLogBufferLogger(embvm::clk::SystemClock& clk, bool enable,
									 logger::level l = logger::LOG_LEVEL_LIMIT,
									 bool echo = LOG_ECHO_EN_DEFAULT) noexcept
		: LoggerBase<TLock>(clk, enable, l, echo)
	{
	}

	/// Default destructor
	~CircularLogBufferLogger() noexcept = default;

	size_t size() const noexcept final
	{
		return log_buffer_.size();
	}

	size_t capacity() const noexcept final
	{
		return log_buffer_.capacity();
	}

	void dump() noexcept final
	{
		for(const auto& t : log_buffer_)
		{
			_putchar(t);
		}
	}

	void clear() noexcept final
	{
		while(!log_buffer_.empty())
		{
			log_buffer_.pop_front();
		}
	}

  protected:
	void log_putc(char c) noexcept final
	{
		log_buffer_.push_back(c);
	}

  private:
	char buffer_[TBufferSize] = {0};
	stdext::ring_span<char> log_buffer_{buffer_, buffer_ + TBufferSize};
};

} // namespace embvm

#endif // CIRCULAR_BUFFER_LOGGER_
