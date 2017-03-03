#pragma once
#include <Math/Matrix/Matrix.h>
class CModel;
class Texture;



struct DirectionalArrow
{
	enum class eDirection
	{
		RIGHT,
		UP,
		FORWARD,
	};
	eDirection direction;

	~DirectionalArrow();
	std::string m_Key;

	CModel* m_Arrow = nullptr;
	Texture* m_DirColor = nullptr;
	CU::Matrix44f m_Orientation;

	CU::Vector3f m_MinPos;
	CU::Vector3f m_MaxPos;
	bool Inside(const CU::Vector3f& position);
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

	DirectionalArrow& GetRight() { return m_Right; }
	DirectionalArrow& GetUp() { return m_Up; }
	DirectionalArrow& GetForward() { return m_Forward; }


private:
	DirectionalArrow m_Right;  // red
	DirectionalArrow m_Up; // green
	DirectionalArrow m_Forward; //blue
	CU::Matrix44f m_Orientation;
};

