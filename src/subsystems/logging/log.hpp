#ifndef LOG_MACROS_HPP_
#define LOG_MACROS_HPP_

#include <platform_logger.hpp>

/// @addtogroup LoggingSubsystem
/// @{

/** @name Logging Macros
 *
 * The log macros can be overridden by defining them in your platform_logger.hpp file
 *
 * Warning is the default log level if one is not supplied
 *
 * For more information see @ref docs/development/ExtendingTheFramework/customizing_log_macros.md
 *
 * @{
 */

// Supply a default log level
#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_LEVEL_WARN
#endif

#if LOG_LEVEL >= LOG_LEVEL_CRITICAL
#ifndef logcritical
#define logcritical(...) PlatformLogger::inst().log(embvm::logger::level::critical, __VA_ARGS__)
#endif
#else
#define logcritical(...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_ERROR
#ifndef logerror
#define logerror(...) PlatformLogger::inst().log(embvm::logger::level::error, __VA_ARGS__)
#endif
#else
#define logcerror(...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_WARN
#ifndef logwarn
#define logwarn(...) PlatformLogger::inst().log(embvm::logger::level::warn, __VA_ARGS__)
#endif
#else
#define logwarn(...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_INFO
#ifndef loginfo
#define loginfo(...) PlatformLogger::inst().log(embvm::logger::level::info, __VA_ARGS__)
#endif
#else
#define loginfo(...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_DEBUG
#ifndef logdebug
#define logdebug(...) PlatformLogger::inst().log(embvm::logger::level::debug, __VA_ARGS__)
#endif
#else
#define logdebug(...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_VERBOSE
#ifndef logverbose
#define logverbose(...) PlatformLogger::inst().log(embvm::logger::level::verbose, __VA_ARGS__)
#endif
#else
#define logverbose(...)
#endif

/// @}
/// @}

#endif // LOG_MACROS_HPP_
