#pragma once

// Infinite Spur turbo

#include "TurboState.h"

class InfiniteSpurTurbo : public TurboState
{
	int timer;
public:
	InfiniteSpurTurbo(int in = 0);
	TurboState* procStateTransition(int input) override;
	void procInput() override;

#ifdef _DEBUG
	const char* name() const override { return "InfiniteSpurTurbo"; }
#endif
};
