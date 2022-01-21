#pragma once

// Standard no-frills turbo

#include "TurboState.h"

class AutofireTurbo : public TurboState
{
	int timer;
public:
	AutofireTurbo(int in = 0) : TurboState(in) { timer = 0; }
	TurboState* procStateTransition(int input) override;
	void procInput() override;

#ifdef _DEBUG
	const char* name() const override { return "AutofireTurbo"; }
#endif
};
