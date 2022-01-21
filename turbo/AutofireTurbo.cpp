#include "AutofireTurbo.h"
#include "../cs_addresses.h"

TurboState* AutofireTurbo::procStateTransition(int input)
{
	setInput(input);
	TurboMode mode = getTurboMode(input);
	if (mode == TurboMode::Standard)
		return this;
	else
		return getState(mode, input);
}

void AutofireTurbo::procInput()
{
	int input = getInput();
	// Wait until you can shoot
	if (timer == 0 && soft_rensha > 1)
	{
		input &= ~KEY_X;
	}
	else
	{
		if (timer == 0 || (timer == 2 && IsNpCharCode(320))) // Also push the button if you have Curly's Nemesis
			input |= KEY_X;
		else
			input &= ~KEY_X;
		timer = (timer + 1) % 4;
	}
	
	setInput(input);
}
