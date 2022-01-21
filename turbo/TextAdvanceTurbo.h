#pragma once

// Text mashing turbo (also handles first-frame door entry after event)

#include "TurboState.h"

class TextAdvanceTurbo : public TurboState
{
	enum class ButtonPressState { NoPress, Press, AlreadyPressed };
	ButtonPressState buttonPress;
public:
	TextAdvanceTurbo(int in = 0);
	TurboState* procStateTransition(int input) override;
	void procInput() override;

#ifdef _DEBUG
	const char* name() const override { return "TextAdvanceTurbo"; }
#endif
};
