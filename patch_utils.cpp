#include "patches.h"
#include "common.h"

void patchBytes(void* addr, const std::uint8_t bytes[], unsigned size)
{
	WriteProcessMemory(GetCurrentProcess(), addr, static_cast<const void*>(bytes), size, NULL);
}

void hookFunction(void* addr, void* func)
{
#ifdef _M_IX86
	std::uint8_t bytes[] = {0x68, 0, 0, 0, 0, 0xC3};
	*reinterpret_cast<std::uint32_t*>(&bytes[1]) = reinterpret_cast<std::uint32_t>(func);

	patchBytes(addr, bytes, sizeof bytes);
#else
#error Please compile for x86!
#endif
}
