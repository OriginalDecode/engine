#pragma once
#include <Math/Matrix/Matrix.h>
class QuadTreeNode;
class QuadTree
{
public:
	QuadTree() = default;
	QuadTree(float first_halfwidth, CU::Vector4f center_pos);
	~QuadTree();


	void Update();


private:

	QuadTreeNode* m_Root = nullptr;

};

