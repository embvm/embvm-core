#ifndef TEMPLATE_PROCESSOR_HPP_
#define TEMPLATE_PROCESSOR_HPP_

#include <processor/virtual_processor.hpp>

/** TODO For new processors:
 *
 * - Rename the class in this file and the .cpp file
 * - Rename TEMPLATE_PROCESSOR_HPP_ to something else
 */

class TemplateProcessor : public embvm::VirtualProcessorBase<TemplateProcessor>
{
	using ProcessorBase = embvm::VirtualProcessorBase<TemplateProcessor>;

  public:
	/// @brief Default constructor.
	TemplateProcessor() noexcept : ProcessorBase("Template Processor") {}

	/// @brief Default destructor.
	~TemplateProcessor();

#pragma mark - Inherited Functions -

	static void earlyInitHook_() noexcept;

	void init_() noexcept;

	void reset_() noexcept;

#pragma mark - Custom Functions -

  private:
};

#endif // TEMPLATE_PROCESSOR_HPP_
