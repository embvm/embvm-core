#include "template_processor.hpp"
#include <processor_architecture.hpp>
#include <processor_includes.hpp>

#pragma mark - Definitions -

#pragma mark - Helpers -

#pragma mark - Interface Functions -

TemplateProcessor::~TemplateProcessor() {}

void TemplateProcessor::init_() noexcept
{
	// What needs to be happen to configure and initialize the processor?
	// This should be common to all uses of the processor across all HW platforms
}

void TemplateProcessor::reset_() noexcept
{
	// Example for ARM: defer to the arch
	// ProcessorArch::systemReset();
}

void TemplateProcessor::earlyInitHook_() noexcept
{
	// Should anything happen for this processor before we initialize the
	// system?
	// For example, configuring the MMU
}
