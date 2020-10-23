#ifndef LOG_LIB_DEFS_HPP_
#define LOG_LIB_DEFS_HPP_

#include "_log_common_defs.h"
#include <gsl-lite/gsl-lite.hpp>
#include <string>

namespace embvm
{
/// @addtogroup LoggingSubsystem
/// @{

/// Log definitions
namespace logger
{
/// C++ log level definitions
enum level
{
	off = LOG_LEVEL_OFF,
	critical = LOG_LEVEL_CRITICAL,
	error = LOG_LEVEL_ERROR,
	warn = LOG_LEVEL_WARN,
	info = LOG_LEVEL_INFO,
	debug = LOG_LEVEL_DEBUG,
	verbose = LOG_LEVEL_VERBOSE,
};

/// Global log level limit
inline constexpr logger::level LOG_LEVEL_LIMIT = static_cast<logger::level>(LOG_LEVEL);

/// Declare a global array of log level names
inline constexpr std::string_view level_string_names[] LOG_LEVEL_NAMES;

// Declare a global array of log level short names
inline constexpr const char* level_short_names[] LOG_LEVEL_SHORT_NAMES;

/// Get the name of a log level
/// @param l The level to get the name for.
/// @returns the full name of the log level as a std::string_view.
inline constexpr std::string_view to_str(level l) noexcept
{
	return gsl_lite::at(level_string_names, l);
}

/// Get the name of a log level as a C-string
/// @param l The level to get the name for.
/// @returns the full name of the log level as a C-string.
inline constexpr const char* to_c_str(level l) noexcept
{
	return gsl_lite::at(level_string_names, l).data();
}

/// Get the short name of a log level as a C-string
/// @param l The level to get the short name for.
/// @returns the short name of the log level as a C-string.
inline constexpr const char* to_short_c_str(level l) noexcept
{
	return gsl_lite::at(level_short_names, l);
}

} // namespace logger

/// @}

} // namespace embvm

#endif // LOG_LIB_DEFS_HPP_
