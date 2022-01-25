#pragma once

// Don't pollute the global namespace with this stuff if you don't need it
#ifdef DDRAW_STUFF
#include <ddraw.h>
#endif

namespace {

// The key inputs are the only thing we actually modify; everything else here can be const
int& gKey = *reinterpret_cast<int*>(0x49E210);

struct ARMSDATA
{
	int code;
	int level;
	int exp;
	int max_num;
	int num;
};
const ARMSDATA(&gArmsData)[8] = *reinterpret_cast<ARMSDATA(*)[8]>(0x499BC8);
const int& gSelectedArms = *reinterpret_cast<int*>(0x499C68);

const int& g_GameFlags = *reinterpret_cast<int*>(0x49E1E8);

const int& client_x = *reinterpret_cast<int*>(0x49CDAC);
const int& client_y = *reinterpret_cast<int*>(0x49CDA8);
const int& scaled_window_width = *reinterpret_cast<int*>(0x49D374);
const int& scaled_window_height = *reinterpret_cast<int*>(0x49D378);

const int& gKeyTrg = *reinterpret_cast<int*>(0x49E214);
//const int& key_old = *reinterpret_cast<int*>(0x49E218);

const int& soft_rensha = *reinterpret_cast<int*>(0x4A5564);

const signed char& gTSmode = *reinterpret_cast<signed char*>(0x4A5ADC);
const int& gTSwait = *reinterpret_cast<int*>(0x4A5AFC);

auto SystemTask = reinterpret_cast<int(*)(void)>(0x413570);
auto RestoreSurfaces = reinterpret_cast<int(*)(void)>(0x40CB60);
auto RestoreStripper = reinterpret_cast<void(*)(void)>(0x40D240);
auto RestoreMapName = reinterpret_cast<void(*)(void)>(0x414330);
auto RestoreTextScript = reinterpret_cast<void(*)(void)>(0x425790);

auto GetTrg = reinterpret_cast<void(*)(void)>(0x4122E0);

auto IsNpCharCode = reinterpret_cast<int(*)(int)>(0x470490);

auto CountArmsBullet = reinterpret_cast<int(*)(int)>(0x403C40);

auto PutFramePerSecound = reinterpret_cast<void(*)(void)>(0x412370);

enum KeyBind
{
	KEY_LEFT = 0x00000001,
	KEY_RIGHT = 0x00000002,
	KEY_UP = 0x00000004,
	KEY_DOWN = 0x00000008,
	KEY_MAP = 0x00000010,
	KEY_X = 0x00000020,
	KEY_Z = 0x00000040,
	KEY_ARMS = 0x00000080,
	KEY_ARMSREV = 0x00000100,
	KEY_SHIFT = 0x00000200,
	KEY_F1 = 0x00000400,
	KEY_F2 = 0x00000800,
	KEY_ITEM = 0x00001000,
	KEY_ESCAPE = 0x00008000,
	KEY_ALT_LEFT = 0x00010000,
	KEY_ALT_DOWN = 0x00020000,
	KEY_ALT_RIGHT = 0x00040000,
	KEY_ALT_UP = 0x00180000,
	KEY_L = 0x00080000,
	KEY_PLUS = 0x00100000
};

constexpr int KEY_TURBO = KEY_SHIFT;

#ifdef DDRAW_STUFF
LPDIRECTDRAWSURFACE& frontbuffer = *reinterpret_cast<LPDIRECTDRAWSURFACE*>(0x49D380);
LPDIRECTDRAWSURFACE& backbuffer = *reinterpret_cast<LPDIRECTDRAWSURFACE*>(0x49D384);
RECT& backbuffer_rect = *reinterpret_cast<RECT*>(0x49CDB0);
RECT& grcFull = *reinterpret_cast<RECT*>(0x48F92C);

auto CortBox = reinterpret_cast<void(*)(const RECT*, unsigned long)>(0x40C9E0);
auto PutBitmap3 = reinterpret_cast<void(*)(const RECT*, int, int, const RECT*, int)>(0x40C3C0);
#endif

}
