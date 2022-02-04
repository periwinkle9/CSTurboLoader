// State machine-like implementation because why not (:

#include "TurboHandler.h"
#include "TurboState.h"
#include "NoTurbo.h"
#include <stdexcept>
#ifdef _DEBUG
#include "../common.h"
#include "../cs_addresses.h"
#include <stdio.h>
static bool paused = false;
#endif

TurboHandler::TurboHandler() : state(new NoTurbo)
{
#ifdef _DEBUG
	AllocConsole();
	FILE* newStdout;
	freopen_s(&newStdout, "CONOUT$", "w", stdout);
#endif
}
TurboHandler::~TurboHandler()
{
	// Nothing to do here; std::unique_ptr will clean up for itself
}

int TurboHandler::getNextInput(int input)
{
	// Stop if something went wrong
	if (!state)
		return input;

	int newInput;
	try
	{
		TurboState* newState = state->procStateTransition(input);
		if (newState != state.get())
			state.reset(newState);

		state->procInput();

		newInput = state->getInput();
	}
	catch (std::logic_error& e)
	{
#ifdef _DEBUG
		std::printf("BUG DETECTED! Error: %s\nStopping turbo handling\n", e.what());
		paused = true;
#else
		(void)e; // Shut up compiler warnings about an unused local variable
#endif
		state.reset();
		newInput = input;
	}

#ifdef _DEBUG
	if (gKeyTrg & KEY_PLUS)
	{
		paused = !paused;
		std::puts(paused ? "paused" : "unpaused");
	}
	if (!paused)
	{
		// Ugly but serviceable
		auto populateInputs = [](char* str, int in)
		{
			if (in & gKeyJump) str[0] = 'Z';
			if (in & gKeyShot) str[1] = 'X';
			if (in & gKeyArmsRev) str[2] = 'A';
			if (in & gKeyArms) str[3] = 'S';
			if (in & gKeyItem) str[4] = 'Q';
			if (in & gKeyMap) str[5] = 'W';
			if (in & gKeyLeft) str[6] = 'L';
			if (in & gKeyUp) str[7] = 'U';
			if (in & gKeyRight) str[8] = 'R';
			if (in & gKeyDown) str[9] = 'D';
		};
		char inputStr[11] = "          ";
		populateInputs(inputStr, newInput);

		char realInputStr[12] = "           ";
		populateInputs(realInputStr, input);
		if (input & KEY_TURBO)
			realInputStr[10] = 't';
		std::printf("State %s: %s, actual inputs %s\n", state->name(), inputStr, realInputStr);
	}
#endif

	return newInput;
}
