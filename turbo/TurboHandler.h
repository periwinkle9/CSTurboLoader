#pragma once

#include <memory>

class TurboState;
class TurboHandler
{
	std::unique_ptr<TurboState> state;
public:
	TurboHandler();
	~TurboHandler();

	// Too lazy to implement (std::unique_ptr can't be copied anyways)
	TurboHandler(const TurboHandler&) = delete;
	TurboHandler& operator=(const TurboHandler&) = delete;
	// I think these are fine as default?
	TurboHandler(TurboHandler&&) = default;
	TurboHandler& operator=(TurboHandler&&) = default;

	int getNextInput(int input);
};
