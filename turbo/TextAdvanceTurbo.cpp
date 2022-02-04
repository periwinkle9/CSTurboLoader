#include "TextAdvanceTurbo.h"
#include "../cs_addresses.h"
#include <stdexcept>

TextAdvanceTurbo::TextAdvanceTurbo(int in) : TurboState(in)
{
	buttonPress = ButtonPressState::NoPress;
	init = true;
}

TurboState* TextAdvanceTurbo::procStateTransition(int input)
{
	setInput(input);
	TurboMode mode = getTurboMode(input);
	
	if (mode == TurboMode::TextAdvance)
		return this;
	else
	{
		// Check if pushing down to enter door
		if ((input & KEY_TURBO) && !(input & (gKeyItem | gKeyMap | KEY_ESCAPE)) &&
			(input & gKeyDown) && buttonPress == ButtonPressState::Press)
			return this;
		else
			return getState(mode, input);
	}
}

void TextAdvanceTurbo::initActions()
{
	int input = getInput();
	init = false;

	// If we started in a <NOD and the player is already holding both Z and X, release them
	if (gTSmode == 2 && (input & gKeyOk) && (input & gKeyCancel))
		input &= ~(gKeyOk | gKeyCancel);
	// If we're sitting at a Y/N prompt, release all of the relevant buttons
	else if (gTSmode == 6 && gTSwait >= 16)
		input &= ~(gKeyOk | gKeyLeft | gKeyRight);
	else // If none of the above, just do the normal thing
		return procInput();

	setInput(input);
}

void TextAdvanceTurbo::procInput()
{
	if (init) // Take care of some first frame edge cases
		return initActions();

	int input = getInput();

	switch (gTSmode)
	{
	case 0: // Event ended, push down to enter door
		if (buttonPress != ButtonPressState::Press)
			throw std::logic_error("Event ended but still in text mashing mode?!");

		input = gKeyDown; // Can only be holding down to enter doors
		buttonPress = ButtonPressState::AlreadyPressed;
		
		break;
	case 1: // TSC processing
		if (!(g_GameFlags & 2)) // Disable if the player still has control
		{
			input |= gKeyOk;
			input &= ~gKeyCancel;
			// Prepare a down press at the end of the event, if the player is holding down
			if (input & gKeyDown)
			{
				buttonPress = ButtonPressState::Press;
				input &= ~gKeyDown;
			}
			else
				buttonPress = ButtonPressState::NoPress;
		}
		break;
	case 2: // NOD
		input |= (gKeyOk | gKeyCancel);
		break;
	case 6: // YNJ
		if (input & (gKeyLeft | gKeyRight)) // Auto-select option if the player is holding left or right
		{
			if (gTSwait < 16)
			{
				input &= ~(gKeyOk | gKeyRight);
				buttonPress = (input & gKeyLeft) ? ButtonPressState::Press : ButtonPressState::NoPress;
			}
			else if (input & gKeyLeft)
			{
				if (buttonPress == ButtonPressState::NoPress)
				{
					input &= ~gKeyOk;
					buttonPress = ButtonPressState::Press;
				}
				else if (buttonPress == ButtonPressState::Press)
				{
					input |= gKeyOk;
					buttonPress = ButtonPressState::AlreadyPressed;
				}
			}
			else // Holding right
			{
				if (buttonPress == ButtonPressState::NoPress) // Cursor over to "No"
				{
					input |= gKeyRight;
					input &= ~gKeyOk;
					buttonPress = ButtonPressState::Press;
				}
				else if (buttonPress == ButtonPressState::Press) // Then press OK on the next frame
				{
					input |= gKeyOk;
					buttonPress = ButtonPressState::AlreadyPressed;
				}
			}
		}
		break;
	}

	setInput(input);
}
