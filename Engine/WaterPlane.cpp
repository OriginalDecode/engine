#include "stdafx.h"
#include "WaterPlane.h"
#include <Engine/Texture.h>
#include <Engine/IGraphicsDevice.h>
#include <Engine/IGraphicsContext.h>


WaterPlane::WaterPlane()
{
	m_Effect = Engine::GetInstance()->GetEffect("Shaders/water.json");


	const WindowSize& window_size = Engine::GetInstance()->GetInnerSize();
	const float window_width = window_size.m_Width;
	const float window_height = window_size.m_Height;


	TextureDesc desc;
	desc.m_Width = window_width;
	desc.m_Height = window_height;
	desc.m_Usage = graphics::DEFAULT_USAGE;
	desc.m_ResourceTypeBinding = graphics::BIND_SHADER_RESOURCE | graphics::BIND_RENDER_TARGET;
	desc.m_ShaderResourceFormat = graphics::RGBA16_FLOAT;
	desc.m_RenderTargetFormat = graphics::RGBA16_FLOAT;

	m_Refraction = new Texture;
	m_Refraction->Initiate(desc, "Water : RefractionDepth");

	m_Reflection = new Texture;
	m_Reflection->Initiate(desc, "Water : ReflectionDepth");

	m_Effect->AddShaderResource(m_RefractionG.GetDiffuse(), Effect::REFRACTION);
	m_Effect->AddShaderResource(m_ReflectionG.GetDiffuse(), Effect::REFLECTION);

	Engine* engine = Engine::GetInstance();

	m_Effect->AddShaderResource(engine->GetTexture("Data/Textures/T_cubemap_level01.dds"), Effect::CUBEMAP);
	m_Effect->AddShaderResource(engine->GetTexture("Data/Textures/water_normal.dds"), Effect::NORMAL);
	m_Effect->AddShaderResource(engine->GetTexture("Data/Textures/water_dudv.dds"), Effect::DUDV);
	CreatePlane();

}

WaterPlane::~WaterPlane()
{
	SAFE_DELETE(m_Reflection);
	SAFE_DELETE(m_Refraction);
	Engine::GetAPI()->ReleasePtr(m_cbPixel);

}

void WaterPlane::SetPosition(const CU::Vector3f& position)
{
	m_Orientation.SetPosition(position);
}

void WaterPlane::UpdateConstantBuffer(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection, const graphics::RenderContext& render_context)
{
	m_VertexMatrices.m_World = m_Orientation;
	m_VertexMatrices.m_InvertedView = CU::Math::Inverse(camera_orientation);
	m_VertexMatrices.m_Projection = camera_projection;
	m_VertexMatrices.m_CameraPos = camera_orientation.GetPosition();
	m_VertexMatrices.m_Time = Engine::GetInstance()->GetTotalTime();
	render_context.GetContext().UpdateConstantBuffer(m_ConstantBuffer, &m_VertexMatrices, sizeof(cbMatrices));
}

void WaterPlane::Render(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection, const graphics::RenderContext& render_context)
{
	auto& ctx = render_context.GetContext();
	auto& api = render_context.GetAPI();

	ctx.SetDepthState(api.GetDepthStencilState(graphics::Z_ENABLED), 1);
	ctx.SetBlendState(api.GetBlendState(graphics::BLEND_FALSE));
	ctx.SetRasterizerState(m_RenderWireframe ? api.GetRasterizerState(graphics::WIREFRAME) : api.GetRasterizerState(graphics::CULL_NONE));

	UpdateConstantBuffer(camera_orientation, camera_projection, render_context);
	ctx.VSSetConstantBuffer(0, 1, &m_ConstantBuffer);
	ctx.DSSetConstantBuffer(0, 1, &m_ConstantBuffer);
	ctx.DrawIndexed(this, m_Effect);
}

void WaterPlane::ShadowRender(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection, const graphics::RenderContext& render_context)
{
	DL_ASSERT("water shadow?");
}

void WaterPlane::SetupRefractionRender(const graphics::RenderContext& render_context)
{
	m_RefractionG.Clear(clearcolor::black, render_context);
	render_context.GetContext().ClearDepthStencilView(m_Refraction->GetDepthView(), graphics::DEPTH | graphics::STENCIL, 1.0f);
	m_RefractionG.SetAsRenderTarget(m_Refraction, render_context);
}

void WaterPlane::SetupReflectionRender(const graphics::RenderContext& render_context)
{
	m_ReflectionG.Clear(clearcolor::black, render_context);
	render_context.GetContext().ClearDepthStencilView(m_Reflection->GetDepthView(), graphics::DEPTH | graphics::STENCIL, 1.0f);
	m_ReflectionG.SetAsRenderTarget(m_Reflection, render_context);
}

void WaterPlane::SetClipPlane(const CU::Vector4f& plane, const graphics::RenderContext& render_context)
{
	m_PixelStruct.m_CompareValue = plane;
	render_context.GetContext().UpdateConstantBuffer(m_cbPixel, &m_PixelStruct, sizeof(m_PixelStruct));
	render_context.GetContext().PSSetConstantBuffer(0, 1, m_cbPixel);
}

void WaterPlane::CreatePlane()
{
	auto& ctx = Engine::GetAPI()->GetContext();
	auto& device = Engine::GetAPI()->GetDevice();

	graphics::InputElementDesc desc[] =
	{
		{ "POSITION", 0, graphics::_16BYTE_RGBA, 0, 0, graphics::INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, graphics::_12BYTE_RGB, 0, 16, graphics::INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, graphics::_8BYTE_RG, 0, 28, graphics::INPUT_PER_VERTEX_DATA, 0 },
		{ "BINORMAL", 0, graphics::_12BYTE_RGB, 0, 36, graphics::INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, graphics::_12BYTE_RGB, 0, 48, graphics::INPUT_PER_VERTEX_DATA, 0 },
	};

	IInputLayout* pInputLayout = device.CreateInputLayout(m_Effect->GetVertexShader(), desc, ARRSIZE(desc));
	
	const float half_width = 2048.f;
	CU::GrowingArray<SVertexPosNormUVBiTang> m_Vertices;

	SVertexPosNormUVBiTang vert;
	vert.position = { -half_width, 0, -half_width };
	vert.normal = { 0, 1, 0 };
	vert.uv = { 0, 0 };
	m_Vertices.Add(vert);

	vert.position = { -half_width, 0, half_width };
	vert.normal = { 0, 1, 0 };
	vert.uv = { 0, 1 };

	m_Vertices.Add(vert);

	vert.position = { half_width, 0, -half_width };
	vert.normal = { 0, 1, 0 };
	vert.uv = { 1, 0 };
	m_Vertices.Add(vert);

	vert.position = { half_width, 0, half_width };
	vert.normal = { 0, 1, 0 };
	vert.uv = { 1, 1 };
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


	CU::GrowingArray<int> indices;
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

	s8* idx_data = new s8[idx_count];
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
	IBuffer* idx_buffer = Engine::GetAPI()->GetDevice().CreateBuffer(idx_desc);

	m_IndexWrapper = IndexWrapper(idx_data, 
								  idx_count, 
								  idx_start, 
								  idx_size, 
								  graphics::R32_UINT, 
								  idx_byte_offset, 
								  idx_buffer);






	m_ConstantBuffer = device.CreateConstantBuffer(sizeof(cbMatrices));
	m_cbPixel = device.CreateConstantBuffer(sizeof(cbPixel));


}