#include "stdafx.h"
#include "LightPass.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "GBuffer.h"
namespace Snowblind
{
	CLightPass::CLightPass(CGBuffer* aGBuffer)
		: myEngine(CEngine::GetInstance())
		, myGBuffer(aGBuffer)
#ifdef SNOWBLIND_DX11
		, myContext(CEngine::GetDirectX()->GetContext())
#endif
	{
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
	}

	CLightPass::~CLightPass()
	{
		SAFE_RELEASE(myConstantBuffers[u32(eBuffer::POINT_LIGHT_VERTEX)]);
		SAFE_RELEASE(myConstantBuffers[u32(eBuffer::POINT_LIGHT_PIXEL)]);
		SAFE_RELEASE(myConstantBuffers[u32(eBuffer::SPOT_LIGHT_VERTEX)]);
		SAFE_RELEASE(myConstantBuffers[u32(eBuffer::SPOT_LIGHT_PIXEL)]);

	}

	void CLightPass::RenderPointlight(CPointLight* pointlight, CCamera* aCamera, const CU::Matrix44f& previousOrientation)
	{
		UpdatePointlightBuffers(pointlight, aCamera, previousOrientation);
		myContext->VSSetConstantBuffers(0, 1, &myConstantBuffers[u32(eBuffer::POINT_LIGHT_VERTEX)]);
		myContext->PSSetConstantBuffers(0, 1, &myConstantBuffers[u32(eBuffer::POINT_LIGHT_PIXEL)]);
		pointlight->Render(previousOrientation, aCamera);
	}

	void CLightPass::RenderSpotlight(CSpotLight* spotlight, CCamera* aCamera, const CU::Matrix44f& previousOrientation)
	{
		UpdateSpotlightBuffers(spotlight, aCamera, previousOrientation);
		myContext->VSSetConstantBuffers(0, 1, &myConstantBuffers[u32(eBuffer::SPOT_LIGHT_VERTEX)]);
		myContext->PSSetConstantBuffers(0, 1, &myConstantBuffers[u32(eBuffer::SPOT_LIGHT_PIXEL)]);
		spotlight->Render(previousOrientation, aCamera);
	}

	CEffect* CLightPass::GetPointlightEffect()
	{
		return myEffect[u32(eLight::POINT_LIGHT)];
	}

	CEffect* CLightPass::GetSpotlightEffect()
	{
		return myEffect[u32(eLight::SPOT_LIGHT)];
	}

	void CLightPass::UpdatePointlightBuffers(CPointLight* pointlight, CCamera* aCamera, const CU::Matrix44f& previousOrientation)
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
		CEngine::GetDirectX()->GetContext()->Map(myConstantBuffers[u32(eBuffer::POINT_LIGHT_VERTEX)], 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		if (msr.pData != nullptr)
		{
			SPointlightConstantBuffer* ptr = (SPointlightConstantBuffer*)msr.pData;
			memcpy(ptr, &myPointlightVertexConstantData.world.myMatrix[0], sizeof(SPointlightConstantBuffer));
		}

		CEngine::GetDirectX()->GetContext()->Unmap(myConstantBuffers[u32(eBuffer::POINT_LIGHT_VERTEX)], 0);

		//----------------------------------------
		// PixelShader Constant Buffer
		//----------------------------------------
		myPixelConstantStruct.myInvertedProjection = CU::Math::InverseReal(aCamera->GetProjection());
		myPixelConstantStruct.myView = previousOrientation;
		myPixelConstantStruct.myColor = pointlight->GetColor();
		myPixelConstantStruct.myPosition = pointlight->GetPosition();
		myPixelConstantStruct.myCameraPosition = previousOrientation.GetPosition();
		ZeroMemory(&msr, sizeof(D3D11_MAPPED_SUBRESOURCE));
		CEngine::GetDirectX()->GetContext()->Map(myConstantBuffers[u32(eBuffer::POINT_LIGHT_PIXEL)], 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		if (msr.pData != nullptr)
		{
			SPixelConstantBuffer* ptr = (SPixelConstantBuffer*)msr.pData;
			memcpy(ptr, &myPixelConstantStruct.myInvertedProjection.myMatrix[0], sizeof(SPixelConstantBuffer));
		}

		CEngine::GetDirectX()->GetContext()->Unmap(myConstantBuffers[u32(eBuffer::POINT_LIGHT_PIXEL)], 0);
#endif
	}

	void CLightPass::UpdateSpotlightBuffers(CSpotLight* spotlight, CCamera* aCamera, const CU::Matrix44f& previousOrientation)
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
		CEngine::GetDirectX()->GetContext()->Map(myConstantBuffers[u32(eBuffer::SPOT_LIGHT_VERTEX)], 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		if (msr.pData != nullptr)
		{
			SSpotlightConstantBuffer* ptr = (SSpotlightConstantBuffer*)msr.pData;
			memcpy(ptr, &mySpotlightVertexConstantData.world.myMatrix[0], sizeof(SSpotlightConstantBuffer));
		}

		CEngine::GetDirectX()->GetContext()->Unmap(myConstantBuffers[u32(eBuffer::SPOT_LIGHT_VERTEX)], 0);

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
		CEngine::GetDirectX()->GetContext()->Map(myConstantBuffers[u32(eBuffer::SPOT_LIGHT_PIXEL)], 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		if (msr.pData != nullptr)
		{
			SSpotPixelConstantBuffer* ptr = (SSpotPixelConstantBuffer*)msr.pData;
			memcpy(ptr, &mySpotPixelConstantStruct.myInvertedProjection.myMatrix[0], sizeof(SSpotPixelConstantBuffer));
		}

		CEngine::GetDirectX()->GetContext()->Unmap(myConstantBuffers[u32(eBuffer::SPOT_LIGHT_PIXEL)], 0);
#endif
	}

	void CLightPass::CreateSpotlightBuffers()
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

		HRESULT hr = CEngine::GetDirectX()->GetDevice()->CreateBuffer(&cbDesc, 0, &myConstantBuffers[u32(eBuffer::SPOT_LIGHT_VERTEX)]);
		CEngine::GetDirectX()->SetDebugName(myConstantBuffers[u32(eBuffer::SPOT_LIGHT_VERTEX)], "Spotlight Vertex Constant Buffer");
		CEngine::GetDirectX()->HandleErrors(hr, "[LightPass] : Failed to Create Spotlight Vertex Constant Buffer, ");


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

		hr = CEngine::GetDirectX()->GetDevice()->CreateBuffer(&cbDesc, 0, &myConstantBuffers[u32(eBuffer::SPOT_LIGHT_PIXEL)]);
		CEngine::GetDirectX()->SetDebugName(myConstantBuffers[u32(eBuffer::SPOT_LIGHT_PIXEL)], "Spotlight Pixel Constant Buffer");
		CEngine::GetDirectX()->HandleErrors(hr, "[LightPass] : Failed to Create Spotlight Pixel Constant Buffer, ");
#endif
	}

	void CLightPass::CreatePointlightBuffers()
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

		HRESULT hr = CEngine::GetDirectX()->GetDevice()->CreateBuffer(&cbDesc, 0, &myConstantBuffers[u32(eBuffer::POINT_LIGHT_VERTEX)]);
		CEngine::GetDirectX()->SetDebugName(myConstantBuffers[u32(eBuffer::POINT_LIGHT_VERTEX)], "Pointlight Vertex Constant Buffer");
		CEngine::GetDirectX()->HandleErrors(hr, "[LightPass] : Failed to Create Pointlight Vertex Constant Buffer, ");


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

		hr = CEngine::GetDirectX()->GetDevice()->CreateBuffer(&cbDesc, 0, &myConstantBuffers[u32(eBuffer::POINT_LIGHT_PIXEL)]);
		CEngine::GetDirectX()->SetDebugName(myConstantBuffers[u32(eBuffer::POINT_LIGHT_PIXEL)], "Pointlight Pixel Constant Buffer");
		CEngine::GetDirectX()->HandleErrors(hr, "[LightPass] : Failed to Create Pointlight Pixel Constant Buffer, ");
#endif
	}

};