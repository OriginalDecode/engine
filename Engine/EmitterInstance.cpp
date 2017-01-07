#include "stdafx.h"
#ifdef SNOWBLIND_DX11
#include "EmitterInstance.h"
#include "AssetsContainer.h"
#include <Randomizer.h>
#include "VertexStructs.h"
namespace Hex
{
	void CEmitterInstance::Initiate(Synchronizer* aSynchronizer)
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

		myData.diffuseTexture = myEngine->GetTexture("Data/Textures/hp.dds");
		//myData.diffuseTexture->SetDebugName("ParticleDiffuseTexture");
		myData.lifeTime = -1.f;
		//myData.shader = myEngine->GetEffect("Data/Shaders/T_Particle.json");
		myData.particleData = data;
		myData.size = { 0.f, 0.f, 0.f };

		myParticles.Init(256);

		for (int i = 0; i < myParticles.Capacity(); i++)
		{
			SParticleObject toAdd;
			myParticles.Add(toAdd);
		}

		CreateVertexBuffer();
		//CreateInputLayout();
		CreateConstantBuffer();
		myTimeToEmit = 0.f;
	}

	void CEmitterInstance::CleanUp()
	{
		SAFE_RELEASE(myInputLayout);
		SAFE_DELETE(myVertexBuffer);
		SAFE_RELEASE(myConstantBuffer);
		SAFE_DELETE(myConstantStruct);
	}

	void CEmitterInstance::Update(float aDeltaTime)
	{
		myTimeToEmit -= aDeltaTime;
		if (myTimeToEmit < 0.f)
		{
			Emit();
			myTimeToEmit = 0.5f;
		}

		UpdateParticle(aDeltaTime);
	}

	void CEmitterInstance::Render(CU::Matrix44f& aPreviousCameraOrientation, CU::Matrix44f& aProjection, Texture* aDepthTexture)
	{
		UpdateVertexBuffer();


		ID3D11DeviceContext* context = Engine::GetAPI()->GetContext();
		context->IASetInputLayout(myInputLayout);

		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
		context->IASetVertexBuffers(myVertexBuffer->myStartSlot, myVertexBuffer->myNrOfBuffers, &myVertexBuffer->myVertexBuffer, &myVertexBuffer->myStride, &myVertexBuffer->myByteOffset);

		Engine::GetAPI()->SetVertexShader(myData.shader->GetVertexShader() ? myData.shader->GetVertexShader()->m_Shader : nullptr);
		Engine::GetAPI()->SetGeometryShader(myData.shader->GetGeometryShader() ? myData.shader->GetGeometryShader()->m_Shader : nullptr);
		Engine::GetAPI()->SetPixelShader(myData.shader->GetPixelShader() ? myData.shader->GetPixelShader()->m_Shader : nullptr);


		ID3D11ShaderResourceView* srv[2];
		srv[0] = myData.diffuseTexture->GetShaderView();
		srv[1] = aDepthTexture->GetShaderView();
		context->PSSetShaderResources(0, 2, &srv[0]);

		SetMatrices(aPreviousCameraOrientation, aProjection);
		context->VSSetConstantBuffers(0, 1, &myConstantBuffer);
		context->Draw(myParticles.Size(), 0);

		
		srv[0] = nullptr;
		srv[1] = nullptr;
		context->PSSetShaderResources(0, 2, &srv[0]);

		Engine::GetAPI()->SetGeometryShader(nullptr);

	}

	void CEmitterInstance::CreateVertexBuffer()
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
			/*
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			Engine::GetAPI()->GetContext()->Map(myVertexBuffer->myVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			if (mappedResource.pData != nullptr)
			{
				SParticleObject *data = (SParticleObject*)mappedResource.pData;
				memcpy(data, &myParticles[0], sizeof(SParticleObject)* myParticles.Size());
			}
			Engine::GetAPI()->GetContext()->Unmap(myVertexBuffer->myVertexBuffer, 0);
			*/
			
			Engine::GetAPI()->UpdateConstantBuffer(myVertexBuffer->myVertexBuffer, &myParticles);

		}
	}

	void CEmitterInstance::CreateConstantBuffer()
	{
		myConstantStruct = new SVertexBaseStruct;
		Engine::GetAPI()->CreateConstantBuffer(myConstantBuffer, sizeof(SVertexBaseStruct));
		Engine::GetAPI()->SetDebugName(myConstantBuffer, "EmitterInstance : Constant Buffer");
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

	void CEmitterInstance::SetMatrices(CU::Matrix44f& aCameraOrientation, CU::Matrix44f& aCameraProjection)
	{
		DL_ASSERT_EXP(myConstantStruct != nullptr, "Vertex Constant Buffer Struct was null.");
		myConstantStruct->world = CU::Matrix44f();
		myConstantStruct->invertedView = CU::Math::Inverse(aCameraOrientation);
		myConstantStruct->projection = aCameraProjection;

		Engine::GetAPI()->UpdateConstantBuffer(myConstantBuffer, myConstantStruct);

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
			myParticles[i].alpha = myParticles[i].lifeTime / 7.f;
		}
	}

	void CEmitterInstance::Emit()
	{
		SParticleObject temp; //Replace with preallocated particles

		temp.position.x = RANDOM(myOrientation.GetPosition().x, myData.size.x);
		temp.position.y = RANDOM(myOrientation.GetPosition().y, myData.size.y);
		temp.position.z = RANDOM(myOrientation.GetPosition().z, myData.size.z);

		temp.size = RANDOM(0.1f, 0.5f);
		temp.direction.x = RANDOM(-0.1f, 0.1f);
		temp.direction.y = RANDOM(0.f, 1.f);
		temp.direction.z = RANDOM(-0.1f, 0.1f);
		temp.lifeTime = 7.f;
		temp.alpha = myData.particleData.startAlpha;
		temp.speed = 0.5f;
		myParticles.Add(temp);
	}
};
#endif