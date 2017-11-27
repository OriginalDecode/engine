#include "stdafx.h"
#include "EmitterInstance.h"
#include "AssetsContainer.h"
#include <Randomizer.h>
#include "VertexStructs.h"

CEmitterInstance::~CEmitterInstance()
{
	Engine::GetAPI()->ReleasePtr(m_ConstantBuffer);
	Engine::GetAPI()->ReleasePtr(m_GeometryBuffer);
}

void CEmitterInstance::Initiate(Synchronizer* aSynchronizer, Texture* depth_texture)
{
	m_Synchronizer = aSynchronizer;
	SParticleData data;
	data.affectedByGravity = false;
	data.lifeTime = 3.f;
	data.minStartSize = 1.f;
	data.maxStartSize = 1.f;
	data.speed = 1.f;
	data.startAlpha = 0.2f;
	data.sizeDelta = 0.f;
	data.alphaDelta = 0.f;

	myData.diffuseTexture = Engine::GetInstance()->GetTexture("Data/Textures/particles/smoke.dds");
	myData.normalTexture = Engine::GetInstance()->GetTexture("Data/Textures/particles/smoke_normal.dds");

	myData.lifeTime = -1.f;
	myData.shader = Engine::GetInstance()->GetEffect("Shaders/particle.json");
	myData.particleData = data;
	myData.size = { 100.f, 10.f, 0.f };

	myParticles.Init(256);

	for (int i = 0; i < myParticles.Capacity(); i++)
	{
		SParticleObject toAdd;
		myParticles.Add(toAdd);
	}

	CreateBuffer();
	myData.shader->AddShaderResource(myData.diffuseTexture->GetShaderView(), Effect::DIFFUSE);
	myData.shader->AddShaderResource(myData.normalTexture->GetShaderView(), Effect::NORMAL);

// 	Effect* shader = myEngine->GetEffect("Shaders/particle_offscreen.json");
// 	shader->AddShaderResource(myData.diffuseTexture->GetShaderView(), Effect::DIFFUSE);
// 	shader->AddShaderResource(myData.normalTexture->GetShaderView(), Effect::NORMAL);



	myTimeToEmit = 0.f;
}

void CEmitterInstance::Update(float aDeltaTime)
{
	myTimeToEmit -= aDeltaTime;
	if (myTimeToEmit < 0.f)
	{
		Emit();
		myTimeToEmit = 0.2f;
	}

	UpdateParticle(aDeltaTime);
}

void CEmitterInstance::Render(CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection, Effect* effect)
{
	UpdateVertexBuffer();
	UpdateConstantBuffer(camera_orientation, camera_projection);
	Engine::GetAPI()->GetContext().Draw(this);
}

void CEmitterInstance::RenderShadowed(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection)
{
	Engine::GetAPI()->GetContext().Draw(this);
}

void CEmitterInstance::SetPosition(const CU::Vector3f& position)
{
	myOrientation.SetPosition(position);
}

void CEmitterInstance::CreateBuffer()
{
	graphics::InputElementDesc layout[] =
	{
		{ "POSITION", 0, graphics::_16BYTE_RGBA, 0, 0, graphics::INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, graphics::_16BYTE_RGBA, 0, 16, graphics::INPUT_PER_VERTEX_DATA, 0 },
		{ "ALPHA", 0, graphics::_4BYTE_R_FLOAT, 0, 32, graphics::INPUT_PER_VERTEX_DATA, 0 },
		{ "SIZE", 0, graphics::_4BYTE_R_FLOAT, 0, 36, graphics::INPUT_PER_VERTEX_DATA, 0 },
	};

	auto& device = Engine::GetAPI()->GetDevice();

	IInputLayout* pLayout = device.CreateInputLayout(myData.shader->GetVertexShader(), layout, ARRSIZE(layout));

	const s32 vtx_stride = sizeof(SParticleObject);
	const s32 vtx_start_slot = 0;
	const s32 vtx_byte_offset = 0;
	const s32 vtx_buffer_count = 1;
	const s32 vtx_byte_width = vtx_stride * myParticles.Capacity();
	const s32 vtx_count = myParticles.Capacity();

	graphics::BufferDesc vtx_buff_desc;
	vtx_buff_desc.m_BindFlag = graphics::BIND_VERTEX_BUFFER;
	vtx_buff_desc.m_UsageFlag = graphics::DYNAMIC_USAGE;
	vtx_buff_desc.m_CPUAccessFlag = graphics::WRITE;
	vtx_buff_desc.m_ByteWidth = vtx_byte_width;

	IBuffer* vtx_buffer =  device.CreateBuffer(vtx_buff_desc, "EmitterInstance Buffer"); 

	m_VertexWrapper.SetStart(vtx_start_slot);
	m_VertexWrapper.SetBuffer(vtx_buffer);
	m_VertexWrapper.SetStride(vtx_stride);
	m_VertexWrapper.SetByteOffset(vtx_byte_offset);
	m_VertexWrapper.SetVertexCount(vtx_count);
	m_VertexWrapper.SetInputLayout(pLayout);
	m_VertexWrapper.SetSize(vtx_byte_width);
	m_VertexWrapper.SetTopology(graphics::POINT_LIST);
	

#ifdef _DEBUG
	m_VertexWrapper.m_DebugName = DEBUG_NAME("particle", CEmitterInstance);
#endif

	m_ConstantBuffer = device.CreateConstantBuffer(sizeof(cbParticleVertex), "EmitterInstance ConstantBuffer");
	m_GeometryBuffer = device.CreateConstantBuffer(sizeof(cbParticleGeometry), "EmitterInstance GeometryConstBuffer");

}

void CEmitterInstance::UpdateVertexBuffer()
{
	if (myParticles.Empty())
		return;

	m_VertexWrapper.SetVertexCount(myParticles.Size());
	IBuffer* pBuffer = m_VertexWrapper.GetVertexBuffer();
	Engine::GetAPI()->GetContext().UpdateConstantBuffer(pBuffer, &myParticles[0]);
}

void CEmitterInstance::UpdateConstantBuffer(CU::Matrix44f& aCameraOrientation, const CU::Matrix44f& aCameraProjection)
{
	auto& ctx = Engine::GetAPI()->GetContext();
	m_VertexCB.m_World = CU::Matrix44f();
	m_VertexCB.m_View = CU::Math::Inverse(aCameraOrientation);
	ctx.UpdateConstantBuffer(m_ConstantBuffer, &m_VertexCB);

	m_GeometryCB.m_Projection = aCameraProjection;
	ctx.UpdateConstantBuffer(m_GeometryBuffer, &m_GeometryCB);



	//ctx.VSSetConstantBuffer(0, 1, &m_ConstantBuffer);
	//ctx.GSSetConstantBuffer(0, 1, &m_GeometryBuffer);
	ctx.PSSetConstantBuffer(0, 1, &m_GeometryBuffer);

}

void CEmitterInstance::UpdateParticle(float aDeltaTime)
{
	for (int i = 0; i < myParticles.Size(); i++)
	{
		if (myParticles[i].currLifeTime < 0.f || myParticles[i].alpha < 0.f)
		{
			myParticles.RemoveCyclicAtIndex(i);
			continue;
		}
		//myParticles[i].position += (myParticles[i].direction * myParticles[i].speed) * aDeltaTime;
		myParticles[i].currLifeTime -= aDeltaTime;
		//myParticles[i].size += (1 * aDeltaTime);
		//myParticles[i].alpha = myParticles[i].currLifeTime / myParticles[i].lifeTime;
	}
}

void CEmitterInstance::Emit()
{
	SParticleObject temp; //Replace with preallocated particles

	temp.position = myOrientation.GetPosition();
	float x0 = temp.position.x;
	float y0 = temp.position.y;
	float z0 = temp.position.z;

	float x1 = temp.position.x + myData.size.x;
	float y1 = temp.position.y + myData.size.y;
	float z1 = temp.position.z + myData.size.z;

	temp.position.x = RANDOM(x0, x1);
	temp.position.y = RANDOM(y0, y1);
	temp.position.z = RANDOM(z0, z1);

	temp.size = RANDOM(1.f, 1.f);
	temp.direction.x = RANDOM(-0.15f, 0.15f);
	temp.direction.y = RANDOM(0.f, 1.f);
	temp.direction.z = RANDOM(-0.15f, 0.15f);
	const float time = 20.f;
	temp.lifeTime = time;
	temp.currLifeTime = time;
	temp.alpha = 1;// myData.particleData.startAlpha;
	temp.speed = 3.f;
	myParticles.Add(temp);
}
