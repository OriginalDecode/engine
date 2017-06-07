#pragma once
#include "engine_shared.h"
#include <Math/Matrix/Matrix44.h>
#include <DataStructures/GrowingArray.h>


struct SLinePoint
{
	SLinePoint(const CU::Vector4f& pos, const CU::Vector4f& col);
	SLinePoint();
	CU::Vector4f position;
	CU::Vector4f color;
};

struct SLine
{
	SLinePoint first;
	SLinePoint second;
};




class DirectX11;
class Effect;
struct VertexBufferWrapper;


struct VertexBaseStruct;

class CLine3D
{
public:
	CLine3D();
	~CLine3D();

	void Initiate(int aLineAmount = 256);


	void Update(const SLinePoint& firstPoint, const SLinePoint& secondPoint);
	void Render(const CU::Matrix44f& prevOrientation, const CU::Matrix44f& projection);

	void AddLine(const SLine& aLine);
	void AddCube(const CU::Vector3f& min, const CU::Vector3f& max);

private:
	void CreateConstantBuffer();
	void CreateBuffer();


	void SetMatrices(const CU::Matrix44f& aCameraOrientation, const CU::Matrix44f& aCameraProjection);

	SLinePoint myFirstPoint;
	SLinePoint mySecondPoint;

	CU::Matrix44f myOrientation;
	DirectX11* myAPI = nullptr;
	Effect* myEffect = nullptr;
	VertexBufferWrapper* myVertexBuffer = nullptr;

	VertexBaseStruct* myConstantStruct = nullptr;

#ifdef SNOWBLIND_DX11
	IInputLayout* myVertexLayout = nullptr;
	ID3D11Buffer* myConstantBuffer = nullptr;
#endif
	CU::GrowingArray<SLinePoint> myVertices;
	int myLineAmount = 0;
};

