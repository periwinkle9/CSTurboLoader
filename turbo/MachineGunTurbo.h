#pragma once

// Machine Gun (and Bubbler 2/3) autofire turbo

#include "TurboState.h"

class MachineGunTurbo : public TurboState
{
	// I *could* be intelligent about this and decide when to fire based on soft_rensha and gMC.rensha,
	// but let's just do this the naive way and base the firing pattern off of a timer,
	// because that's what a normal macro would do
	int timer;
public:
	MachineGunTurbo(int in = 0) : TurboState(in) { timer = 0; }
	TurboState* procStateTransition(int input) override;
	void procInput() override;

#ifdef _DEBUG
	const char* name() const override { return "MachineGunTurbo"; }
#endif
};
