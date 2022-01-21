#include "AutofireAbuseTurbo.h"
#include "../cs_addresses.h"
#include <stdexcept>

AutofireAbuseTurbo::AutofireAbuseTurbo(int in): TurboState(in)
{
	timer = 0;
	mainWeaponNo = gSelectedArms;
	prevInput = in;
	status = OperMode::Normal;
	swapTransition = SwapDirection::None;

	// Figure out which swap direction is the autofire weapon
	int adjLeftIdx = prevWeaponIdx();
	int adjRightIdx = nextWeaponIdx();

	// Prioritize swapping to Machine Gun over Bubbler
	if (gArmsData[adjLeftIdx].code == 4)
		swapDir = SwapDirection::SwapLeft;
	else if (gArmsData[adjRightIdx].code == 4)
		swapDir = SwapDirection::SwapRight;
	else if (gArmsData[adjLeftIdx].code == 7 && gArmsData[adjLeftIdx].level > 1)
		swapDir = SwapDirection::SwapLeft;
	else if (gArmsData[adjRightIdx].code == 7 && gArmsData[adjRightIdx].level > 1)
		swapDir = SwapDirection::SwapRight;
	else
		throw std::logic_error("Autofire abuse turbo has no autofire weapon to swap to???");
}

static int getWeaponCount()
{
	int weaponCount = 0;
	while (weaponCount < 8 && gArmsData[weaponCount].code != 0)
		++weaponCount;
	return weaponCount;
}

int AutofireAbuseTurbo::prevWeaponIdx() const
{
	if (mainWeaponNo > 0)
		return mainWeaponNo - 1;
	else
		return getWeaponCount() - 1;
}
int AutofireAbuseTurbo::nextWeaponIdx() const
{
	if (mainWeaponNo + 1 < getWeaponCount())
		return mainWeaponNo + 1;
	else
		return 0;
}

// The base class getTurboMode() function doesn't account for being in the middle of the autofire abuse inputs,
// so we provide our own instead
auto AutofireAbuseTurbo::getMode() const -> TurboMode
{
	int input = getInput();
	// Disable if player released the turbo button or pressed one of these problematic keys
	if (!(input & KEY_TURBO) || (input & (KEY_ITEM | KEY_MAP | KEY_ESCAPE)))
		return TurboMode::None;

	// If an event started, we should switch to text mashing mode
	// I think this should also cover inventory changes, since those have to be done via TSC events
	if (gTSmode != 0)
		return TurboMode::TextAdvance;

	// If released X, switch to normal autofire mode
	if (!(input & KEY_X))
		return TurboMode::Standard;

	// If the autofire gun was the Bubbler and it leveled down to L1, switch to normal autofire on the main gun
	int autofireGun = (swapDir == SwapDirection::SwapLeft ? prevWeaponIdx() : nextWeaponIdx());
	if (gArmsData[autofireGun].code == 7 && gArmsData[autofireGun].level == 1)
		return TurboMode::Standard;

	// If none of the aboe apply, we should be good to stay on the current mode
	return TurboMode::AutofireAbuse;
}

TurboState* AutofireAbuseTurbo::procStateTransition(int input)
{
	int tmp = input;
	setInput(input);
	
	if (status != OperMode::Normal)
	{
		if (status == OperMode::Stopped)
			return TurboState::procStateTransition(input);
		else if (g_GameFlags & 2) // Continue with weapon swapping as long as we still can
		{
			/*if (status == OperMode::Stopping && timer == 0)
				status = OperMode::Swapping;*/
			return this;
		}
	}

	TurboMode mode = getMode();

	if (mode != TurboMode::AutofireAbuse)
	{
		// Swap back to original weapon
		// ...unless a cutscene activated, in which case there's nothing we can do
		if (g_GameFlags & 2)
		{
			swapTransition = SwapDirection::None;
			status = OperMode::Stopping;
			return this;
		}
		else
			return getState(mode, input);
	}

	// We're still here, but check if the player wants to switch weapons

	int trg = input & ~prevInput;

	if (trg & KEY_ARMS)
	{
		swapTransition = SwapDirection::SwapRight;
		status = OperMode::Stopping;
	}
	else if (trg & KEY_ARMSREV)
	{
		swapTransition = SwapDirection::SwapLeft;
		status = OperMode::Stopping;
	}

	prevInput = tmp;
	return this;
}

void AutofireAbuseTurbo::procInput()
{
	switch (status)
	{
	case OperMode::Normal:
		autofireProc();
		break;
	case OperMode::Stopping:
		if (timer > 0)
			autofireProc();
		else
		{
			if (gSelectedArms != mainWeaponNo)
				throw std::logic_error("Wrong weapon?");
			status = OperMode::Swapping;
			// Add a 1f delay for safety, I guess?
			int input = getInput();
			input &= ~(KEY_ARMS | KEY_ARMSREV | KEY_X);
			setInput(input);
		}
		break;
	case OperMode::Swapping:
		weaponSwapProc();
		break;
	case OperMode::Stopped:
		throw std::logic_error("Stopped autofire abuse turbo but still in autofire abuse turbo mode?");
	}
}

void AutofireAbuseTurbo::autofireProc()
{
	int input = getInput();
	input &= ~(KEY_ARMS | KEY_ARMSREV); // Prevent shenanigans

	switch (timer++)
	{
	case 0:
		// Sanity check
		if (gSelectedArms != mainWeaponNo)
			throw std::logic_error("Autofire abuse turbo - weapon desync!");

		// Wait until you can shoot
		if (soft_rensha <= 1)
		{
			input |= KEY_X;
			if (swapDir == SwapDirection::SwapLeft)
				input |= KEY_ARMSREV;
			else if (swapDir == SwapDirection::SwapRight)
				input |= KEY_ARMS;
		}
		else
		{
			input &= ~KEY_X;
			timer = 0;
		}
		break;
	case 1:
		// Failsafe in case we didn't actually swap weapons last frame
		if (gSelectedArms == mainWeaponNo)
		{
			input &= ~KEY_X;
			timer = 0;
		}
		else
			input |= KEY_X;
		break;
	case 2:
		input &= ~KEY_X;
		break;
	case 3:
		input &= ~KEY_X;
		if (swapDir == SwapDirection::SwapLeft)
			input |= KEY_ARMS;
		else if (swapDir == SwapDirection::SwapRight)
			input |= KEY_ARMSREV;
		// Fallthrough to reset timer
	default:
		timer = 0;
	}

	setInput(input);
}

void AutofireAbuseTurbo::weaponSwapProc()
{
	int input = getInput();
	input &= ~KEY_X; // Stop shooting

	int targetWeapon = 0;
	int swapButton = 0;
	switch (swapTransition)
	{
	case SwapDirection::None:
		targetWeapon = mainWeaponNo;
		break;
	case SwapDirection::SwapLeft:
		targetWeapon = prevWeaponIdx();
		swapButton = KEY_ARMSREV;
		break;
	case SwapDirection::SwapRight:
		targetWeapon = nextWeaponIdx();
		swapButton = KEY_ARMS;
	}

	if (gSelectedArms == targetWeapon)
	{
		//input &= ~(KEY_ARMS | KEY_ARMSREV);
		input |= swapButton; // Hold the button in case the player is also still holding it
		status = OperMode::Stopped; // We're done
	}
	else
	{
		// Even timer: push the button
		// Odd timer: release it
		if (swapTransition == SwapDirection::SwapLeft)
		{
			if (timer++ % 2 == 0)
				input |= KEY_ARMSREV;
			else
				input &= ~KEY_ARMSREV;
		}
		else if (swapTransition == SwapDirection::SwapRight)
		{
			if (timer++ % 2 == 0)
				input |= KEY_ARMS;
			else
				input &= ~KEY_ARMS;
		}
		else
			throw std::logic_error("Wrong weapon selected in weapon swap mode?!");
	}

	setInput(input);
}

#ifdef _DEBUG
#include <string>

static std::string mode; // Static allocation to prevent it from going out of scope when .c_str() is returned.
                         // Yes, I know this is super lazy but it's just for debugging purposes so I don't care :p
const char* AutofireAbuseTurbo::name() const
{
	switch (status)
	{
	case OperMode::Normal:
		return "AutofireAbuseTurbo (autofire mode)";
	case OperMode::Stopping:
		return (mode = "AutofireAbuseTurbo (stopping), timer = " + std::to_string(timer)).c_str();
	case OperMode::Swapping:
		return (mode = "AutofireAbuseTurbo (weapon swap mode), timer = " + std::to_string(timer)).c_str();
	case OperMode::Stopped:
		return "AutofireAbuseTurbo (stopped)";
	default:
		return "????????????????????"; // ???????????? (evading compiler warning)
	}
}
#endif
