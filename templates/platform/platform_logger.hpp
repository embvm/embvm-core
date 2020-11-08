#ifndef TEMPLATE_PLATFORM_LOGGER_HPP_
#define TEMPLATE_PLATFORM_LOGGER_HPP_

/** New Platform bring-up checklist:
 * - [ ] Select a logger declaration, or leave this empty for no logging
 * - [ ] Add a lock if needed
 * - [ ] Rename TEMPLATE_PLATFORM_LOGGER_HPP_ to something else
 */

// Include the logger header for your desired strategy
// #include <logging/circular_buffer_logger.hpp>
// Include the proper if you need a lock, for e.g.:
// #include <mutex>

// Keep the name "PlatformLogger" the same but fill in the desired strategy for your logger
// In this example, we set up a 4 kB circular log buffer.
// using PlatformLogger =
//	embvm::PlatformLogger_t<embvm::CircularLogBufferLogger<4 * 1024, std::mutex>>;

#endif // TEMPLATE_PLATFORM_LOGGER_HPP_
