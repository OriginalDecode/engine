#pragma once
#include "State.h"

class CPauseState : public State
{
public:
	CPauseState() = default;
	void InitState(StateStack* state_stack) override;
	void Update(float aDeltaTime) override;

	void EndState() override;
	void Render(bool render_through) override;

private:

};

