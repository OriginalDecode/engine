#include "stdafx.h"
#include "LightPass.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "GBuffer.h"
namespace Snowblind
{
	bool LightPass::Initiate(GBuffer* aGBuffer)
	{
		myEngine = Engine::GetInstance();
		myContext = myEngine->GetAPI()->GetContext();
		myGBuffer = aGBuffer;
#ifdef SNOWBLIND_DX11
		myEffect[u32(eLight::POINT_LIGHT)] = myEngine->GetEffect("Data/Shaders/T_Deferred_Lightmesh.json");
		myEffect[u32(eLight::SPOT_LIGHT)] = myEngine->GetEffect("Data/Shaders/T_Deferred_Spotlight.json");

		myEffect[u32(eLight::POINT_LIGHT)]->AddShaderResource(myGBuffer->myAlbedo->GetShaderView());
		myEffect[u32(eLight::POINT_LIGHT)]->AddShaderResource(myGBuffer->myNormal->GetShaderView());
		myEffect[u32(eLight::POINT_LIGHT)]->AddShaderResource(myGBuffer->myDepth->GetShaderView());


		myEffect[u32(eLight::SPOT_LIGHT)]->AddShaderResource(myGBuffer->myAlbedo->GetShaderView());
		myEffect[u32(eLight::SPOT_LIGHT)]->AddShaderResource(myGBuffer->myNormal->GetShaderView());
		myEffect[u32(eLight::SPOT_LIGHT)]->AddShaderResource(myGBuffer->myDepth->GetShaderView());
#endif

		CreatePointlightBuffers();
		CreateSpotlightBuffers();

		return true;
	}

	bool LightPass::CleanUp()
	{
		SAFE_RELEASE(myConstantBuffers[u32(eBuffer::POINT_LIGHT_VERTEX)]);
		SAFE_RELEASE(myConstantBuffers[u32(eBuffer::POINT_LIGHT_PIXEL)]);
		SAFE_RELEASE(myConstantBuffers[u32(eBuffer::SPOT_LIGHT_VERTEX)]);
		SAFE_RELEASE(myConstantBuffers[u32(eBuffer::SPOT_LIGHT_PIXEL)]);
		return true;
	}

	void LightPass::RenderPointlight(CPointLight* pointlight, Camera* aCamera, const CU::Matrix44f& previousOrientation)
	{
		if (HasInitiated())
		{
			UpdatePointlightBuffers(pointlight, aCamera, previousOrientation);
			myContext->VSSetConstantBuffers(0, 1, &myConstantBuffers[u32(eBuffer::POINT_LIGHT_VERTEX)]);
			myContext->PSSetConstantBuffers(0, 1, &myConstantBuffers[u32(eBuffer::POINT_LIGHT_PIXEL)]);
			pointlight->Render(previousOrientation, aCamera);
		}
	}

	void LightPass::RenderSpotlight(CSpotLight* spotlight, Camera* aCamera, const CU::Matrix44f& previousOrientation)
	{
		UpdateSpotlightBuffers(spotlight, aCamera, previousOrientation);
		myContext->VSSetConstantBuffers(0, 1, &myConstantBuffers[u32(eBuffer::SPOT_LIGHT_VERTEX)]);
		myContext->PSSetConstantBuffers(0, 1, &myConstantBuffers[u32(eBuffer::SPOT_LIGHT_PIXEL)]);
		spotlight->Render(previousOrientation, aCamera);
	}

	Effect* LightPass::GetPointlightEffect()
	{
		return myEffect[u32(eLight::POINT_LIGHT)];
	}

	Effect* LightPass::GetSpotlightEffect()
	{
		return myEffect[u32(eLight::SPOT_LIGHT)];
	}

	void LightPass::UpdatePointlightBuffers(CPointLight* pointlight, Camera* aCamera, const CU::Matrix44f& previousOrientation)
	{
#ifdef SNOWBLIND_DX11
		//----------------------------------------
		// VertexShader Constant Buffer
		//----------------------------------------
		myPointlightVertexConstantData.world = pointlight->GetOrientation();
		myPointlightVertexConstantData.invertedView = CU::Math::Inverse(previousOrientation);
		myPointlightVertexConstantData.projection = aCamera->GetProjection();
		myPointlightVertexConstantData.scale.x = pointlight->GetRange();
		myPointlightVertexConstantData.scale.y = pointlight->GetRange();
		myPointlightVertexConstantData.scale.z = pointlight->GetRange();
		myPointlightVertexConstantData.scale.w = pointlight->GetRange();

		D3D11_MAPPED_SUBRESOURCE msr;
		ZeroMemory(&msr, sizeof(D3D11_MAPPED_SUBRESOURCE));
		Engine::GetAPI()->GetContext()->Map(myConstantBuffers[u32(eBuffer::POINT_LIGHT_VERTEX)], 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		if (msr.pData != nullptr)
		{
			SPointlightConstantBuffer* ptr = (SPointlightConstantBuffer*)msr.pData;
			memcpy(ptr, &myPointlightVertexConstantData.world.myMatrix[0], sizeof(SPointlightConstantBuffer));
		}

		Engine::GetAPI()->GetContext()->Unmap(myConstantBuffers[u32(eBuffer::POINT_LIGHT_VERTEX)], 0);

		//----------------------------------------
		// PixelShader Constant Buffer
		//----------------------------------------
		myPixelConstantStruct.myInvertedProjection = CU::Math::InverseReal(aCamera->GetProjection());
		myPixelConstantStruct.myView = previousOrientation;
		myPixelConstantStruct.myColor = pointlight->GetColor();
		myPixelConstantStruct.myPosition = pointlight->GetPosition();
		myPixelConstantStruct.myCameraPosition = previousOrientation.GetPosition();
		ZeroMemory(&msr, sizeof(D3D11_MAPPED_SUBRESOURCE));
		Engine::GetAPI()->GetContext()->Map(myConstantBuffers[u32(eBuffer::POINT_LIGHT_PIXEL)], 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		if (msr.pData != nullptr)
		{
			SPixelConstantBuffer* ptr = (SPixelConstantBuffer*)msr.pData;
			memcpy(ptr, &myPixelConstantStruct.myInvertedProjection.myMatrix[0], sizeof(SPixelConstantBuffer));
		}

		Engine::GetAPI()->GetContext()->Unmap(myConstantBuffers[u32(eBuffer::POINT_LIGHT_PIXEL)], 0);
#endif
	}

	void LightPass::UpdateSpotlightBuffers(CSpotLight* spotlight, Camera* aCamera, const CU::Matrix44f& previousOrientation)
	{
#ifdef SNOWBLIND_DX11
		//----------------------------------------
		// VertexShader Constant Buffer
		//----------------------------------------
		const SSpotlightData& data = spotlight->GetData();

		mySpotlightVertexConstantData.world = data.myOrientation;
		mySpotlightVertexConstantData.invertedView = CU::Math::Inverse(previousOrientation);
		mySpotlightVertexConstantData.projection = aCamera->GetProjection();

		mySpotlightVertexConstantData.scale.x = data.myRange;
		mySpotlightVertexConstantData.scale.y = data.myRange;

		mySpotlightVertexConstantData.angle.x = data.myAngle;
		mySpotlightVertexConstantData.angle.y = data.myAngle;

		D3D11_MAPPED_SUBRESOURCE msr;
		ZeroMemory(&msr, sizeof(D3D11_MAPPED_SUBRESOURCE));
		Engine::GetAPI()->GetContext()->Map(myConstantBuffers[u32(eBuffer::SPOT_LIGHT_VERTEX)], 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		if (msr.pData != nullptr)
		{
			SSpotlightConstantBuffer* ptr = (SSpotlightConstantBuffer*)msr.pData;
			memcpy(ptr, &mySpotlightVertexConstantData.world.myMatrix[0], sizeof(SSpotlightConstantBuffer));
		}

		Engine::GetAPI()->GetContext()->Unmap(myConstantBuffers[u32(eBuffer::SPOT_LIGHT_VERTEX)], 0);

		//----------------------------------------
		// PixelShader Constant Buffer
		//----------------------------------------
		mySpotPixelConstantStruct.myInvertedProjection = CU::Math::InverseReal(aCamera->GetProjection());
		mySpotPixelConstantStruct.myView = previousOrientation;
		mySpotPixelConstantStruct.myColor = data.myLightColor;
		mySpotPixelConstantStruct.myPosition = data.myOrientation.GetPosition();
		mySpotPixelConstantStruct.myCameraPosition = previousOrientation.GetPosition();
		mySpotPixelConstantStruct.myDirection = data.myDirection;

		ZeroMemory(&msr, sizeof(D3D11_MAPPED_SUBRESOURCE));
		Engine::GetAPI()->GetContext()->Map(myConstantBuffers[u32(eBuffer::SPOT_LIGHT_PIXEL)], 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		if (msr.pData != nullptr)
		{
			SSpotPixelConstantBuffer* ptr = (SSpotPixelConstantBuffer*)msr.pData;
			memcpy(ptr, &mySpotPixelConstantStruct.myInvertedProjection.myMatrix[0], sizeof(SSpotPixelConstantBuffer));
		}

		Engine::GetAPI()->GetContext()->Unmap(myConstantBuffers[u32(eBuffer::SPOT_LIGHT_PIXEL)], 0);
#endif
	}

	void LightPass::CreateSpotlightBuffers()
	{
#ifdef SNOWBLIND_DX11
		//----------------------------------------
		// Spotlight Vertex Constant Buffer
		//----------------------------------------
		D3D11_BUFFER_DESC cbDesc;
		ZeroMemory(&cbDesc, sizeof(cbDesc));
		cbDesc.ByteWidth = sizeof(SSpotlightConstantBuffer);
		cbDesc.Usage = D3D11_USAGE_DYNAMIC;
		cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbDesc.MiscFlags = 0;
		cbDesc.StructureByteStride = 0;

		HRESULT hr = Engine::GetAPI()->GetDevice()->CreateBuffer(&cbDesc, 0, &myConstantBuffers[u32(eBuffer::SPOT_LIGHT_VERTEX)]);
		Engine::GetAPI()->SetDebugName(myConstantBuffers[u32(eBuffer::SPOT_LIGHT_VERTEX)], "LightPass : Spotlight Vertex Constant Buffer");
		Engine::GetAPI()->HandleErrors(hr, "[LightPass] : Failed to Create Spotlight Vertex Constant Buffer, ");


		//----------------------------------------
		// Spotlight Constant Buffer
		//----------------------------------------
		ZeroMemory(&cbDesc, sizeof(cbDesc));
		cbDesc.ByteWidth = sizeof(SSpotPixelConstantBuffer);
		cbDesc.Usage = D3D11_USAGE_DYNAMIC;
		cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbDesc.MiscFlags = 0;
		cbDesc.StructureByteStride = 0;

		hr = Engine::GetAPI()->GetDevice()->CreateBuffer(&cbDesc, 0, &myConstantBuffers[u32(eBuffer::SPOT_LIGHT_PIXEL)]);
		Engine::GetAPI()->SetDebugName(myConstantBuffers[u32(eBuffer::SPOT_LIGHT_PIXEL)], "LightPass : Spotlight Pixel Constant Buffer");
		Engine::GetAPI()->HandleErrors(hr, "[LightPass] : Failed to Create Spotlight Pixel Constant Buffer, ");
#endif
	}

	void LightPass::CreatePointlightBuffers()
	{
#ifdef SNOWBLIND_DX11
		//----------------------------------------
		// Pointlight Vertex Constant Buffer
		//----------------------------------------
		D3D11_BUFFER_DESC cbDesc;
		ZeroMemory(&cbDesc, sizeof(cbDesc));
		cbDesc.ByteWidth = sizeof(SPointlightConstantBuffer);
		cbDesc.Usage = D3D11_USAGE_DYNAMIC;
		cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbDesc.MiscFlags = 0;
		cbDesc.StructureByteStride = 0;

		HRESULT hr = Engine::GetAPI()->GetDevice()->CreateBuffer(&cbDesc, 0, &myConstantBuffers[u32(eBuffer::POINT_LIGHT_VERTEX)]);
		Engine::GetAPI()->SetDebugName(myConstantBuffers[u32(eBuffer::POINT_LIGHT_VERTEX)], "LightPass : Pointlight Vertex Constant Buffer");
		Engine::GetAPI()->HandleErrors(hr, "[LightPass] : Failed to Create Pointlight Vertex Constant Buffer, ");


		//----------------------------------------
		// PixelShader Constant Buffer
		//----------------------------------------
		ZeroMemory(&cbDesc, sizeof(cbDesc));
		cbDesc.ByteWidth = sizeof(SPixelConstantBuffer);
		cbDesc.Usage = D3D11_USAGE_DYNAMIC;
		cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbDesc.MiscFlags = 0;
		cbDesc.StructureByteStride = 0;

		hr = Engine::GetAPI()->GetDevice()->CreateBuffer(&cbDesc, 0, &myConstantBuffers[u32(eBuffer::POINT_LIGHT_PIXEL)]);
		Engine::GetAPI()->SetDebugName(myConstantBuffers[u32(eBuffer::POINT_LIGHT_PIXEL)], "LightPass : Pointlight Pixel Constant Buffer");
		Engine::GetAPI()->HandleErrors(hr, "[LightPass] : Failed to Create Pointlight Pixel Constant Buffer, ");
#endif
	}

};