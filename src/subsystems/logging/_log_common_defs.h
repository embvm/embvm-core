// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#ifndef LOG_LIB_COMMON_DEFS_H_
#define LOG_LIB_COMMON_DEFS_H_

/// @addtogroup LoggingSubsystem
/// @{

/// @name Log Level Definitions
/// @{

/// Logging is disabled
#define LOG_LEVEL_OFF 0
/// Indicates the system is unusable, or an error that is unrecoverable
#define LOG_LEVEL_CRITICAL 1
/// Indicates an error condition
#define LOG_LEVEL_ERROR 2
/// Indicates a warning condition
#define LOG_LEVEL_WARN 3
/// Informational messages
#define LOG_LEVEL_INFO 4
/// Debug-level messages
#define LOG_LEVEL_DEBUG 5
/// Extra super verbose messages
#define LOG_LEVEL_VERBOSE 6
/// The maximum log level that can be set
#define LOG_LEVEL_MAX LOG_LEVEL_VERBOSE
/// The number of possible log levels
#define LOG_LEVEL_COUNT (LOG_LEVEL_MAX + 1)
/// @}
// End log level definitions

/// @name Log Level Prefixes
/// @{
#define LOG_LEVEL_CRITICAL_PREFIX "!"
#define LOG_LEVEL_ERROR_PREFIX "E"
#define LOG_LEVEL_WARNING_PREFIX "W"
#define LOG_LEVEL_INFO_PREFIX "I"
#define LOG_LEVEL_DEBUG_PREFIX "D"
#define LOG_LEVEL_VERBOSE_PREFIX "V"
#define LOG_LEVEL_INTERRUPT_PREFIX "I"
/// @}

#ifndef LOG_LEVEL
/** Default maximum log level.
 *
 * This is the maximum log level that will be compiled in.
 * To set a custom log level, define the LOG_LEVEL before including this header
 * (e.g., as a compiler definition)
 */
#define LOG_LEVEL LOG_LEVEL_DEBUG
#endif

#ifndef LOG_EN_DEFAULT
/// Whether the logging module is enabled automatically on boot.
#define LOG_EN_DEFAULT true
#endif

#ifndef LOG_ECHO_EN_DEFAULT
/// Indicates that log statements should be echoed to the console
/// If true, log statements will be echoed.
/// If false, log statements will only go to the log.
#define LOG_ECHO_EN_DEFAULT false
#endif

#ifndef LOG_LEVEL_NAMES
/// Users can override these default names with a compiler definition
#define LOG_LEVEL_NAMES                                                   \
	{                                                                     \
		"off", "critical", "error", "warning", "info", "debug", "verbose" \
	}
#endif

#ifndef LOG_LEVEL_SHORT_NAMES
/// Users can override these default short names with a compiler definition
#define LOG_LEVEL_SHORT_NAMES                                                             \
	{                                                                                     \
		"O", LOG_LEVEL_CRITICAL_PREFIX, LOG_LEVEL_ERROR_PREFIX, LOG_LEVEL_WARNING_PREFIX, \
			LOG_LEVEL_INFO_PREFIX, LOG_LEVEL_DEBUG_PREFIX, LOG_LEVEL_VERBOSE_PREFIX       \
	}
#endif

/// @}

#endif // LOG_LIB_COMMON_DEFS_H_
