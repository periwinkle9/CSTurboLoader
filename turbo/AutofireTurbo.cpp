#include "AutofireTurbo.h"
#include "../cs_addresses.h"

AutofireTurbo::AutofireTurbo(int in, bool curly) : TurboState(in)
{
	timer = 0;
	hasCurly = curly;
}

TurboState* AutofireTurbo::procStateTransition(int input)
{
	setInput(input);
	TurboMode mode = getTurboMode(input);
	if ((!hasCurly && mode == TurboMode::Standard) || (hasCurly && mode == TurboMode::AutofireCurly))
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
		if (timer == 0 || (hasCurly && timer == 2)) // Also push the button if you have Curly's Nemesis
			input |= KEY_X;
		else
			input &= ~KEY_X;
		timer = (timer + 1) % 4;
	}
	
	setInput(input);
}
