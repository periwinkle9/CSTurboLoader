#include "common.h"
#define DDRAW_STUFF
#include "cs_addresses.h"
#include "hires_timer.h"
#include "patches.h"
#include "turbo/TurboHandler.h"

static TurboHandler turbo;

// Get the buttons actually being pressed by the user
// (The buttons pressed by the turbo controller won't automatically be released on the next frame,
// because button releases only happen when the window receives a WM_KEYUP message,
// i.e. the player physically releases the button.)
static int getActualKeyPresses()
{
	BYTE keyboardState[256];
	if (!GetKeyboardState(keyboardState))
		return 0;

	int realgKey = 0;
	if (keyboardState[VK_ESCAPE] & 0x80)
		realgKey |= KEY_ESCAPE;
	if (keyboardState['W'] & 0x80)
		realgKey |= KEY_MAP;
	if (keyboardState[VK_LEFT] & 0x80)
		realgKey |= KEY_LEFT;
	if (keyboardState[VK_RIGHT] & 0x80)
		realgKey |= KEY_RIGHT;
	if (keyboardState[VK_UP] & 0x80)
		realgKey |= KEY_UP;
	if (keyboardState[VK_DOWN] & 0x80)
		realgKey |= KEY_DOWN;
	if (keyboardState['X'] & 0x80)
		realgKey |= KEY_X;
	if (keyboardState['Z'] & 0x80)
		realgKey |= KEY_Z;
	if (keyboardState['S'] & 0x80)
		realgKey |= KEY_ARMS;
	if (keyboardState['A'] & 0x80)
		realgKey |= KEY_ARMSREV;
	if (keyboardState[VK_SHIFT] & 0x80)
		realgKey |= KEY_SHIFT;
	if (keyboardState[VK_F1] & 0x80)
		realgKey |= KEY_F1;
	if (keyboardState[VK_F2] & 0x80)
		realgKey |= KEY_F2;
	if (keyboardState['Q'] & 0x80)
		realgKey |= KEY_ITEM;
	if (keyboardState[VK_OEM_COMMA] & 0x80)
		realgKey |= KEY_ALT_LEFT;
	if (keyboardState[VK_OEM_PERIOD] & 0x80)
		realgKey |= KEY_ALT_DOWN;
	if (keyboardState[VK_OEM_2] & 0x80)
		realgKey |= KEY_ALT_RIGHT;
	if (keyboardState['L'] & 0x80)
		realgKey |= KEY_L;
	if (keyboardState[VK_OEM_PLUS] & 0x80)
		realgKey |= KEY_PLUS;

	return realgKey;
}

void putTurboMessage()
{
	// Gonna do a similar thing to what ReadyMapName() does
	const char* text = "<Turbo engaged>";
	constexpr int x = 16;
	constexpr int y = 4;
	PutText(x, y + 1, text, RGB(0x11, 0x00, 0x22));
	PutText(x, y, text, RGB(0xFF, 0xFF, 0xFE));
}

// Mostly vanilla, except where indicated
BOOL Flip_SystemTask(HWND hWnd)
{
	static DWORD timePrev;
	static DWORD timeNow;

	while (TRUE)
	{
		if (!SystemTask())
			return FALSE;

		// Use our tick counter instead of GetTickCount()
		timeNow = GetTicks();

		if (timeNow >= timePrev + 20)
			break;

		Sleep(1);
	}

	// Run turbo handler
	gKey = turbo.getNextInput(getActualKeyPresses());
	if (turbo.isTurbo())
		putTurboMessage();

	if (timeNow >= timePrev + 100)
		timePrev = timeNow;
	else
		timePrev += 20;

	static RECT dst_rect;
	GetWindowRect(hWnd, &dst_rect);
	dst_rect.left += client_x;
	dst_rect.top += client_y;
	dst_rect.right = dst_rect.left + scaled_window_width;
	dst_rect.bottom = dst_rect.top + scaled_window_height;

	frontbuffer->Blt(&dst_rect, backbuffer, &backbuffer_rect, DDBLT_WAIT, NULL);

	if (RestoreSurfaces())
	{
		RestoreStripper();
		RestoreMapName();
		RestoreTextScript();
	}

	return TRUE;
}

void applyTurboPatch()
{
	// Replace vanilla Flip_SystemTask with ours
	hookFunction(reinterpret_cast<void*>(0x40B340), reinterpret_cast<void*>(Flip_SystemTask));
}
