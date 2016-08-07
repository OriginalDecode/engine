#include "stdafx.h"
#include "Terrain.h"

namespace Snowblind
{
	CTerrain::CTerrain()
	{
		myIsNULLObject = false;

	}


	CTerrain::CTerrain(u32 width, u32 height)
	{
		myIsNULLObject = false;
		myEffect = myEngine->GetEffect("Data/Shaders/T_Terrain_Base.json");
		CreateVertices(width, height);
	}

	CTerrain::CTerrain(const std::string& aFilePath)
	{
		DL_ASSERT("Not implemented.");
		myIsNULLObject = false;
		Load(aFilePath);
	}

	CTerrain::~CTerrain()
	{
	}

	void CTerrain::Render(const CU::Matrix44f& aCameraOrientation, const CU::Matrix44f& aCameraProjection)
	{
		__super::Render(aCameraOrientation, aCameraProjection);
		if (!myIsNULLObject)
		{
			myContext->VSSetConstantBuffers(0, 1, &myConstantBuffer);
			myAPI->SetSamplerState(eSamplerStates::LINEAR_WRAP);
			myContext->DrawIndexed(myIndexData->myIndexCount, 0, 0);
		}
	}

	void CTerrain::Save(const std::string& aFilename)
	{
		DL_ASSERT("Not implemented.");
	}

	void CTerrain::Load(const std::string& aFilePath)
	{
		DL_ASSERT("Not implemented.");
	}

	void CTerrain::CreateVertices(u32 width, u32 height)
	{
		D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		myVertexFormat.Init(5);
		myVertexFormat.Add(vertexDesc[0]);
		myVertexFormat.Add(vertexDesc[1]);
		myVertexFormat.Add(vertexDesc[2]);
		myVertexFormat.Add(vertexDesc[3]);
		myVertexFormat.Add(vertexDesc[4]);

		float newWidth = float(width) / 2.f;
		float newHeight = float(height) / 2.f;

		float cellWidth = 1.f;
		float cellHeight = 1.f;

		u32 cellWidthAmount = width / cellWidth;
		u32 cellHeightAmount = height / cellHeight;

		CU::GrowingArray<SVertexPosNormUVBiTang> vertices;
		CU::GrowingArray<u32> indexes;
		float wAndH = 1024;

		for (u32 i = 0; i < wAndH; i++)
		{
			for (u32 j = 0; j < wAndH; j++)
			{
				SVertexPosNormUVBiTang vertex;
				vertex.position.x = float(j) * width / float(wAndH);
				vertex.position.z = float(i) * height / float(wAndH);
				vertex.uv.x = float(j) / float(wAndH);
				vertex.uv.y = float(1.f - i) / float(wAndH);
				vertices.Add(vertex);
			}
		}

		for (int z = 0; z < wAndH - 1; ++z)
		{
			for (int x = 0; x < wAndH - 1; ++x)
			{
				indexes.Add(z * wAndH + x);
				indexes.Add((z + 1) * wAndH + x);
				indexes.Add(z * wAndH + x + 1);
				indexes.Add((z + 1) * wAndH + x);
				indexes.Add((z + 1) * wAndH + x + 1);
				indexes.Add(z * wAndH + x + 1);
			}
		}

		myVertexData = new SVertexDataWrapper;
		myConstantStruct = new SVertexBaseStruct;
		myVertexData->myNrOfVertexes = vertices.Size();
		myVertexData->myStride = sizeof(SVertexPosNormUVBiTang);
		myVertexData->mySize = myVertexData->myNrOfVertexes*myVertexData->myStride;
		myVertexData->myVertexData = new  char[myVertexData->mySize]();
		memcpy(myVertexData->myVertexData, &vertices[0], myVertexData->mySize);


		myIndexData = new SVertexIndexWrapper;
		myIndexData->myFormat = DXGI_FORMAT_R32_UINT;
		myIndexData->myIndexCount = indexes.Size();
		myIndexData->mySize = myIndexData->myIndexCount * 4;

		myIndexData->myIndexData = new char[myIndexData->mySize];
		memcpy(myIndexData->myIndexData, &indexes[0], myIndexData->mySize);

		InitVertexBuffer();
		InitIndexBuffer();
		InitConstantBuffer();


	}

	void CTerrain::SetMatrices(const CU::Matrix44f& aCameraOrientation, const CU::Matrix44f& aCameraProjection)
	{
		if (myIsNULLObject == false)
		{
			DL_ASSERT_EXP(myConstantStruct != nullptr, "Vertex Constant Buffer Struct was null.");

			myConstantStruct->world = myOrientation;
			myConstantStruct->invertedView = CU::Math::Inverse(aCameraOrientation);
			myConstantStruct->projection = aCameraProjection;


			D3D11_MAPPED_SUBRESOURCE msr;
			myAPI->GetContext()->Map(myConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
			if (msr.pData != nullptr)
			{
				SVertexBaseStruct* ptr = (SVertexBaseStruct*)msr.pData;
				memcpy(ptr, &myConstantStruct->world.myMatrix[0], sizeof(SVertexBaseStruct));
			}

			myAPI->GetContext()->Unmap(myConstantBuffer, 0);
		}
	}

};