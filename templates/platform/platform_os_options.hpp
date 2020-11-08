#ifndef TEMPLATE_PLATFORM_OS_OPTIONS_HPP_
#define TEMPLATE_PLATFORM_OS_OPTIONS_HPP_

// This file can be left blank if you are not using an OS.

/** Checklist for new platforms:
 * - [ ] Rename TEMPLATE_PLATFORM_OS_OPTIONS_HPP_ to something different
 * - [ ] Update pool size definitions for your platform
 */

#define OS_THREAD_POOL_SIZE 4
#define OS_MUTEX_POOL_SIZE 4
#define OS_SEMAPHORE_POOL_SIZE 4
#define OS_EVENT_FLAG_POOL_SIZE 4

#endif // TEMPLATE_PLATFORM_OS_OPTIONS_HPP_
