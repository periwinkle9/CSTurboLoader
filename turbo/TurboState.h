#pragma once

class TurboState
{
	int savedInput;
protected:
	void setInput(int in) { savedInput = in; }
	enum class TurboMode { None, Standard, TextAdvance, AutofireGun, AutofireCurly, AutofireAbuse, InfiniteSpur };
	static TurboMode getTurboMode(int input);
	static TurboState* getState(TurboMode mode, int input);
public:
	TurboState(int in = 0) : savedInput(in) {}
	virtual ~TurboState() = default;

	virtual void procInput() = 0;
	virtual TurboState* procStateTransition(int input);

	int getInput() const { return savedInput; }

	virtual bool isTurbo() const { return true; } // Default to true

#ifdef _DEBUG
	virtual const char* name() const = 0;
#endif
};
