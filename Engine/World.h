#pragma once
#include "TreeNode.h"

class World
{
public:
	World() = default;
	void Initiate(CU::Vector3f position);
	void Update(float dt);
private:
	TreeNode m_Root;
	CU::Vector3f m_WorldPos;
};

