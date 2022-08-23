// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#ifndef LOGGER_BASE_HPP_
#define LOGGER_BASE_HPP_

#include "log_defs.hpp"
#include <cassert>
#include <driver/system_clock.hpp>
#include <optional>
#include <printf.h>
#include <ring_span/ring_span.hpp>

// TODO: log from an interrupt

/**
 * TODO: consider additional fields
 * The format of your status messages can make a big difference in later analysis. Break the
 * messages into fields, so that certain information always appears in a particular column. One
 * column should be a system time stamp that’s accurate enough to deal with timing problems. There
 * are many other candidates for standard columns: the module or source file that outputs the
 * message; a general code for message type, such as “info,” “error,” or “really nasty error”; the
 * initials of the engineer who wrote the output message, to help track down who worked on what and
 * why they can’t spell; and run-time data such as commands, status codes, and expected versus
 * actual values to give you the real details you’ll need later. Finally, by using consistent
 * formats and keywords, you can filter the debug logs afterward to help you focus on the stuff you
 * need to see.
 */

namespace embvm
{
/** Base class for logger implementations.
 *
 * This class provides the interface for loggers, as well as implements basic operations.
 * To create a logger, you can derive from this class and fill in the missing operations.
 *
 * For an example logger implementation, see the CircularLogBufferLogger in
 * circular_buffer_logger.hpp.
 *
 * # Using a Logger
 *
 * The primary method for interacting with a logger is through the log() interface. Macros
 * are provided for convenient logging at different levels (loginfo(), logdebug(), logerror(), etc).
 *
 * The full contents of the log buffer can be dumped to the console using the dump() API.
 *
 * # Defining a Logger
 *
 * Do declare your own logger type, inherit from this base class:
 *
 * @code
 * class CircularLogBufferLogger final : public LoggerBase
 * @endcode
 *
 * This derived class is used to declare the actual log buffer storage and handle the log buffer
 * implementation.
 *
 * Derived classes must implement the following functions to control specific log behavior:
 *
 * - size()
 * - capacity()
 * - dump()
 * - log_putc()
 * - clear()
 *
 * @tparam TLock the type of lock to use with the LoggerBase. Locking can be disabled by using
 * 	the embutil::nop_lock type.
 *
 * @ingroup LoggingSubsystem
 */
template<typename TLock>
class LoggerBase
{
  public:
	/** Get the current log buffer size
	 *
	 * Derived classes must implement this function.
	 *
	 * @returns The current size of the log buffer, in bytes.
	 */
	[[nodiscard]] virtual size_t size() const noexcept = 0;

	/** Get the log buffer capacity
	 *
	 * Derived classes must implement this function.
	 *
	 * @returns The total capacity of the log buffer, in bytes.
	 */
	[[nodiscard]] virtual size_t capacity() const noexcept = 0;

	/** Check if the log is enabled.
	 *
	 * @returns true if log output is enabled, false if it is disabled.
	 */
	[[nodiscard]] bool enabled() const noexcept
	{
		return enabled_;
	}

	/** Enable/disable logging
	 *
	 * @param en Enable switch. If true, logging is enabled. If false, logging is disabled.
	 * @returns true if log output is enabled, false if it is disabled.
	 */
	bool enabled(bool en) noexcept
	{
		enabled_ = en;
		return enabled_;
	}

	/** Check the echo setting
	 *
	 * @returns true if echo to console is enabled, false if disabled.
	 */
	[[nodiscard]] bool echo() const noexcept
	{
		return echo_;
	}

	/** Enable/disable echo to console.
	 *
	 * @param en Echo switch. If true, log statements will also be echoed to the console through
	 * printf(). If false, log statements will only go to the log buffer.
	 * @returns true if echo to console is enabled, false if disabled.
	 */
	bool echo(bool en) noexcept
	{
		echo_ = en;
		return echo_;
	}

	/** Get the maximum log level (filtering)
	 *
	 * @returns the current log level maximum.
	 */
	[[nodiscard]] logger::level level() const noexcept
	{
		return level_;
	}

	/** Set the maximum log level (filtering)
	 *
	 * @param l The maximum log level. Levels grater than `l` will not be added to the log buffer.
	 * @returns the current log level maximum.
	 */
	logger::level level(logger::level l) noexcept
	{
		if(l <= logger::LOG_LEVEL_LIMIT)
		{
			level_ = l;
		}

		return level_;
	}

	/** Add data to the log buffer
	 *
	 * @note Implementor's note: We would convert this to a variadic template funciton, but we can't
	 *   take advantage ofprintf format string checks and get extra warnings.
	 *   See commit 65991eddd9e2ba4a9cd9276d86a0ba24d7bf59aa for the details, and reversion in the
	 *   future in case this limitation is addressed.
	 *
	 * @param[in] l The log level associated with this statement.
	 * @param[in] fmt The log format string.
	 * @param[in] args The variadic arguments that are associated with the format string.
	 */
	void log(logger::level l, const char* fmt, ...) noexcept __attribute__((format(printf, 3, 4)))
	{
		if(enabled_ && l <= level_)
		{
			va_list argptr;
			va_start(argptr, fmt);

			mutex_.lock();
			if(system_clock_)
			{
				fctprintf(&LoggerBase::log_putc_bounce, this, "[%llu] ", system_clock_->ticks());
			}

			// Add our prefix
			fctprintf(&LoggerBase::log_putc_bounce, this, "<%s> ", logger::to_short_c_str(l));

			// Send the primary log statement
			vfctprintf(&LoggerBase::log_putc_bounce, this, fmt, argptr);
			mutex_.unlock();

			if(echo_)
			{
				if(system_clock_)
				{
					printf("[%llu] ", system_clock_->ticks());
				}

				printf("<%s> ", logger::to_short_c_str(l));

				vprintf(fmt, argptr);
			}

			va_end(argptr);
		}
	}

	/** Set the system clock
	 *
	 * @param clk The system clock instance to use for timestamping.
	 */
	void setClock(embvm::clk::SystemClock& clk) noexcept
	{
		system_clock_ = &clk;
	}

	/** Print the contents of the log buffer in the console.
	 *
	 * When called, the contents of the log buffer will be echoed to the console through printf.
	 * The entire log buffer will be displayed.
	 *
	 * Derived classes must implement this function.
	 */
	virtual void dump() noexcept = 0;

	/** Clear the contents of the log buffer.
	 *
	 * Reset the log buffer to an empty state.
	 *
	 * @post The log buffer will be empty.
	 */
	virtual void clear() noexcept = 0;

  protected:
	/// Default constructor
	LoggerBase() = default;

	/** Initialize the logger with a system clock for timestamp support.
	 *
	 * If a system clock instance is provided to the logger, timestamps will be
	 * appended to each log statement.
	 *
	 * @param clk The system clock instance to use for timestamping.
	 */
	explicit LoggerBase(embvm::clk::SystemClock& clk) noexcept : system_clock_(&clk) {}

	/** Initialize the logger with options
	 *
	 * @param enable If true, log statements will be output to the log buffer. If false,
	 * logging will be disabled and log statements will not be output to the log buffer.
	 * @param l Runtime log filtering level. Levels greater than the target will not be output
	 * to the log buffer.
	 * @param echo If true, log statements will be logged and printed to the console with printf().
	 * If false, log statements will only be added to the log buffer.
	 */
	explicit LoggerBase(bool enable, logger::level l = logger::LOG_LEVEL_LIMIT,
						bool echo = LOG_ECHO_EN_DEFAULT) noexcept
		: enabled_(enable), level_(l), echo_(echo)
	{
	}

	/** Initialize the logger with options and a system clock for timestamp support.
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
	explicit LoggerBase(embvm::clk::SystemClock& clk, bool enable,
						logger::level l = logger::LOG_LEVEL_LIMIT, bool echo = LOG_ECHO_EN_DEFAULT)
		: enabled_(enable), level_(l), echo_(echo), system_clock_(&clk)
	{
	}

	/// Default destructor
	virtual ~LoggerBase() = default;

	/** Log buffer putc function
	 *
	 * This function adds a character to the underlying log buffer.
	 *
	 * This function is used with the fctprintf() interface to output to the log buffer.
	 * This enables the framework to reuse the same print formatting for both logging and printf().
	 *
	 * Derived classes must implement this function.
	 *
	 * @param c The character to insert into the log buffer.
	 */
	virtual void log_putc(char c) = 0;

	/** putc bounce function
	 *
	 * This is a bounce function which registers with the C printf API. We use the private parameter
	 * to store the `this` pointer so we can get back to our logger's putc instance.
	 *
	 * @param c The character to log.
	 * @param this_ptr The this pointer of the logger instance. Used to invoke log_putc() on the
	 * correct instance.
	 */
	static void log_putc_bounce(char c, void* this_ptr)
	{
		reinterpret_cast<LoggerBase*>(this_ptr)->log_putc(c);
	}

  private:
	/// Indicates whether logging is currently enabled
	bool enabled_ = LOG_EN_DEFAULT;

	/// The current log level.
	/// Levels greater than the current setting will be filtered out.
	logger::level level_ = logger::LOG_LEVEL_LIMIT;

	/// Console echoing.
	/// If true, log statements will be printed to the console through printf().
	bool echo_ = LOG_ECHO_EN_DEFAULT;

	/// Mutex which protects the log buffer
	TLock mutex_{};

	/// The system clock instance, used for timestamping.
	embvm::clk::SystemClock* system_clock_ = nullptr;
};

/** Declare a static platform logger instance.
 *
 * This class is used to declare a static platform logger instance.
 *
 * Define a platform logger which is templated on the log type:
 *
 * @code
 * using PlatformLogger = PlatformLogger_t<CircularLogBufferLogger<8 * 1024>>;
 * @endcode
 *
 * This `PlatformLogger` type will then work with the logging macros (loginfo(), etc.).
 *
 * The instance can also be grabbed manually:
 *
 * @code
 * PlatformLogger::inst().setClock(clk.value());
 * PlatformLogger::inst().dump();
 * @endcode
 *
 * @ingroup LoggingSubsystem
 */
template<class TLogger>
class PlatformLogger_t
{
  public:
	PlatformLogger_t() = default;
	~PlatformLogger_t() = default;

	static TLogger& inst()
	{
		static TLogger logger_;
		return logger_;
	}
};

} // namespace embvm

#endif // LOGGER_BASE_HPP_
