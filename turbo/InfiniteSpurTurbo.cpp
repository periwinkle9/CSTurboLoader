#include "InfiniteSpurTurbo.h"
#include "../cs_addresses.h"

InfiniteSpurTurbo::InfiniteSpurTurbo(int in) : TurboState(in)
{
	timer = 0;
}

TurboState* InfiniteSpurTurbo::procStateTransition(int input)
{
	setInput(input);
	
	// If we're in the middle of doing the inputs for firing the Spur,
	// then finish doing those inputs no matter what.
	// I may regret this later, but let's see how this goes for now
	if (timer > 0)
		return this;

	TurboMode mode = getTurboMode(input);
	if (mode == TurboMode::InfiniteSpur)
		return this;
	else
		return getState(mode, input);
}

void InfiniteSpurTurbo::procInput()
{
	int input = getInput();

	switch (timer++)
	{
	case 0:
		input |= gKeyShot;
		if (CountArmsBullet(13) > 0 || CountArmsBullet(14) > 0)
			timer = 0;
		else
			input |= gKeyItem;
		break;
	case 1:
		// Prevent accidental menuing
		input &= ~(gKeyItem | gKeyLeft | gKeyRight | gKeyUp | gKeyDown | gKeyOk | gKeyCancel);
		break;
	case 2:
		input |= gKeyItem;
		input &= ~(gKeyShot | gKeyLeft | gKeyRight);
		break;
	case 3:
		input |= gKeyShot;
		// Fallthrough to reset timer
	default:
		timer = 0;
	}

	setInput(input);
}
