#pragma once
#include <State.h>

class DebugState : public State
{
public:
    DebugState() = default;

	void InitState(StateStack* state_stack) override;
	void EndState() override;
    
	void Update(float aDeltaTime) override;
    void Render(bool render_through) override;
	
private:

};