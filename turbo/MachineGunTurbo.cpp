#include "MachineGunTurbo.h"
#include "../cs_addresses.h"

TurboState* MachineGunTurbo::procStateTransition(int input)
{
	setInput(input);
	TurboMode mode = getTurboMode(input);
	if (mode == TurboMode::AutofireGun)
		return this;
	else
		return getState(mode, input);
}

void MachineGunTurbo::procInput()
{
	int input = getInput();

	switch (timer++)
	{
	case 0:
		// Wait until you can actually fire
		if (soft_rensha > 1)
		{
			timer = 0;
			input &= ~gKeyShot;
		}
		else
			input |= gKeyShot;
		break;
	case 1:
		input &= ~gKeyShot;
		break;
	case 2:
		input |= gKeyShot;
		break;
	case 3:
		input |= gKeyShot;
		break;
	case 4:
		input &= ~gKeyShot;
		// Fallthrough to reset timer
	default:
		timer = 0;
	}

	setInput(input);
}
