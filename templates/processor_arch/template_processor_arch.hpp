#ifndef TEMPLATE_ARCHITECTURE_HPP_
#define TEMPLATE_ARCHITECTURE_HPP_

#include <processor/virtual_processor_arch.hpp>

/** TODO for new arch:
 *
 * - Rename class
 * - Rename TEMPLATE_ARCHITECTURE_HPP_ to something else
 * - Rename class in template_processor_arch.cpp
 */

class TemplateProcessorArch : public embvm::VirtualProcessorArchBase<TemplateProcessorArch>
{
  public:
#pragma mark - Specific Arch Functions -

#pragma mark - Common Arch Functions -
	static void interruptsEnable_() noexcept;

	static void interruptsDisable_() noexcept;

	static void memoryBarrier_() noexcept;

	static void instructionBarrier_() noexcept;
};

#endif // TEMPLATE_ARCHITECTURE_HPP_
