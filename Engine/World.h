#pragma once
#include "TreeNode.h"

namespace Hex
{
	class Synchronizer;
};

class World
{
public:
	World() = default;
	void Initiate(CU::Vector3f position);
	void Update(float dt);
private:
	TreeNode m_Root;
	CU::Vector3f m_WorldPos;
	Hex::Synchronizer* m_Synchronizer;
	
};

