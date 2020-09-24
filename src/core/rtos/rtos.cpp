#include "rtos.hpp"

using namespace embvm;

VirtualThread::~VirtualThread() noexcept = default;

VirtualSemaphore::~VirtualSemaphore() noexcept = default;

VirtualMutex::~VirtualMutex() noexcept = default;

VirtualEventFlag::~VirtualEventFlag() noexcept = default;

VirtualConditionVariable::~VirtualConditionVariable() noexcept = default;
