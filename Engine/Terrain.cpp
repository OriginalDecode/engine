#include "stdafx.h"
#include "Terrain.h"
#include "TGA32.h"
#include "Surface.h"
#define DIVIDE 255.f


bool Terrain::Initiate(const std::string& aFile, const CU::Vector3f position, const CU::Vector2f& aSize)
{
	myWidth = aSize.x;
	myDepth = aSize.y;
//	m_Filename = "Terrain";
	m_IsRoot = false;
	m_Effect = Engine::GetInstance()->GetEffect("Shaders/terrain_base.json");

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
	mySurface = new Surface(m_Effect);
	mySurface->AddTexture("Data/Textures/terrain.dds", Effect::DIFFUSE);
	//mySurface->AddTexture("Data/Textures/default_textures/no-texture-bw.dds", _ROUGHNESS);

	m_ClipEffect = Engine::GetInstance()->GetEffect("Shaders/terrain_clip.json");
	m_ClipEffect->AddShaderResource(Engine::GetInstance()->GetTexture("Data/Textures/terrain.dds"), Effect::DIFFUSE);

	m_ConstantBuffer = Engine::GetAPI()->GetDevice().CreateConstantBuffer(sizeof(VertexBaseStruct));

	m_HasLoaded = true;
	return true;
}

void Terrain::CleanUp()
{
	myIndexes.clear();
	myVertices.clear();
	SAFE_DELETE(mySurface);
	//Engine::GetAPI()->GetDevice().ReleasePtr(m_ConstantBuffer);

}

void Terrain::Render(const CU::Matrix44f& aCameraOrientation, const CU::Matrix44f& aCameraProjection, const graphics::RenderContext& render_context)
{
	graphics::IGraphicsContext& ctx = render_context.GetContext();
	UpdateConstantBuffer(aCameraOrientation, aCameraProjection, render_context);
	ctx.PSSetSamplerState(0, 1, render_context.GetEngine().GetActiveSampler());
 
	mySurface->Activate(render_context);
	ctx.DrawIndexed(this);
	mySurface->Deactivate();

}

//For water????
void Terrain::Render(const CU::Matrix44f& aCameraOrientation, const CU::Matrix44f& aCameraProjection, const graphics::RenderContext& render_context, bool override_shader)
{

	graphics::IGraphicsContext& ctx = render_context.GetContext();
	UpdateConstantBuffer(aCameraOrientation, aCameraProjection, render_context);
	ctx.PSSetSamplerState(0, 1, render_context.GetEngine().GetActiveSampler());

	if (!override_shader)
	{
		mySurface->Activate(render_context);
		ctx.DrawIndexed(this);
		mySurface->Deactivate();
	}
	else
	{
		ctx.DrawIndexed(this, m_ClipEffect);
	}

}

void Terrain::ShadowRender(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection, const graphics::RenderContext& render_context)
{
	graphics::IGraphicsContext& ctx = render_context.GetContext();
	UpdateConstantBuffer(camera_orientation, camera_projection, render_context);
	ctx.PSSetSamplerState(0, 1, render_context.GetEngine().GetActiveSampler());
	ctx.DrawIndexed(this);
}

void Terrain::Save(const std::string& /*aFilename*/)
{
	DL_ASSERT("Not implemented.");
}

void Terrain::Load(const std::string& /*filepath*/)
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


	CU::GrowingArray<SVertexPosNormUVBiTang> vertices;

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

	CU::GrowingArray<u32> indexes;
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

	graphics::IGraphicsDevice& device = Engine::GetAPI()->GetDevice();

	graphics::InputElementDesc inputdesc[] =
	{
		{ "POSITION", 0, graphics::_12BYTE_RGB, 0, 0, graphics::INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, graphics::_12BYTE_RGB, 0, 12, graphics::INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, graphics::_8BYTE_RG, 0, 24, graphics::INPUT_PER_VERTEX_DATA, 0 },
		{ "BINORMAL", 0, graphics::_12BYTE_RGB, 0, 28, graphics::INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, graphics::_12BYTE_RGB, 0, 40, graphics::INPUT_PER_VERTEX_DATA, 0 },
	};
	IInputLayout* pInputLayout = device.CreateInputLayout(m_Effect->GetVertexShader(), inputdesc, ARRSIZE(inputdesc));

	const s32 vtx_stride = sizeof(SVertexPosNormUVBiTang);
	const s32 vtx_count = myVertices.size();
	const s32 vtx_size = vtx_count * vtx_stride;
	const s32 vtx_buff_count = 1;
	const s32 vtx_start = 0;
	const s32 vtx_byte_offset = 0;
	s8* vtx_data = new s8[vtx_size];
	memcpy(vtx_data, &vertices[0], vtx_size);

	graphics::BufferDesc vtx_desc;
	vtx_desc.m_Size = vtx_size;
	vtx_desc.m_Data = vtx_data;
	vtx_desc.m_BindFlag = graphics::BIND_VERTEX_BUFFER;
	vtx_desc.m_UsageFlag = graphics::DYNAMIC_USAGE;
	vtx_desc.m_CPUAccessFlag = graphics::WRITE;
	vtx_desc.m_ByteWidth = vtx_size;

	IBuffer* vtx_buffer = device.CreateBuffer(vtx_desc);
	m_VertexWrapper = VertexWrapper(vtx_data,
									vtx_start,
									vtx_buff_count,
									vtx_stride,
									vtx_byte_offset,
									vtx_count,
									vtx_size,
									vtx_buffer,
									pInputLayout,
									graphics::TRIANGLE_LIST);

	const s32 idx_count = indexes.Size();
	const s32 idx_stride = sizeof(u32);
	const s32 idx_size = idx_count * idx_stride;
	const s32 idx_start = 0;
	const s32 idx_byte_offset = 0;

	s8* idx_data = new s8[idx_count];
	memcpy(idx_data, &indexes[0], idx_size);

	graphics::BufferDesc idx_desc;
	idx_desc.m_Size = idx_size;
	idx_desc.m_Data = idx_data;
	idx_desc.m_BindFlag = graphics::BIND_INDEX_BUFFER;
	idx_desc.m_UsageFlag = graphics::IMMUTABLE_USAGE;
	idx_desc.m_StructuredByteStride = 0;
	idx_desc.m_CPUAccessFlag = graphics::NO_ACCESS_FLAG;
	idx_desc.m_MiscFlags = 0;
	idx_desc.m_ByteWidth = idx_desc.m_Size;
	IBuffer* idx_buffer = Engine::GetAPI()->GetDevice().CreateBuffer(idx_desc);

	m_IndexWrapper = IndexWrapper(idx_data,
								  idx_count,
								  idx_start,
								  idx_size,
								  graphics::R32_UINT,
								  idx_byte_offset,
								  idx_buffer);
}

void Terrain::UpdateConstantBuffer(const CU::Matrix44f& aCameraOrientation, const CU::Matrix44f& aCameraProjection, const graphics::RenderContext& render_context)
{
	graphics::IGraphicsContext& ctx = render_context.GetContext();
	myConstantStruct.world = myOrientation;
	myConstantStruct.invertedView = CU::Math::Inverse(aCameraOrientation);
	myConstantStruct.projection = aCameraProjection;
	ctx.UpdateConstantBuffer(m_ConstantBuffer, &myConstantStruct, sizeof(VertexBaseStruct));
	ctx.VSSetConstantBuffer(0, 1, &m_ConstantBuffer);
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

SHeightMap Create(const char* filepath)
{
	TGA32::Image* image = TGA32::Load(filepath);

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

