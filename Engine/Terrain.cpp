#include "stdafx.h"
#include "Terrain.h"
#include "TGA32.h"
#include <Engine/Effect.h>
#define DIVIDE 255.f



const char* GrayTile = "Data/Textures/GrayTile.dds";
const char* Britannia = "Data/Textures/terrain/britannia.dds";
const char* BritanniaNomrmal = "Data/Textures/terrain/brit_n.dds";
const char* Flat = "Data/Textures/flat_height.dds";
const char* Grass = "Data/Material/grass/grass1-albedo3.dds";

#define ALBEDO GrayTile
#define HEIGHTMAP Britannia

void Terrain::SetupTextures()
{
	Engine::GetInstance()->LoadTexture(HEIGHTMAP);
	Engine::GetInstance()->LoadTexture(ALBEDO);
	//Engine::GetInstance()->LoadTe


	m_Effect->AddShaderResource(Engine::GetInstance()->GetTexture(ALBEDO), TextureSlot::REGISTER_0);
	m_Effect->AddShaderResource(Engine::GetInstance()->GetTexture("Data/Material/grass/grass1-rough.dds"), TextureSlot::ROUGHNESS);
	m_Effect->AddShaderResource(Engine::GetInstance()->GetTexture(HEIGHTMAP), TextureSlot::REGISTER_7);
	m_Effect->AddShaderResource(Engine::GetInstance()->GetTexture(BritanniaNomrmal), TextureSlot::NORMAL);

	//m_Material = Engine::GetInstance()->GetMaterial("Data/Material/mat_grass.json");

	Effect* e = Engine::GetInstance()->GetEffect("Shaders/gpu_shadow.json");
	e->AddShaderResource(Engine::GetInstance()->GetTexture(HEIGHTMAP), TextureSlot::REGISTER_7);
}

Terrain::Terrain(float halfwidth, CU::Vector2f tex[4], CU::Vector3f color)
	: m_Color(color)
{
	m_Effect = Engine::GetInstance()->GetEffect("Data/Shaders/gpu_terrain.json");

	for (int i = 0; i < 4; i++)
		m_UV[i] = tex[i];

	myWidth = halfwidth / 2;
	myDepth = halfwidth / 2;

	CreatePlane(halfwidth);
	m_IsRoot = false;

	m_Buffer.RegisterVariable(&m_Orientation);
	m_Buffer.RegisterVariable(&Engine::GetInstance()->GetCamera()->GetOrientation());
	m_Buffer.Initiate();

	m_PixelBuffer.RegisterVariable(&m_Color);
	m_PixelBuffer.Initiate();

	SetupTextures();

}

Terrain::Terrain(float halfwidth, CU::Vector3f color )
{
	m_Effect = Engine::GetInstance()->GetEffect("Data/Shaders/gpu_terrain.json");

	myWidth = halfwidth / 2;
	myDepth = halfwidth / 2;

	CreatePlane(halfwidth);
	m_IsRoot = false;

	m_Buffer.RegisterVariable(&m_Orientation);
	m_Buffer.RegisterVariable(&Engine::GetInstance()->GetCamera()->GetOrientation());
	m_Buffer.Initiate();

	m_PixelBuffer.RegisterVariable(&m_Color);
	m_PixelBuffer.Initiate();

	SetupTextures();
}

bool Terrain::Initiate(const std::string& aFile, const CU::Vector3f position, const CU::Vector2f& aSize)
{
	myWidth = (u32)aSize.x;
	myDepth = (u32)aSize.y;
	//	m_Filename = "Terrain";
	m_IsRoot = false;
	m_Effect = Engine::GetInstance()->GetEffect("Shaders/terrain_base.json");

	
	CreateVertices(myWidth, myDepth, position);

	m_ClipEffect = Engine::GetInstance()->GetEffect("Shaders/terrain_clip.json");
	m_ClipEffect->AddShaderResource(Engine::GetInstance()->GetTexture("Data/Textures/terrain.dds"), TextureSlot::DIFFUSE);

	m_ConstantBuffer = Engine::GetAPI()->GetDevice().CreateConstantBuffer(sizeof(CU::Matrix44f), "Terrain ConstantBuffer");


	m_Buffer.RegisterVariable(&m_Orientation);
	m_Buffer.RegisterVariable(&Engine::GetInstance()->GetCamera()->GetOrientation());
	m_Buffer.Initiate();

	m_HasLoaded = true;
	return true;
}

void Terrain::CleanUp()
{
	myIndexes.clear();
	myVertices.clear();
	//Engine::GetAPI()->GetDevice().ReleasePtr(m_ConstantBuffer);

}

void Terrain::Render(const graphics::RenderContext& rc)
{
	// 	if (!m_Material)
	// 		return;

	graphics::IGraphicsContext& ctx = rc.GetContext();
	graphics::IGraphicsAPI& api = rc.GetAPI();

	ctx.SetDepthState(graphics::Z_ENABLED, 1);
	ctx.SetBlendState(graphics::BLEND_FALSE);

	ctx.SetRasterState(graphics::CULL_NONE);

	//UpdateConstantBuffer(rc);
	m_Buffer.Bind(1, graphics::ConstantBuffer::VERTEX | graphics::ConstantBuffer::DOMAINS, rc);
	m_PixelBuffer.Bind(1, graphics::ConstantBuffer::PIXEL, rc);

	rc.GetContext().VSSetSamplerState(0, 1, graphics::POINT_CLAMP);
	rc.GetContext().PSSetSamplerState(0, 1, graphics::POINT_CLAMP);

	rc.GetContext().DSSetSamplerState(0, 1, graphics::POINT_CLAMP);
	rc.GetContext().DSSetSamplerState(1, 1, graphics::LINEAR_CLAMP);

	//mySurface->Activate(rc);
	//ctx.DrawIndexed(this, m_Effect);


	//ctx.SetRasterizerState(api.GetRasterizerState(graphics::WIREFRAME));
	//m_Material->Use(m_Effect);
	ctx.DrawIndexed(this, m_Effect);
	//mySurface->Deactivate();
}

void Terrain::Render(const graphics::RenderContext& rc, bool override_shader)
{

	graphics::IGraphicsContext& ctx = rc.GetContext();
	//UpdateConstantBuffer(rc);
	//ctx.PSSetSamplerState(0, 1, rc.GetEngine().GetActiveSampler());

	if (!override_shader)
	{
		ctx.DrawIndexed(this);
	}
	else
	{
		ctx.DrawIndexed(this, m_ClipEffect);
	}

}

void Terrain::Render(const graphics::RenderContext& rc, bool, bool)
{
	graphics::IGraphicsContext& ctx = rc.GetContext();

	ctx.SetDepthState(graphics::Z_ENABLED, 1);
	ctx.SetBlendState(graphics::BLEND_FALSE);

	ctx.SetRasterState(graphics::CULL_NONE);
	
	m_PixelBuffer.Bind(1, graphics::ConstantBuffer::PIXEL, rc);

	rc.GetContext().PSSetSamplerState(0, 1, graphics::MSAA_x16);
	rc.GetContext().VSSetSamplerState(0, 1, graphics::MSAA_x16);
	rc.GetContext().DSSetSamplerState(0, 1, graphics::MSAA_x16);
	ctx.DrawIndexed(this, m_Effect);
}

void Terrain::Wireframe(const graphics::RenderContext& rc)
{
	graphics::IGraphicsContext& ctx = rc.GetContext();

	graphics::IGraphicsAPI& api = rc.GetAPI();

	ctx.SetDepthState(graphics::Z_ENABLED, 1);
	ctx.SetBlendState(graphics::BLEND_FALSE);

	CU::Vector4f translation = m_Orientation.GetTranslation();
	const float offset = 0.09f;
	translation.y += offset;
	m_Orientation.SetTranslation(translation);

	m_Buffer.Bind(1, graphics::ConstantBuffer::VERTEX | graphics::ConstantBuffer::DOMAINS, rc);
	m_PixelBuffer.Bind(1, graphics::ConstantBuffer::PIXEL, rc);

	translation.y -= offset;
	m_Orientation.SetTranslation(translation);

	rc.GetContext().PSSetSamplerState(0, 1, graphics::MSAA_x16);
	rc.GetContext().VSSetSamplerState(0, 1, graphics::MSAA_x16);
	ctx.SetRasterState(graphics::WIREFRAME);

	ctx.DrawIndexed(this, rc.GetEngine().GetEffect("Data/Shaders/wireframe_terrain.json"));
}

void Terrain::ShadowRender(const graphics::RenderContext& rc)
{
	graphics::IGraphicsContext& ctx = rc.GetContext();
	graphics::IGraphicsAPI& api = rc.GetAPI();

	ctx.SetDepthState(graphics::Z_ENABLED, 1);
	ctx.SetBlendState(graphics::BLEND_FALSE);
	ctx.SetRasterState(graphics::CULL_FRONT);

	m_Buffer.Bind(1, graphics::ConstantBuffer::VERTEX | graphics::ConstantBuffer::DOMAINS, rc);

	rc.GetContext().DSSetSamplerState(0, 1, graphics::MSAA_x16);
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
	m_Orientation.SetPosition(CU::Vector4f(position.x - myWidth, 0, position.y - myDepth, 1));
}

void Terrain::CreateVertices(u32 width, u32 height, const CU::Vector3f& position)
{


	CU::GrowingArray<SVertexPosNormUVBiTang> vertices((myHeightmap.myDepth)*(myHeightmap.myWidth));

	for (u32 z = 0; z < myHeightmap.myDepth; z++)
	{
		for (u32 x = 0; x < myHeightmap.myWidth; x++)
		{
			SVertexPosNormUVBiTang vertex;
			vertex.position.x = position.x + float(x) * width / float(myHeightmap.myWidth);
			vertex.position.y = position.y + myHeightmap.myData[(myHeightmap.myDepth - (1 + z)) * myHeightmap.myWidth + x]; //* 128.f / DIVIDE;
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

	CU::GrowingArray<u32> indexes((myHeightmap.myDepth *  myHeightmap.myWidth) * 6);
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

	constexpr s32 vtx_stride = sizeof(SVertexPosNormUVBiTang);
	const s32 vtx_count = vertices.Size();
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

	IBuffer* vtx_buffer = device.CreateBuffer(vtx_desc, "Terrain VertexBuffer");
	m_VertexWrapper.SetData(vtx_data);
	m_VertexWrapper.SetStart(vtx_start);
	m_VertexWrapper.SetStride(vtx_stride);
	m_VertexWrapper.SetByteOffset(vtx_byte_offset);
	m_VertexWrapper.SetVertexCount(vtx_count);
	m_VertexWrapper.SetSize(vtx_size);
	m_VertexWrapper.SetBuffer(vtx_buffer);
	m_VertexWrapper.SetInputLayout(pInputLayout);
	m_VertexWrapper.SetTopology(graphics::TRIANGLE_LIST);


#ifdef _DEBUG
	m_VertexWrapper.m_DebugName = DEBUG_NAME("particle_emitter", Terrain);
#endif
	const s32 idx_count = indexes.Size();
	const s32 idx_stride = sizeof(u32);
	const s32 idx_size = idx_count * idx_stride;
	const s32 idx_start = 0;
	const s32 idx_byte_offset = 0;

	s8* idx_data = new s8[idx_size];
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
	IBuffer* idx_buffer = Engine::GetAPI()->GetDevice().CreateBuffer(idx_desc, "Terrain IndexBuffer");

	m_IndexWrapper.SetData(idx_data);
	m_IndexWrapper.SetIndexCount(idx_count);
	m_IndexWrapper.SetStart(idx_start);
	m_IndexWrapper.SetSize(idx_size);
	m_IndexWrapper.SetFormat(graphics::R32_UINT);
	m_IndexWrapper.SetByteOffset(idx_byte_offset);
	m_IndexWrapper.SetBuffer(idx_buffer);

#ifdef _DEBUG
	m_IndexWrapper.m_DebugName = DEBUG_NAME("TerrainINdex", Terrain);
#endif
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
	return myHeightmap.myData[(myHeightmap.myDepth - (1 + aY)) * myHeightmap.myWidth + aX];
}

float Terrain::GetHeight(unsigned int aIndex) const
{
	return myHeightmap.myData[aIndex];
}

void Terrain::CreatePlane(float halfwidth)
{
	PROFILE_FUNCTION(profiler::colors::Green);

	auto& device = Engine::GetAPI()->GetDevice();
	graphics::InputElementDesc desc[] =
	{
		{ "POSITION", 0, graphics::_16BYTE_RGBA, 0, 0, graphics::INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, graphics::_12BYTE_RGB, 0, 16, graphics::INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, graphics::_8BYTE_RG, 0, 28, graphics::INPUT_PER_VERTEX_DATA, 0 },
		{ "BINORMAL", 0, graphics::_12BYTE_RGB, 0, 36, graphics::INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, graphics::_12BYTE_RGB, 0, 48, graphics::INPUT_PER_VERTEX_DATA, 0 },
	};


	CU::GrowingArray<SVertexPosNormUVBiTang> m_Vertices;

	SVertexPosNormUVBiTang vert;
	vert.position = { -halfwidth, 0, halfwidth, 1 };
	vert.normal = { 0, 1, 0 };
	vert.uv = { m_UV[0].x, m_UV[0].y };
	m_Vertices.Add(vert);

	vert.position = { -halfwidth, 0, -halfwidth, 1 };
	vert.normal = { 0, 1, 0 };
	vert.uv = { m_UV[1].x, m_UV[1].y };

	m_Vertices.Add(vert);

	vert.position = { halfwidth, 0, halfwidth, 1 };
	vert.normal = { 0, 1, 0 };
	vert.uv = { m_UV[2].x, m_UV[2].y };
	m_Vertices.Add(vert);

	vert.position = { halfwidth, 0, -halfwidth, 1 };
	vert.normal = { 0, 1, 0 };
	vert.uv = { m_UV[3].x, m_UV[3].y };
	m_Vertices.Add(vert);

	const s32 vtx_stride = sizeof(SVertexPosNormUVBiTang);
	const s32 vtx_count = m_Vertices.Size();
	const s32 vtx_size = vtx_count * vtx_stride;
	const s32 vtx_buff_count = 1;
	const s32 vtx_start = 0;
	const s32 vtx_byte_offset = 0;
	s8* vtx_data = new s8[vtx_size];
	memcpy(vtx_data, &m_Vertices[0], vtx_size);


	graphics::BufferDesc vtx_desc;
	vtx_desc.m_Size = vtx_size;
	vtx_desc.m_Data = vtx_data;
	vtx_desc.m_BindFlag = graphics::BIND_VERTEX_BUFFER;
	vtx_desc.m_UsageFlag = graphics::DYNAMIC_USAGE;
	vtx_desc.m_CPUAccessFlag = graphics::WRITE;
	vtx_desc.m_ByteWidth = vtx_size;

	char temp[100];
	sprintf_s(temp, 100, "%s Vertex Buffer", typeid(this).name());
	IBuffer* vtx_buffer = device.CreateBuffer(vtx_desc, temp);


	
	m_VertexWrapper.SetData(vtx_data);
	m_VertexWrapper.SetStart(vtx_start);
	m_VertexWrapper.SetStride(vtx_stride);
	m_VertexWrapper.SetByteOffset(vtx_byte_offset);
	m_VertexWrapper.SetVertexCount(vtx_count);
	m_VertexWrapper.SetSize(vtx_size);
	m_VertexWrapper.SetBuffer(vtx_buffer);
	m_VertexWrapper.SetInputLayout(device.CreateInputLayout(m_Effect->GetVertexShader(), desc, ARRSIZE(desc)));
	m_VertexWrapper.SetTopology(graphics::_4_CONTROL_POINT_PATCHLIST);

	CU::GrowingArray<u32> indices;
	indices.Add(0);
	indices.Add(1);
	indices.Add(2);

	indices.Add(3);
	indices.Add(2);
	indices.Add(1);

	const s32 idx_count = indices.Size();
	const s32 idx_stride = sizeof(u32);
	const s32 idx_size = idx_count * idx_stride;
	const s32 idx_start = 0;
	const s32 idx_byte_offset = 0;

	s8* idx_data = new s8[idx_size];
	memcpy(idx_data, &indices[0], idx_size);

	graphics::BufferDesc idx_desc;
	idx_desc.m_Size = idx_size;
	idx_desc.m_Data = idx_data;
	idx_desc.m_BindFlag = graphics::BIND_INDEX_BUFFER;
	idx_desc.m_UsageFlag = graphics::IMMUTABLE_USAGE;
	idx_desc.m_StructuredByteStride = 0;
	idx_desc.m_CPUAccessFlag = graphics::NO_ACCESS_FLAG;
	idx_desc.m_MiscFlags = 0;
	idx_desc.m_ByteWidth = idx_desc.m_Size;

	sprintf_s(temp, 100, "%s Index Buffer", typeid(this).name());
	IBuffer* idx_buffer = Engine::GetAPI()->GetDevice().CreateBuffer(idx_desc, temp);

	m_IndexWrapper.SetData(idx_data);
	m_IndexWrapper.SetIndexCount(idx_count);
	m_IndexWrapper.SetStart(idx_start);
	m_IndexWrapper.SetSize(idx_size);
	m_IndexWrapper.SetFormat(graphics::R32_UINT);
	m_IndexWrapper.SetByteOffset(idx_byte_offset);
	m_IndexWrapper.SetBuffer(idx_buffer);
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

