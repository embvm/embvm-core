#include "template_processor_arch.hpp"
// Possiblly include:
// #include <processor_includes.hpp> // Processor-specific include that contains headers/defines

void TemplateProcessorArch::interruptsEnable_() noexcept {}

void TemplateProcessorArch::interruptsDisable_() noexcept {}

void TemplateProcessorArch::memoryBarrier_() noexcept {}

void TemplateProcessorArch::instructionBarrier_() noexcept {}

void TemplateProcessorArch::systemReset() noexcept {}

void TemplateProcessorArch::sysTickConfig(uint32_t ticks) noexcept {}
