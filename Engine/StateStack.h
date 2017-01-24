#pragma once
#include "snowblind_shared.h"
#include <DataStructures/GrowingArray.h>

class State;
typedef CU::GrowingArray<State*> SubStateContainer;
typedef CU::GrowingArray<SubStateContainer> MainStateContainer;
class StateStack
{
public:
	StateStack() = default;

	enum eGameState
	{
		MAIN,
		SUB
	};
	void PopCurrentMainState();
	void PopCurrentSubState();
	void PushState(State* game_state, eGameState game_state_type);
	void PauseCurrentState();
	void ResumeCurrentState();
	//void PopState(u32 state_id);

	bool UpdateCurrentState(float dt);


	void Clear();

private:
	MainStateContainer m_GameStates;
	s32 m_MainIndex = 0;
	s32 m_SubIndex = 0;
};

