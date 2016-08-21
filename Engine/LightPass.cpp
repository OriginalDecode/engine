#include "stdafx.h"
#include "LightPass.h"
#include "PointLight.h"
namespace Snowblind
{
	CLightPass::CLightPass()
		: myContext(CEngine::GetDirectX()->GetContext())
		, myEngine(CEngine::GetInstance())
	{
		myEffect = myEngine->GetEffect("Data/Shaders/T_Deferred_LightMesh.json");
		CreatePointlightBuffers();
	}

	CLightPass::~CLightPass()
	{
		SAFE_RELEASE(myConstantBuffers[u32(eBuffer::POINT_LIGHT_VERTEX)]);
		SAFE_RELEASE(myConstantBuffers[u32(eBuffer::POINT_LIGHT_PIXEL)]);
		SAFE_RELEASE(myConstantBuffers[u32(eBuffer::SPOT_LIGHT_VERTEX)]);
		SAFE_RELEASE(myConstantBuffers[u32(eBuffer::SPOT_LIGHT_PIXEL)]);

	}

	void CLightPass::RenderPointlight(CPointLight* pointlight, CCamera* aCamera, CU::Matrix44f& previousOrientation)
	{
		UpdatePointlightBuffers(pointlight, aCamera, previousOrientation);
		myContext->VSSetConstantBuffers(0, 1, &myConstantBuffers[u32(eBuffer::POINT_LIGHT_VERTEX)]);
		myContext->PSSetConstantBuffers(0, 1, &myConstantBuffers[u32(eBuffer::POINT_LIGHT_PIXEL)]);
		pointlight->Render(previousOrientation, aCamera);
	}

	CEffect* CLightPass::GetPointLightEffect()
	{
		return myEffect;
	}

	void CLightPass::UpdatePointlightBuffers(CPointLight* pointlight, CCamera* aCamera, CU::Matrix44f& previousOrientation)
	{
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
	}

	void CLightPass::CreateSpotlightBuffers()
	{
		//----------------------------------------
		// Spotlight Vertex Constant Buffer
		//----------------------------------------
		D3D11_BUFFER_DESC cbDesc;
		ZeroMemory(&cbDesc, sizeof(cbDesc));
		cbDesc.ByteWidth = sizeof(SPointlightConstantBuffer);
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
		cbDesc.ByteWidth = sizeof(SPixelConstantBuffer);
		cbDesc.Usage = D3D11_USAGE_DYNAMIC;
		cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbDesc.MiscFlags = 0;
		cbDesc.StructureByteStride = 0;

		hr = CEngine::GetDirectX()->GetDevice()->CreateBuffer(&cbDesc, 0, &myConstantBuffers[u32(eBuffer::SPOT_LIGHT_PIXEL)]);
		CEngine::GetDirectX()->SetDebugName(myConstantBuffers[u32(eBuffer::SPOT_LIGHT_PIXEL)], "Spotlight Pixel Constant Buffer");
		CEngine::GetDirectX()->HandleErrors(hr, "[LightPass] : Failed to Create Spotlight Pixel Constant Buffer, ");
	}

	void CLightPass::CreatePointlightBuffers()
	{
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
	}

};