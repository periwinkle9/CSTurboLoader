#include "TurboState.h"
#include "../cs_addresses.h"
#include "NoTurbo.h"
#include "AutofireTurbo.h"
#include "TextAdvanceTurbo.h"
#include "MachineGunTurbo.h"
#include "AutofireAbuseTurbo.h"
#include "InfiniteSpurTurbo.h"

auto TurboState::getTurboMode(int input) -> TurboMode
{
	// Dealing with opening the inventory/map/escape menu while turbo-ing is a pain,
	// so let's just not deal with it at all
	if (!(input & KEY_TURBO) || (input & (KEY_ITEM | KEY_MAP | KEY_ESCAPE)))
		return TurboMode::None;

	// If in an event, use text mashing mode
	if (gTSmode != 0 && !(g_GameFlags & 2)) // Unless the player still has control (e.g. Ballos phase transitions)
		return TurboMode::TextAdvance;

	// If no weapons, no point in turbo-ing
	const ARMSDATA& curWeapon = gArmsData[gSelectedArms];
	if (curWeapon.code == 0)
		return TurboMode::None;

	// If holding X, detect smart turbo mode
	if (input & KEY_X)
	{
		auto isAutofireGun = [](const ARMSDATA& arms)
		{
			// Machine Gun or L2/3 Bubbler
			return arms.code == 4 || (arms.code == 7 && arms.level > 1);
		};
		
		if (IsNpCharCode(320)) // Check for Curly
			return TurboMode::AutofireCurly;
		else if (isAutofireGun(curWeapon))
			return TurboMode::AutofireGun;
		else if (curWeapon.code == 13 && curWeapon.level > 1) // L2/3 Spur
			return TurboMode::InfiniteSpur;
		else if (gArmsData[1].code != 0) // Need at least 2 weapons to do this
		{
			// Check adjacent weapons for MG or Bubbler
			int weaponCount = 0;
			for (; weaponCount < 8 && gArmsData[weaponCount].code != 0; ++weaponCount) {}
			
			int adjLeftIdx = (gSelectedArms > 0 ? gSelectedArms - 1 : weaponCount - 1);
			int adjRightIdx = (gSelectedArms + 1) % weaponCount;
			if (isAutofireGun(gArmsData[adjLeftIdx]) || isAutofireGun(gArmsData[adjRightIdx]))
				return TurboMode::AutofireAbuse;
		}
	}

	// If none of the above apply, then just use regular autofire turbo
	return TurboMode::Standard;
}

TurboState* TurboState::getState(TurboMode mode, int input)
{
	switch (mode)
	{
	case TurboMode::None:
		return new NoTurbo(input);
	case TurboMode::Standard:
		return new AutofireTurbo(input);
	case TurboMode::TextAdvance:
		return new TextAdvanceTurbo(input);
	case TurboMode::AutofireGun:
		return new MachineGunTurbo(input);
	case TurboMode::AutofireCurly:
		return new AutofireTurbo(input, true);
	case TurboMode::AutofireAbuse:
		return new AutofireAbuseTurbo(input);
	case TurboMode::InfiniteSpur:
		return new InfiniteSpurTurbo(input);
	}
	return nullptr; // Shouldn't ever get here
}

TurboState* TurboState::procStateTransition(int input)
{
	setInput(input);
	return getState(getTurboMode(input), input);
}
