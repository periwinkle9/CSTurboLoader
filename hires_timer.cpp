// Timer implementation largely based off of SDL2's SDL_GetTicks() implementation
// Any system running Windows XP or later should in theory be able to use
// QueryPerformanceCounter(), but just for the heck of it let's keep a fallback
// in case somebody wants to try running this on an ancient PC. :P

// No WIN32_LEAN_AND_MEAN here, that disables the declaration of timeGetTime()
//typedef struct IUnknown IUnknown; // Workaround for a compiler error with the Windows XP toolset/SDK
#include <Windows.h>

namespace {
	bool inited = false;
	bool hasHiResTimer;
	LARGE_INTEGER startTicks;
	LARGE_INTEGER tickFreq;

	void TimerInit()
	{
		if (QueryPerformanceFrequency(&tickFreq))
		{
			hasHiResTimer = true;
			QueryPerformanceCounter(&startTicks);
		}
		else
			hasHiResTimer = false;
		inited = true;
	}
}

DWORD GetTicks()
{
	if (!inited)
		TimerInit();
	if (hasHiResTimer)
	{
		LARGE_INTEGER time;
		QueryPerformanceCounter(&time);

		time.QuadPart -= startTicks.QuadPart;
		time.QuadPart = (time.QuadPart * 1000) / tickFreq.QuadPart;

		return static_cast<DWORD>(time.QuadPart);
	}
	else
		return timeGetTime();
}
