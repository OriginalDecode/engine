#include "stdafx.h"
#include "Terrain.h"
#include "TGA32.h"
#include "Surface.h"

#define DIVIDE 255.f


bool Terrain::Initiate(const std::string& aFile, const CU::Vector3f position, const CU::Vector2f& aSize)
{
	myWidth = aSize.x;
	myDepth = aSize.y;
	m_Filename = "Terrain";
	m_IsRoot = false;
	myEffect = Engine::GetInstance()->GetEffect("Shaders/terrain_base.json");

	TGA32::Image* image = TGA32::Load(aFile.c_str());
	/*u8* data;*/
	myHeightmap.myData = new u8[image->myWidth * image->myHeight];
	for (int i = 0; i < image->myWidth * image->myHeight; ++i)
	{
		myHeightmap.myData[i] = image->myImage[i * 4];
	}

	//memcpy(&, &data, sizeof(u8) * (image->myWidth * image->myHeight));
	myHeightmap.myDepth = image->myHeight;
	myHeightmap.myWidth = image->myWidth;

	SAFE_DELETE(image);
	CreateVertices(aSize.x, aSize.y, position);
	mySurface = new Surface(myEffect);
	mySurface->AddTexture("Data/Textures/terrain.dds", Effect::DIFFUSE);
	//mySurface->AddTexture("Data/Textures/default_textures/no-texture-bw.dds", _ROUGHNESS);

	m_ClipEffect = Engine::GetInstance()->GetEffect("Shaders/terrain_clip.json");
	m_ClipEffect->AddShaderResource(Engine::GetInstance()->GetTexture("Data/Textures/terrain.dds"), Effect::DIFFUSE);

	m_HasLoaded = true;
	return true;
}

void Terrain::CleanUp()
{
	myIndexes.clear();
	myVertices.clear();

	SAFE_RELEASE(m_VertexLayout);

	SAFE_DELETE(mySurface);

	SAFE_RELEASE(m_ConstantBuffer);

}

void Terrain::Render(const CU::Matrix44f& aCameraOrientation, const CU::Matrix44f& aCameraProjection)
{
	SetupLayoutsAndBuffers();
 
	myEffect->Use();
 
	UpdateConstantBuffer(aCameraOrientation, aCameraProjection, render_context);
	render_context.m_Context->VSSetConstantBuffers(0, 1, &m_ConstantBuffer);
	render_context.m_API->SetSamplerState(eSamplerStates::LINEAR_WRAP);
 
	mySurface->Activate(render_context);
	render_context.m_Context->DrawIndexed(m_IndexData.myIndexCount, 0, 0);
	mySurface->Deactivate();

	myEffect->Clear();
}

void Terrain::Render(const CU::Matrix44f& aCameraOrientation, const CU::Matrix44f& aCameraProjection, bool override_shader)
{
	SetupLayoutsAndBuffers();

	m_ClipEffect->Use();

	UpdateConstantBuffer(aCameraOrientation, aCameraProjection, render_context);
	render_context.m_Context->VSSetConstantBuffers(0, 1, &m_ConstantBuffer);
	render_context.m_API->SetSamplerState(eSamplerStates::LINEAR_WRAP);

	if (!override_shader)
	{
		mySurface->Activate(render_context);
		render_context.m_Context->DrawIndexed(m_IndexData.myIndexCount, 0, 0);
		mySurface->Deactivate();
	}
	else
	{
		render_context.m_Context->DrawIndexed(m_IndexData.myIndexCount, 0, 0);
	}
	m_ClipEffect->Clear();
}

void Terrain::ShadowRender(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection)
{
	SetupLayoutsAndBuffers();

	UpdateConstantBuffer(camera_orientation, camera_projection, render_context);
	render_context.m_Context->VSSetConstantBuffers(0, 1, &m_ConstantBuffer);
	render_context.m_API->SetSamplerState(eSamplerStates::LINEAR_WRAP);

	render_context.m_Context->DrawIndexed(m_IndexData.myIndexCount, 0, 0);
}

void Terrain::Save(const std::string& /*aFilename*/)
{
	DL_ASSERT("Not implemented.");
}

void Terrain::Load(const std::string& /*aFilePath*/)
{
	DL_ASSERT("Not implemented.");
}

void Terrain::AddNormalMap(const std::string& filepath)
{
	mySurface->AddTexture(filepath, Effect::NORMAL);
}

std::vector<float> Terrain::GetVerticeArrayCopy()
{
	return myVertices;
}

std::vector<s32> Terrain::GetIndexArrayCopy()
{
	return myIndexes;
}

void Terrain::SetPosition(CU::Vector2f position)
{

}

void Terrain::CreateVertices(u32 width, u32 height, const CU::Vector3f& position)
{
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	myVertexFormat.ReInit(5);
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

	for (u32 z = 0; z < myHeightmap.myDepth - 1; ++z)
	{
		for (u32 x = 0; x < myHeightmap.myWidth - 1; ++x)
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


	//myVertexData = new VertexDataWrapper;

	m_VertexData.myNrOfVertexes = vertices.Size();
	m_VertexData.myStride = sizeof(SVertexPosNormUVBiTang);
	m_VertexData.mySize = m_VertexData.myNrOfVertexes * m_VertexData.myStride;
	m_VertexData.myVertexData = new char[m_VertexData.mySize]();
	memcpy(m_VertexData.myVertexData, &vertices[0], m_VertexData.mySize);


	//myIndexData = new VertexIndexWrapper;
	m_IndexData.myFormat = DXGI_FORMAT_R32_UINT;
	m_IndexData.myIndexCount = indexes.Size();
	m_IndexData.mySize = m_IndexData.myIndexCount * 4;

	m_IndexData.myIndexData = new char[m_IndexData.mySize];
	memcpy(m_IndexData.myIndexData, &indexes[0], m_IndexData.mySize);

	InitVertexBuffer();
	InitInputLayout();
	InitIndexBuffer();
	InitConstantBuffer();
}

void Terrain::UpdateConstantBuffer(const CU::Matrix44f& aCameraOrientation, const CU::Matrix44f& aCameraProjection)
{
	myConstantStruct.world = myOrientation;
	myConstantStruct.invertedView = CU::Math::Inverse(aCameraOrientation);
	myConstantStruct.projection = aCameraProjection;
	render_context.m_API->UpdateConstantBuffer((m_ConstantBuffer), &myConstantStruct);


}

void Terrain::InitConstantBuffer()
{
	D3D11_BUFFER_DESC cbDesc;
	ZeroMemory(&cbDesc, sizeof(cbDesc));
	cbDesc.ByteWidth = sizeof(VertexBaseStruct);
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	HRESULT hr = Engine::GetAPI()->GetDevice()->CreateBuffer(&cbDesc, 0, &m_ConstantBuffer);
	Engine::GetAPI()->SetDebugName(m_ConstantBuffer, "Model Constantbuffer : " + m_Filename);
	Engine::GetAPI()->HandleErrors(hr, "[Terrain] : Failed to Create Constantbuffer, ");
}

void Terrain::CalculateNormals(CU::GrowingArray<SVertexPosNormUVBiTang>& VertArray)
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

			VertArray[y * width + x].normal = normal;
		}
	}
}

float Terrain::GetHeight(unsigned int aX, unsigned int aY) const
{
	return myHeightmap.myData[(myHeightmap.myDepth - (1 + aY)) * myHeightmap.myWidth + aX] / DIVIDE;
}

float Terrain::GetHeight(unsigned int aIndex) const
{
	return myHeightmap.myData[aIndex] / DIVIDE;
}

SHeightMap Create(const char* aFilePath)
{
	TGA32::Image* image = TGA32::Load(aFilePath);

	u32 width = image->myWidth;
	u32 depth = image->myHeight;

	u8* data = new u8[width * depth];

	for (u32 i = 0; i < width * depth; ++i)
	{
		data[i] = image->myImage[i * 4];
	}

	SAFE_DELETE(image);

	SHeightMap height_map;
	height_map.myWidth = width;
	height_map.myDepth = depth;
	height_map.myData = data;

	return height_map;
}

