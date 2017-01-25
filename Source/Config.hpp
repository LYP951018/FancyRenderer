#pragma once

#include <cstdint>

#if !defined(__AVX__)
#error "Please specify /arch:AVX"
#endif

static_assert(sizeof(std::intptr_t) == 8, "X64 only!");
