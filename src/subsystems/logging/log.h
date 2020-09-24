#ifndef LOG_LIB_H_
#define LOG_LIB_H_

#include "_log_common_defs.h"

/** C log buffer container
 *
 * This is the definition of a circular log buffer for C programs.
 *
 * @ingroup FrameworkLogging
 */
typedef struct
{
	/// The pointer to the buffer representing the log.
	char* buf;
	/// The insertion index of the next log statement.
	uint32_t head;
	/// Size of the log buffer.
	uint32_t size;
} log_buffer_t;

#endif // LOG_LIB_H_
