#pragma once

// MG-adjacent autofire abuse turbo

#include "TurboState.h"

class AutofireAbuseTurbo : public TurboState
{
	int timer;
	int mainWeaponNo;
	enum class OperMode { Normal, Stopping, Swapping, Stopped } status;
	enum class SwapDirection { None, SwapLeft, SwapRight };
	SwapDirection swapDir; // Direction to swap from main weapon to autofire weapon
	SwapDirection swapTransition; // Direction to swap if player stops turboing or changes weapons
	int prevInput;

	TurboMode getMode() const;
	void autofireProc();
	void weaponSwapProc();
	int prevWeaponIdx() const;
	int nextWeaponIdx() const;
public:
	AutofireAbuseTurbo(int in = 0);
	TurboState* procStateTransition(int input) override;
	void procInput() override;

#ifdef _DEBUG
	const char* name() const override;
#endif
};
