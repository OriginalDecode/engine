#include "stdafx.h"
#include "EmitterInstance.h"
#include "AssetsContainer.h"
#include <Randomizer.h>
#include "VertexStructs.h"
void CEmitterInstance::Initiate(Synchronizer* aSynchronizer, Texture* depth_texture)
{
	myEngine = Engine::GetInstance();
	mySynchronizer = aSynchronizer;
	SParticleData data;
	data.affectedByGravity = false;
	data.lifeTime = 3.f;
	data.minStartSize = 1.f;
	data.maxStartSize = 1.f;
	data.speed = 1.f;
	data.startAlpha = 0.2f;
	data.sizeDelta = 0.f;
	data.alphaDelta = 0.f;

	myData.diffuseTexture = myEngine->GetTexture("Data/Textures/particles/smoke.dds");
	myData.normalTexture = myEngine->GetTexture("Data/Textures/particles/test_normal.dds");

	myData.lifeTime = -1.f;
	myData.shader = myEngine->GetEffect("Shaders/T_Particle.json");
	myData.particleData = data;
	myData.size = { 0.f, 0.f, 0.f };

	myParticles.Init(256);

	for (int i = 0; i < myParticles.Capacity(); i++)
	{
		SParticleObject toAdd;
		myParticles.Add(toAdd);
	}

	CreateBuffer();
	CreateInputLayout();
	CreateConstantBuffer();

	Effect* shader = myEngine->GetEffect("Shaders/T_particle_offscreen.json");


	myData.shader->AddShaderResource(myData.diffuseTexture->GetShaderView(), Effect::DIFFUSE);
	//myData.shader->AddShaderResource(myData.normalTexture->GetShaderView(), Effect::NORMAL);

	shader->AddShaderResource(myData.diffuseTexture->GetShaderView(), Effect::DIFFUSE);
	shader->AddShaderResource(myData.normalTexture->GetShaderView(), Effect::NORMAL);



	myTimeToEmit = 0.f;
}

void CEmitterInstance::CleanUp()
{
	SAFE_RELEASE(myInputLayout);
	SAFE_DELETE(myVertexBuffer);
	SAFE_RELEASE(myConstantBuffer);
	SAFE_DELETE(myConstantStruct);
	SAFE_RELEASE(m_GeometryBuffer);
}

void CEmitterInstance::Update(float aDeltaTime)
{
	myTimeToEmit -= aDeltaTime;
	if (myTimeToEmit < 0.f)
	{
		Emit();
		myTimeToEmit = 0.2f;
	}

	/*static int i = 0;
	for (; i < 4; i++ )
	{
		Emit();
	}
*/
	UpdateParticle(aDeltaTime);
}

void CEmitterInstance::Render(CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection, Effect* effect)
{
	if (!myConstantBuffer)
		return;

	UpdateVertexBuffer();

	DirectX11* dx = Engine::GetAPI();
	ID3D11DeviceContext* context = Engine::GetAPI()->GetContext();
	context->IASetInputLayout(myInputLayout);

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	context->IASetVertexBuffers(myVertexBuffer->myStartSlot, myVertexBuffer->myNrOfBuffers, &myVertexBuffer->myVertexBuffer, &myVertexBuffer->myStride, &myVertexBuffer->myByteOffset);

	if (!myData.shader->GetVertexShader())
		return;

	SetMatrices(camera_orientation, camera_projection);

	context->VSSetConstantBuffers(0, 1, &myConstantBuffer);
	context->GSSetConstantBuffers(0, 1, &m_GeometryBuffer);

	//myData.shader->Use();
	effect->Use();
	context->Draw(myParticles.Size(), 0);
	effect->Clear();
	//myData.shader->Clear();
}

void CEmitterInstance::RenderShadowed(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection)
{

}

void CEmitterInstance::SetPosition(const CU::Vector3f& position)
{
	myOrientation.SetPosition(position);
}

void CEmitterInstance::CreateBuffer()
{
	myVertexBuffer = new VertexBufferWrapper();
	myVertexBuffer->myStride = sizeof(SParticleObject);
	myVertexBuffer->myByteOffset = 0;
	myVertexBuffer->myStartSlot = 0;
	myVertexBuffer->myNrOfBuffers = 1;

	HRESULT hr;

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	if (myVertexBuffer->myVertexBuffer != nullptr)
		myVertexBuffer->myVertexBuffer->Release();

	vertexBufferDesc.ByteWidth = sizeof(SParticleObject) * myParticles.Capacity();

	D3D11_SUBRESOURCE_DATA vertexData;
	ZeroMemory(&vertexData, sizeof(vertexData));
	vertexData.pSysMem = reinterpret_cast<char*>(&myParticles[0]);
	hr = Engine::GetAPI()->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &myVertexBuffer->myVertexBuffer); //Added vertexData to this
	Engine::GetAPI()->HandleErrors(hr, "Failed to Create Particle Vertex Buffer");
}

void CEmitterInstance::UpdateVertexBuffer()
{
	if (myParticles.Size() > 0)
	{
		Engine::GetAPI()->UpdateConstantBuffer(myVertexBuffer->myVertexBuffer, &myParticles[0], sizeof(SParticleObject) * myParticles.Size());
	}
}

void CEmitterInstance::CreateConstantBuffer()
{
	myConstantBuffer = Engine::GetAPI()->CreateConstantBuffer(sizeof(cbParticleVertex));
	Engine::GetAPI()->SetDebugName(myConstantBuffer, "EmitterInstance : Vertex Constant Buffer");

	m_GeometryBuffer = Engine::GetAPI()->CreateConstantBuffer(sizeof(cbParticleGeometry));
	Engine::GetAPI()->SetDebugName(m_GeometryBuffer, "EmitterInstance : Geometry Constant Buffer");
}

void CEmitterInstance::CreateInputLayout()
{
	HRESULT hr = S_OK;

	void* shader = myData.shader->GetVertexShader()->compiledShader;
	int size = myData.shader->GetVertexShader()->shaderSize;

	const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "ALPHA", 0, DXGI_FORMAT_R32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "SIZE", 0, DXGI_FORMAT_R32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	hr = Engine::GetAPI()->GetDevice()->CreateInputLayout(layout, ARRAYSIZE(layout), shader, size, &myInputLayout);
	Engine::GetAPI()->HandleErrors(hr, "Failed to create InputLayout!");
	Engine::GetAPI()->SetDebugName(myInputLayout, "Particle Input Layout");
}

void CEmitterInstance::SetMatrices(CU::Matrix44f& aCameraOrientation, const CU::Matrix44f& aCameraProjection)
{
	m_VertexCB.m_World = CU::Matrix44f();
	m_VertexCB.m_View = CU::Math::Inverse(aCameraOrientation);
	Engine::GetAPI()->UpdateConstantBuffer(myConstantBuffer, &m_VertexCB);

	m_GeometryCB.m_Projection = aCameraProjection;
	Engine::GetAPI()->UpdateConstantBuffer(m_GeometryBuffer, &m_GeometryCB);
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
		myParticles[i].position += (myParticles[i].direction * myParticles[i].speed) * aDeltaTime;
		myParticles[i].currLifeTime -= aDeltaTime;
		//myParticles[i].size += (1 * aDeltaTime);
		myParticles[i].alpha = myParticles[i].currLifeTime / myParticles[i].lifeTime;
	}
}

void CEmitterInstance::Emit()
{
	SParticleObject temp; //Replace with preallocated particles

	temp.position = myOrientation.GetPosition();
	float x0 = temp.position.x;// + myData.size.x;
	float y0 = temp.position.y;// + myData.size.y;
	float z0 = temp.position.z;// + myData.size.z;

	float x1 = temp.position.x;// - myData.size.x;
	float y1 = temp.position.y;// - myData.size.y;
	float z1 = temp.position.z;// - myData.size.z;

	temp.position.x = RANDOM(x0 + -1.5f, x1 + 1.5f);
	temp.position.y = RANDOM(y0 + -1.5f, y1);
	temp.position.z = RANDOM(z0 + -1.5f, z1 + 1.5f);

	temp.size = RANDOM(1.f, 1.f);
	temp.direction.x = RANDOM(-0.15f, 0.15f);
	temp.direction.y = RANDOM(0.f, 1.f);
	temp.direction.z = RANDOM(-0.15f, 0.15f);
	const float time = 20.f;
	temp.lifeTime = time;
	temp.currLifeTime = time;
	temp.alpha = 1;// myData.particleData.startAlpha;
	temp.speed = 0.75f;
	myParticles.Add(temp);
}
