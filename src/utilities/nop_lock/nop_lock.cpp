#include "nop_lock.hpp"

using namespace embutil;

void nop_lock::lock() noexcept {}

void nop_lock::unlock() noexcept {}
