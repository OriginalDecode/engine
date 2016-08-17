#include "stdafx.h"
#include "Terrain.h"
#include "TGA32.h"
#include "Surface.h"

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
		myHeightmap = SHeightMap::Create("Data/Textures/T_heightmap_level_00.tga");
		CreateVertices(width, height);
		mySurface = new CSurface(myEffect);
		mySurface->AddTexture("TerrainAlbedo", "Data/Textures/No-Texture.dds");

	}

	CTerrain::CTerrain(const std::string& aFilePath)
	{
		DL_ASSERT("Not implemented.");
		myIsNULLObject = false;
		Load(aFilePath);
	}

	CTerrain::~CTerrain()
	{
		SAFE_DELETE(mySurface);
	}

	void CTerrain::Render(const CU::Matrix44f& aCameraOrientation, const CU::Matrix44f& aCameraProjection)
	{
		if (!myIsNULLObject)
		{
			__super::Render(aCameraOrientation, aCameraProjection);
			myContext->VSSetConstantBuffers(0, 1, &myConstantBuffer);
			myAPI->SetSamplerState(eSamplerStates::LINEAR_WRAP);
			mySurface->Activate();
			myContext->DrawIndexed(myIndexData->myIndexCount, 0, 0);
			mySurface->Deactivate();
		}
	}

	void CTerrain::Save(const std::string& aFilename)
	{
		DL_ASSERT("Not implemented.");
	}

	void CTerrain::Load(const std::string& aFilePath)
	{

		/* What format should we use for our heightmaps. */

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

		CU::GrowingArray<SVertexPosNormUVBiTang> vertices;
		CU::GrowingArray<u32> indexes;
		u32 wAndH = 512;

		for (u32 z = 0; z < myHeightmap->myDepth; z++)
		{
			for (u32 x = 0; x < myHeightmap->myWidth; x++)
			{
				SVertexPosNormUVBiTang vertex;
				vertex.position.x = float(x) * width / float(myHeightmap->myWidth);
				vertex.position.y = myHeightmap->myData[(myHeightmap->myDepth - (1 + z)) * myHeightmap->myWidth + x]  * height / 255.f;
				vertex.position.z = float(z) * height / float(myHeightmap->myDepth);
				vertex.uv.x = float(x) / float(myHeightmap->myWidth);
				vertex.uv.y = float(1.f - z) / float(myHeightmap->myDepth);
				vertices.Add(vertex);
			}
		}

		for (int z = 0; z < myHeightmap->myDepth - 1; ++z)
		{
			for (int x = 0; x < myHeightmap->myWidth- 1; ++x)
			{
				indexes.Add(z * myHeightmap->myWidth + x);
				indexes.Add((z + 1) * myHeightmap->myWidth + x);
				indexes.Add(z * myHeightmap->myWidth + x + 1);

				indexes.Add((z + 1) * myHeightmap->myWidth + x);
				indexes.Add((z + 1) * myHeightmap->myWidth  + x + 1);
				indexes.Add(z * myHeightmap->myWidth + x + 1);
			}
		}

		myVertexData = new SVertexDataWrapper;
		myConstantStruct = new TerrainConstantStruct;
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
			myConstantStruct->time.x = myEngine->GetDeltaTime();
			D3D11_MAPPED_SUBRESOURCE msr;
			myAPI->GetContext()->Map(myConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
			if (msr.pData != nullptr)
			{
				TerrainConstantStruct* ptr = (TerrainConstantStruct*)msr.pData;
				memcpy(ptr, &myConstantStruct->world.myMatrix[0], sizeof(TerrainConstantStruct));
			}

			myAPI->GetContext()->Unmap(myConstantBuffer, 0);
		}
	}

	void CTerrain::InitConstantBuffer()
	{
		D3D11_BUFFER_DESC cbDesc;
		ZeroMemory(&cbDesc, sizeof(cbDesc));
		cbDesc.ByteWidth = sizeof(TerrainConstantStruct);
		cbDesc.Usage = D3D11_USAGE_DYNAMIC;
		cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbDesc.MiscFlags = 0;
		cbDesc.StructureByteStride = 0;

		HRESULT hr = myAPI->GetDevice()->CreateBuffer(&cbDesc, 0, &myConstantBuffer);
		myAPI->SetDebugName(myConstantBuffer, "Model cb");
		myAPI->HandleErrors(hr, "[BaseModel] : Failed to Create Constant Buffer, ");
	}

	SHeightMap* SHeightMap::Create(const char* aFilePath)
	{
		TGA32::Image* image = TGA32::Load(aFilePath);

		u32 width = image->myWidth;
		u32 depth = image->myHeight;

		u8* data = new u8[width * depth];

		for (int i = 0; i < width * depth; ++i)
		{
			data[i] = image->myImage[i * 4];
		}

		SAFE_DELETE(image);

		return new SHeightMap(width, depth, data);
	}

	SHeightMap::SHeightMap(u32 aWidth, u32 aDepth, u8* const someData)
		: myWidth(aWidth)
		, myDepth(aDepth)
		, myData(someData)
	{
	}

	SHeightMap::SHeightMap(std::fstream& aStream)
		: myWidth(0)
		, myDepth(0)
		, myData(nullptr)
	{
		aStream.read((s8*)&myWidth, sizeof(u32));
		aStream.read((s8*)&myDepth, sizeof(u32));
		myData = new u8[myWidth * myDepth];
		aStream.read((s8*)myData, sizeof(s8) * myWidth * myDepth);
	}

};