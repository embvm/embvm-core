#ifndef TEMPLATE_PLATFORM_LOGGER_HPP_
#define TEMPLATE_PLATFORM_LOGGER_HPP_

/** TODO for new platforms:
 * Select a logger declaration, or leave this empty for no logging
 * Add a lock if needed
 * Rename TEMPLATE_PLATFORM_LOGGER_HPP_ to something else
 */

// Include the logger header:
// #include <logging/circular_buffer_logger.hpp>
// Include the proper if you need a lock, for e.g.:
// #include <mutex>

// Keep the name "PlatformLogger" the same but fill in the desired strategy for your logger
// using PlatformLogger =
//	embvm::PlatformLogger_t<embvm::CircularLogBufferLogger<4 * 1024, std::mutex>>;

#endif // TEMPLATE_PLATFORM_LOGGER_HPP_
