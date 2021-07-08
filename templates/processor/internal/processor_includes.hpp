#ifndef TEMPLATE_PROCESSOR_INCLUDES_HPP_
#define TEMPLATE_PROCESSOR_INCLUDES_HPP_

/* Purpose:
 * This processor header can be used by internal-facing files to include all relevant processor
 * headers supplied by a processor vendor, SDK, etc. The idea is that the headers in this file
 * should not be exposed to the rest of the framework. Keep them contained and use this header
 * within processor .cpp files.
 */

/** Checklist for new processor bring up:
 *
 * - [ ] Rename TEMPLATE_PROCESSOR_INCLUDES_HPP_ to something else
 * - [ ] Add any processor-specific includes here that may be needed for internal modules (drivers,
 * etc.)
 */

#endif // TEMPLATE_PROCESSOR_INCLUDES_HPP_
