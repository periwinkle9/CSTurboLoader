#pragma once

// Normal non-turboing state

#include "TurboState.h"

class NoTurbo : public TurboState
{
public:
	NoTurbo(int in = 0) : TurboState(in) {}
	void procInput() override {} // Nothing to do here
	bool isTurbo() const override { return false; }
#ifdef _DEBUG
	const char* name() const override { return "NoTurbo"; }
#endif
};
