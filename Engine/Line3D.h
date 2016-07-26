#pragma once
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



namespace Snowblind
{
	class CDirectX11;
	class CEffect;
	struct SVertexBufferWrapper;

}

struct ID3D11Buffer;
struct SVertexBaseStruct;
struct ID3D11InputLayout;

class CLine3D
{
public:
	CLine3D();
	~CLine3D();

	void Initiate(int aLineAmount = 256);


	void Update(const SLinePoint& firstPoint, const SLinePoint& secondPoint);
	void Render(CU::Matrix44f& prevOrientation, CU::Matrix44f& projection);

	void AddLine(const SLine& aLine);
	void AddCube(const CU::Vector3f& min, const CU::Vector3f& max);

private:
	void CreateConstantBuffer();
	void CreateVertexBuffer();


	void SetMatrices(CU::Matrix44f& aCameraOrientation, CU::Matrix44f& aCameraProjection);

	SLinePoint myFirstPoint;
	SLinePoint mySecondPoint;

	CU::Matrix44f myOrientation;
	Snowblind::CDirectX11* myAPI = nullptr;
	Snowblind::CEffect* myEffect = nullptr;
	Snowblind::SVertexBufferWrapper* myVertexBuffer = nullptr;

	SVertexBaseStruct* myConstantStruct = nullptr;


	ID3D11InputLayout* myVertexLayout = nullptr;
	ID3D11Buffer* myConstantBuffer = nullptr;

	CU::GrowingArray<SLinePoint> myVertices;
	int myLineAmount = 0;
};

