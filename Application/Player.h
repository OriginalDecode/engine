#pragma once
#include <standard_datatype.hpp>

/*
	This class is more of a gathering point for everything related to the player
	mechanics and such.
*/

class RigidBody;
class TreeDweller;

class Player
{
public:
	Player() = default;
	~Player();

	TreeDweller* Initiate();


	void Update(float dt);

private:
	RigidBody* m_Body = nullptr;
	uint32 m_EntityID = 0;

};

