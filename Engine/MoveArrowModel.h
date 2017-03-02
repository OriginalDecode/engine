#pragma once
#include <Math/Matrix/Matrix.h>
class CModel;
class Texture;

struct DirectionalArrow
{
	~DirectionalArrow();
	std::string m_Key;

	CModel* m_Arrow = nullptr;
	Texture* m_DirColor = nullptr;
	CU::Matrix44f m_Orientation;
};

class MoveArrowModel
{
public:

	MoveArrowModel();
	void Initiate();
	void CleanUp();
	void Render();
	void Update();
	//Collision check?
	void SetPosition(const CU::Vector3f& pos);
private:

	DirectionalArrow m_Right;  // red
	DirectionalArrow m_Up; // green
	DirectionalArrow m_Forward; //blue
	CU::Matrix44f m_Orientation;
};

