#include "TextAdvanceTurbo.h"
#include "../cs_addresses.h"
#include <stdexcept>

TextAdvanceTurbo::TextAdvanceTurbo(int in) : TurboState(in)
{
	buttonPress = ButtonPressState::NoPress;
}

TurboState* TextAdvanceTurbo::procStateTransition(int input)
{
	setInput(input);
	TurboMode mode = getTurboMode(input);
	
	if (mode == TurboMode::TextAdvance)
		return this;
	else
	{
		// Event ended, check if pushing down to enter door
		if ((input & KEY_DOWN) && buttonPress == ButtonPressState::Press)
			return this;
		else
			return getState(mode, input);
	}
}

void TextAdvanceTurbo::procInput()
{
	int input = getInput();

	switch (gTSmode)
	{
	case 0: // Event ended, push down to enter door
		if (buttonPress != ButtonPressState::Press)
			throw std::logic_error("Event ended but still in text mashing mode?!");

		input = KEY_DOWN; // Can only be holding down to enter doors
		buttonPress = ButtonPressState::AlreadyPressed;
		
		break;
	case 1: // TSC processing
		if (!(g_GameFlags & 2)) // Disable if the player still has control
		{
			input |= KEY_X;
			input &= ~KEY_Z;
			// Prepare a down press at the end of the event, if the player is holding down
			if (input & KEY_DOWN)
			{
				buttonPress = ButtonPressState::Press;
				input &= ~KEY_DOWN;
			}
			else
				buttonPress = ButtonPressState::NoPress;
		}
		break;
	case 2: // NOD
		input |= (KEY_Z | KEY_X);
		break;
	case 6: // YNJ
		if (input & (KEY_LEFT | KEY_RIGHT)) // Auto-select option if the player is holding left or right
		{
			if (gTSwait < 16)
			{
				input &= ~(KEY_Z | KEY_RIGHT);
				buttonPress = ButtonPressState::NoPress;
			}
			else if (input & KEY_LEFT)
				input |= KEY_Z;
			else // Holding right
			{
				if (buttonPress == ButtonPressState::NoPress) // Cursor over to "No"
				{
					input |= KEY_RIGHT;
					buttonPress = ButtonPressState::Press;
				}
				else if (buttonPress == ButtonPressState::Press) // Then press OK on the next frame
				{
					input |= KEY_Z;
					buttonPress = ButtonPressState::AlreadyPressed;
				}
			}
		}
		break;
	}

	setInput(input);
}
