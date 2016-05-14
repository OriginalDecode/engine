#include "stdafx.h"
#include "EmitterInstance.h"
#include "AssetsContainer.h"
#include <Randomizer.h>
namespace Snowblind
{
	CEmitterInstance::CEmitterInstance()
	{
	}

	CEmitterInstance::~CEmitterInstance()
	{
	}

	void CEmitterInstance::Initiate(CSynchronizer* aSynchronizer)
	{
		mySynchronizer = aSynchronizer;
		SParticleData data;
		data.affectedByGravity = false;
		data.lifeTime = 3.f;
		data.minStartSize = 1.f;
		data.maxStartSize = 1.f;
		data.speed = 1.f;
		data.startAlpha = 1.f;
		data.sizeDelta = 0.f;
		data.alphaDelta = 0.f;


		myData.diffuseTexture = Snowblind::CAssetsContainer::GetInstance()->GetTexture("Data/Textures/colors.dds");
		myData.lifeTime = -1.f;
		myData.shader = Snowblind::CAssetsContainer::GetInstance()->GetEffect("Data/Shaders/Particle.fx");
		myData.particleData = data;
		myData.size = { 0.f,0.f,0.f };

		myParticles.Init(256);

		for (int i = 0; i < myParticles.Capacity(); i++)
		{
			SParticleObject toAdd;
			myParticles.Add(toAdd);
		}




		CreateVertexBuffer();
		CreateInputLayout();


		myTimeToEmit = 0.f;
	}

	void CEmitterInstance::Update(float aDeltaTime)
	{
		myTimeToEmit -= aDeltaTime;
		if (myTimeToEmit < 0.f)
		{
			Emit();
			myTimeToEmit = 0.1f;
		}

		UpdateParticle(aDeltaTime);
	}

	void CEmitterInstance::Render(CCamera* camera)
	{
		UpdateVertexBuffer();

		myData.shader->SetTexture(myData.diffuseTexture);

		myData.shader->SetMatrices(myOrientation, camera->GetOrientation(), camera->GetProjection());
		ID3D11DeviceContext* context = CEngine::GetDirectX()->GetContext();
		context->IASetInputLayout(myInputLayout);
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
		context->IASetVertexBuffers(myVertexBuffer->myStartSlot, myVertexBuffer->myNrOfBuffers, &myVertexBuffer->myVertexBuffer, &myVertexBuffer->myStride, &myVertexBuffer->myByteOffset);
		D3DX11_TECHNIQUE_DESC techDesc;
		myData.shader->GetTechnique()->GetDesc(&techDesc);

		for (UINT p = 0; p < techDesc.Passes; ++p)
		{
			myData.shader->GetTechnique()->GetPassByIndex(0)->Apply(0, context);
			context->Draw(myParticles.Size(), 0);
		}
	}

	void CEmitterInstance::CreateVertexBuffer()
	{
		myVertexBuffer = new SVertexBufferWrapper();
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

		vertexBufferDesc.ByteWidth = sizeof(SParticleObject) * myParticles.Size();

		D3D11_SUBRESOURCE_DATA vertexData;
		ZeroMemory(&vertexData, sizeof(vertexData));
		vertexData.pSysMem = reinterpret_cast<char*>(&myParticles[0]);
		hr = CEngine::GetDirectX()->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &myVertexBuffer->myVertexBuffer); //Added vertexData to this
		CEngine::GetDirectX()->HandleErrors(hr, "Failed to Create Particle Vertex Buffer");
	}

	void CEmitterInstance::UpdateVertexBuffer()
	{
		if (myParticles.Size() > 0)
		{
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			CEngine::GetDirectX()->GetContext()->Map(myVertexBuffer->myVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			if (mappedResource.pData != nullptr)
			{
				SParticleObject *data = (SParticleObject*)mappedResource.pData;
				memcpy(data, &myParticles[0], sizeof(SParticleObject)* myParticles.Capacity());
			}
			CEngine::GetDirectX()->GetContext()->Unmap(myVertexBuffer->myVertexBuffer, 0);
		}
	}

	void CEmitterInstance::CreateInputLayout()
	{
		HRESULT hr = S_OK;

		D3DX11_PASS_DESC passDesc;
		hr = myData.shader->GetTechnique()->GetPassByIndex(0)->GetDesc(&passDesc);
		CEngine::GetDirectX()->HandleErrors(hr, "Failed to get tenchnique from shader.");

		const D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "ALPHA", 0, DXGI_FORMAT_R32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "SIZE", 0, DXGI_FORMAT_R32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		hr = CEngine::GetDirectX()->GetDevice()->CreateInputLayout(layout, ARRAYSIZE(layout), passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &myInputLayout);
		CEngine::GetDirectX()->HandleErrors(hr, "Failed to create InputLayout!");
		CEngine::GetDirectX()->SetDebugName(myInputLayout, "Particle Input Layout");
	}

	void CEmitterInstance::UpdateParticle(float aDeltaTime)
	{
		for (int i = 0; i < myParticles.Size(); i++)
		{
			if (myParticles[i].lifeTime < 0.f || myParticles[i].alpha < 0.f)
			{
				myParticles.RemoveCyclicAtIndex(i);
				continue;
			}
			myParticles[i].position += (myParticles[i].direction * myParticles[i].speed) * aDeltaTime;
			myParticles[i].lifeTime -= aDeltaTime;
		}
	}

	void CEmitterInstance::Emit()
	{

		SParticleObject temp; //Replace with preallocated particles

		temp.position.x = RANDOM(myOrientation.GetPosition().x, myData.size.x);
		temp.position.y = RANDOM(myOrientation.GetPosition().y, myData.size.y);
		temp.position.z = RANDOM(myOrientation.GetPosition().z, myData.size.z);

		temp.size = 1.f;
		temp.direction.x = RANDOM(-1.f, 1.f);
		temp.direction.y = RANDOM(-1.f, 1.f);
		temp.direction.z = RANDOM(-1.f, 1.f);
		temp.lifeTime = 8.f;
		temp.alpha = 1.f;
		temp.speed = 20.f;
		myParticles.Add(temp);
	}
};