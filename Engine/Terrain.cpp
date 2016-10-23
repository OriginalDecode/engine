#include "stdafx.h"
#include "Terrain.h"
#include "TGA32.h"
#include "Surface.h"

#define DIVIDE 255.f

namespace Snowblind
{
	CTerrain::CTerrain(const std::string& aFile, const CU::Vector3f position, const CU::Vector2f& aSize)
		: myWidth(aSize.x)
		, myDepth(aSize.y)
	{
#ifdef SNOWBLIND_DX11
		myIsNULLObject = false;
		myEffect = myEngine->GetEffect("Data/Shaders/T_Terrain_Base.json");

		TGA32::Image* image = TGA32::Load(aFile.c_str());
		/*u8* data;*/
		myHeightmap.myData = new u8[image->myWidth * image->myHeight];
		for (int i = 0; i < image->myWidth * image->myHeight; ++i)
		{
			myHeightmap.myData[i] = image->myImage[i * 4];
		}
		
		if (*myHeightmap.myData == 0x0000000000000001)
		{
			DL_ASSERT("what the fuck");
		}


		//memcpy(&, &data, sizeof(u8) * (image->myWidth * image->myHeight));
		myHeightmap.myDepth = image->myHeight;
		myHeightmap.myWidth = image->myWidth;

		SAFE_DELETE(image);
		CreateVertices(aSize.x, aSize.y, position);
		mySurface = new CSurface(myEffect);
		mySurface->AddTexture("TerrainAlbedo", "Data/Textures/grass.dds");
#endif
	}

	CTerrain::~CTerrain()
	{
		SAFE_DELETE(mySurface);
	}

	void CTerrain::Render(const CU::Matrix44f& aCameraOrientation, const CU::Matrix44f& aCameraProjection)
	{
#ifdef SNOWBLIND_DX11
		if (!myIsNULLObject)
		{
			__super::Render(aCameraOrientation, aCameraProjection);
			myContext->VSSetConstantBuffers(0, 1, &myConstantBuffer);
			myAPI->SetSamplerState(eSamplerStates::LINEAR_WRAP);
			mySurface->Activate();
			myContext->DrawIndexed(myIndexData->myIndexCount, 0, 0);
			mySurface->Deactivate();
		}
#endif
	}

	void CTerrain::Save(const std::string& aFilename)
	{
		DL_ASSERT("Not implemented.");
	}

	void CTerrain::Load(const std::string& aFilePath)
	{
		DL_ASSERT("Not implemented.");
	}

	std::vector<float> CTerrain::GetVerticeArrayCopy()
	{
		return myVertices;
	}

	std::vector<s32> CTerrain::GetIndexArrayCopy()
	{
		return myIndexes;
	}

	void CTerrain::CreateVertices(u32 width, u32 height, const CU::Vector3f& position)
	{
#ifdef SNOWBLIND_DX11
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

		for (u32 z = 0; z < myHeightmap.myDepth; z++)
		{
			for (u32 x = 0; x < myHeightmap.myWidth; x++)
			{
				SVertexPosNormUVBiTang vertex;
				vertex.position.x = position.x + float(x) * width / float(myHeightmap.myWidth);
				vertex.position.y = position.y + myHeightmap.myData[(myHeightmap.myDepth - (1 + z)) * myHeightmap.myWidth + x] * 128.f / DIVIDE;
				vertex.position.z = position.z + float(z) * height / float(myHeightmap.myDepth);
				vertex.uv.x = float(x) / float(myHeightmap.myWidth);
				vertex.uv.y = float(1.f - z) / float(myHeightmap.myDepth);
				vertices.Add(vertex);
				myVertices.push_back(vertex.position.x);
				myVertices.push_back(vertex.position.y);
				myVertices.push_back(vertex.position.z);


			}
		}

		CalculateNormals(vertices);

		for (int z = 0; z < myHeightmap.myDepth - 1; ++z)
		{
			for (int x = 0; x < myHeightmap.myWidth - 1; ++x)
			{
				indexes.Add(z * myHeightmap.myWidth + x);
				indexes.Add((z + 1) * myHeightmap.myWidth + x);
				indexes.Add(z * myHeightmap.myWidth + x + 1);

				indexes.Add((z + 1) * myHeightmap.myWidth + x);
				indexes.Add((z + 1) * myHeightmap.myWidth + x + 1);
				indexes.Add(z * myHeightmap.myWidth + x + 1);

			}
		}

		for (u32 index : indexes)
		{
			myIndexes.push_back(index);
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
#endif
	}

	void CTerrain::SetMatrices(const CU::Matrix44f& aCameraOrientation, const CU::Matrix44f& aCameraProjection)
	{
#ifdef SNOWBLIND_DX11
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
#endif
	}

	void CTerrain::InitConstantBuffer()
	{
#ifdef SNOWBLIND_DX11
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
#endif
	}

	void CTerrain::CalculateNormals(CU::GrowingArray<SVertexPosNormUVBiTang>& VertArray)
	{

		unsigned int height = myHeightmap.myDepth;
		unsigned int width = myHeightmap.myWidth;
		float yScale = 128.f / DIVIDE;
		yScale *= 0.2f;
		//float xScale = mySize.x / myHeightMap->myDepth;
		float xzScale = float(myDepth) / float(myHeightmap.myDepth);


		for (unsigned int y = 0; y < height; ++y)
		{
			for (unsigned int x = 0; x < width; ++x)
			{
				float sx = GetHeight(x < width - 1 ? x + 1 : x, y) - GetHeight(x == 0 ? x : x - 1, y);
				if (x == 0 || x == width - 1)
					sx *= 2;

				float sy = GetHeight(x, y < height - 1 ? y + 1 : y) - GetHeight(x, y == 0 ? y : y - 1);
				if (y == 0 || y == height - 1)
					sy *= 2;

				CU::Vector3f normal(-sx*xzScale, yScale, sy*xzScale);
				CU::Math::Normalize(normal);
				normal.z = -normal.z;

				VertArray[y*width + x].normal = normal;
			}
		}
	}

	float CTerrain::GetHeight(unsigned int aX, unsigned int aY) const
	{
		return myHeightmap.myData[(myHeightmap.myDepth - (1 + aY)) * myHeightmap.myWidth + aX] / DIVIDE;
	}

	float CTerrain::GetHeight(unsigned int aIndex) const
	{
		return myHeightmap.myData[aIndex] / DIVIDE;
	}

	SHeightMap Create(const char* aFilePath)
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

		SHeightMap height_map;
		height_map.myWidth = width;
		height_map.myDepth = depth;
		height_map.myData = data;

//		return SHeightMap(width, depth, data);
		return height_map;
	}

	//SHeightMap::SHeightMap(u32 aWidth, u32 aDepth, u8* const someData)
	//	: myWidth(aWidth)
	//	, myDepth(aDepth)
	//	, myData(someData)
	//{
	//}

	//SHeightMap::SHeightMap(std::fstream& aStream)
	//	: myWidth(0)
	//	, myDepth(0)
	//	, myData(nullptr)
	//{
	//	aStream.read((s8*)&myWidth, sizeof(u32));
	//	aStream.read((s8*)&myDepth, sizeof(u32));
	//	myData = new u8[myWidth * myDepth];
	//	aStream.read((s8*)myData, sizeof(s8) * myWidth * myDepth);
	//}

};