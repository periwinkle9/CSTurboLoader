#pragma once

#include <cstdint>

void applyTurboPatch();

// Utility functions
void patchBytes(void* addr, const std::uint8_t bytes[], unsigned size);
void hookFunction(void* addr, void* func);
