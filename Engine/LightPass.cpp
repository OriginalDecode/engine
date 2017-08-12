#include "stdafx.h"
#include "LightPass.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "GBuffer.h"
#include <Engine/engine_shared.h>
bool LightPass::Initiate(const GBuffer& gbuffer, Texture* shadow_texture)
{
	myEffect[eLight::POINTLIGHT] = Engine::GetInstance()->GetEffect("Shaders/deferred_pointlight.json");
	myEffect[eLight::SPOTLIGHT] =	Engine::GetInstance()->GetEffect("Shaders/deferred_spotlight.json");

	myEffect[eLight::POINTLIGHT]->AddShaderResource(gbuffer.GetDiffuse(), Effect::DIFFUSE);
	myEffect[eLight::POINTLIGHT]->AddShaderResource(gbuffer.GetNormal(), Effect::NORMAL);
	myEffect[eLight::POINTLIGHT]->AddShaderResource(gbuffer.GetDepth(), Effect::DEPTH);

	myEffect[eLight::SPOTLIGHT]->AddShaderResource(gbuffer.GetDiffuse(), Effect::DIFFUSE);
	myEffect[eLight::SPOTLIGHT]->AddShaderResource(gbuffer.GetNormal(), Effect::NORMAL);
	myEffect[eLight::SPOTLIGHT]->AddShaderResource(gbuffer.GetDepth(), Effect::DEPTH);

	CreatePointlightBuffers();
	CreateSpotlightBuffers();

	//myEffect[u32(eLight::POINTLIGHT)]->AddShaderResource(shadow_texture->GetDepthStencilView());
	//myEffect[u32(eLight::SPOTLIGHT)]->AddShaderResource(shadow_texture->GetDepthStencilView());
	return true;
}

bool LightPass::CleanUp()
{
	SAFE_RELEASE(myConstantBuffers[eBuffer::POINTLIGHT_VERTEX]);
	SAFE_RELEASE(myConstantBuffers[eBuffer::POINTLIGHT_PIXEL]);
	SAFE_RELEASE(myConstantBuffers[eBuffer::SPOTLIGHT_VERTEX]);
	SAFE_RELEASE(myConstantBuffers[eBuffer::SPOTLIGHT_PIXEL]);
	return true;
}

void LightPass::RenderPointlight(PointLight* pointlight, Camera* aCamera, const CU::Matrix44f& previousOrientation, const CU::Matrix44f& shadow_matrix, const RenderContext& render_context)
{
	if (HasInitiated())
	{
		UpdatePointlightBuffers(pointlight, aCamera, previousOrientation, shadow_matrix);
		render_context.m_Context->VSSetConstantBuffers(0, 1, &myConstantBuffers[eBuffer::POINTLIGHT_VERTEX]);
		render_context.m_Context->PSSetConstantBuffers(0, 1, &myConstantBuffers[eBuffer::POINTLIGHT_PIXEL]);
		pointlight->Render(previousOrientation, aCamera, render_context);
	}
}

void LightPass::RenderSpotlight(SpotLight* spotlight, Camera* aCamera, const CU::Matrix44f& previousOrientation, const CU::Matrix44f& shadow_matrix, const RenderContext& render_context)
{
	if ( HasInitiated() )
	{
		UpdateSpotlightBuffers(spotlight, aCamera, previousOrientation, shadow_matrix);
		render_context.m_Context->VSSetConstantBuffers(0, 1, &myConstantBuffers[eBuffer::SPOTLIGHT_VERTEX]);
		render_context.m_Context->PSSetConstantBuffers(0, 1, &myConstantBuffers[eBuffer::SPOTLIGHT_PIXEL]);
		spotlight->Render(previousOrientation, aCamera, render_context);
	}
}

Effect* LightPass::GetPointlightEffect()
{
	return myEffect[u32(eLight::POINTLIGHT)];
}

Effect* LightPass::GetSpotlightEffect()
{
	return myEffect[u32(eLight::SPOTLIGHT)];
}

void LightPass::UpdatePointlightBuffers(PointLight* pointlight, Camera* aCamera, const CU::Matrix44f& previousOrientation, const CU::Matrix44f& shadow_matrix)
{
	//----------------------------------------
	// VertexShader Constant Buffer
	//----------------------------------------
	myPointlightVertexConstantData.world = pointlight->GetOrientation();
	myPointlightVertexConstantData.invertedView = CU::Math::Inverse(previousOrientation);
	myPointlightVertexConstantData.projection = aCamera->GetPerspective();
	myPointlightVertexConstantData.scale.x = pointlight->GetRange();
	myPointlightVertexConstantData.scale.y = pointlight->GetRange();
	myPointlightVertexConstantData.scale.z = pointlight->GetRange();
	myPointlightVertexConstantData.scale.w = pointlight->GetRange();

	D3D11_MAPPED_SUBRESOURCE msr;
	ZeroMemory(&msr, sizeof(D3D11_MAPPED_SUBRESOURCE));
	Engine::GetAPI()->GetContext()->Map(myConstantBuffers[eBuffer::POINTLIGHT_VERTEX], 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	if (msr.pData != nullptr)
	{
		PointlightConstantBuffer* ptr = (PointlightConstantBuffer*)msr.pData;
		memcpy(ptr, &myPointlightVertexConstantData.world.myMatrix[0], sizeof(PointlightConstantBuffer));
	}

	Engine::GetAPI()->GetContext()->Unmap(myConstantBuffers[eBuffer::POINTLIGHT_VERTEX], 0);

	//----------------------------------------
	// PixelShader Constant Buffer
	//----------------------------------------
	myPixelConstantStruct.myInvertedProjection = CU::Math::InverseReal(aCamera->GetPerspective());
	myPixelConstantStruct.myView = previousOrientation;
	myPixelConstantStruct.myColor = pointlight->GetColor();
	myPixelConstantStruct.myPosition = pointlight->GetPosition();
	myPixelConstantStruct.myCameraPosition = previousOrientation.GetPosition();
	myPixelConstantStruct.m_ShadowMVP = shadow_matrix;

	ZeroMemory(&msr, sizeof(D3D11_MAPPED_SUBRESOURCE));
	Engine::GetAPI()->GetContext()->Map(myConstantBuffers[eBuffer::POINTLIGHT_PIXEL], 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	if (msr.pData != nullptr)
	{
		PixelConstantBuffer* ptr = (PixelConstantBuffer*)msr.pData;
		memcpy(ptr, &myPixelConstantStruct.myInvertedProjection.myMatrix[0], sizeof(PixelConstantBuffer));
	}

	Engine::GetAPI()->GetContext()->Unmap(myConstantBuffers[eBuffer::POINTLIGHT_PIXEL], 0);
}

void LightPass::UpdateSpotlightBuffers(SpotLight* spotlight, Camera* aCamera, const CU::Matrix44f& previousOrientation, const CU::Matrix44f& shadow_matrix)
{
	//----------------------------------------
	// VertexShader Constant Buffer
	//----------------------------------------
	const SpotlightData& data = spotlight->GetData();

	mySpotlightVertexConstantData.world = data.myOrientation;
	mySpotlightVertexConstantData.invertedView = CU::Math::Inverse(previousOrientation);
	mySpotlightVertexConstantData.projection = aCamera->GetPerspective();

	mySpotlightVertexConstantData.scale.x = data.myRange;
	mySpotlightVertexConstantData.scale.y = data.myRange;

	mySpotlightVertexConstantData.scale.z = data.myAngle;
	mySpotlightVertexConstantData.scale.w = data.myAngle;

	D3D11_MAPPED_SUBRESOURCE msr;
	ZeroMemory(&msr, sizeof(D3D11_MAPPED_SUBRESOURCE));
	Engine::GetAPI()->GetContext()->Map(myConstantBuffers[eBuffer::SPOTLIGHT_VERTEX], 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	if (msr.pData != nullptr)
	{
		SpotlightConstantBuffer* ptr = (SpotlightConstantBuffer*)msr.pData;
		memcpy(ptr, &mySpotlightVertexConstantData.world.myMatrix[0], sizeof(SpotlightConstantBuffer));
	}

	Engine::GetAPI()->GetContext()->Unmap(myConstantBuffers[eBuffer::SPOTLIGHT_VERTEX], 0);

	//----------------------------------------
	// PixelShader Constant Buffer
	//----------------------------------------
	mySpotPixelConstantStruct.myInvertedProjection = CU::Math::InverseReal(aCamera->GetPerspective());
	mySpotPixelConstantStruct.myView = previousOrientation;
	mySpotPixelConstantStruct.myColor = data.myLightColor;
	mySpotPixelConstantStruct.myPosition = data.myOrientation.GetPosition();
	mySpotPixelConstantStruct.myCameraPosition = previousOrientation.GetPosition();
	mySpotPixelConstantStruct.myDirection = data.myDirection;
	mySpotPixelConstantStruct.m_ShadowMVP = shadow_matrix;

	ZeroMemory(&msr, sizeof(D3D11_MAPPED_SUBRESOURCE));
	Engine::GetAPI()->GetContext()->Map(myConstantBuffers[eBuffer::SPOTLIGHT_PIXEL], 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	if (msr.pData != nullptr)
	{
		SpotPixelConstantBuffer* ptr = (SpotPixelConstantBuffer*)msr.pData;
		memcpy(ptr, &mySpotPixelConstantStruct.myInvertedProjection.myMatrix[0], sizeof(SpotPixelConstantBuffer));
	}

	Engine::GetAPI()->GetContext()->Unmap(myConstantBuffers[eBuffer::SPOTLIGHT_PIXEL], 0);
}

void LightPass::CreateSpotlightBuffers()
{
	//----------------------------------------
	// Spotlight Vertex Constant Buffer
	//----------------------------------------
	D3D11_BUFFER_DESC cbDesc;
	ZeroMemory(&cbDesc, sizeof(cbDesc));
	cbDesc.ByteWidth = sizeof(SpotlightConstantBuffer);
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	HRESULT hr = Engine::GetAPI()->GetDevice()->CreateBuffer(&cbDesc, 0, &myConstantBuffers[eBuffer::SPOTLIGHT_VERTEX]);
	Engine::GetAPI()->SetDebugName(myConstantBuffers[eBuffer::SPOTLIGHT_VERTEX], "LightPass : Spotlight Vertex Constant Buffer");
	Engine::GetAPI()->HandleErrors(hr, "[LightPass] : Failed to Create Spotlight Vertex Constant Buffer, ");


	//----------------------------------------
	// Spotlight Constant Buffer
	//----------------------------------------
	ZeroMemory(&cbDesc, sizeof(cbDesc));
	cbDesc.ByteWidth = sizeof(SpotPixelConstantBuffer);
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	hr = Engine::GetAPI()->GetDevice()->CreateBuffer(&cbDesc, 0, &myConstantBuffers[eBuffer::SPOTLIGHT_PIXEL]);
	Engine::GetAPI()->SetDebugName(myConstantBuffers[u32(eBuffer::SPOTLIGHT_PIXEL)], "LightPass : Spotlight Pixel Constant Buffer");
	Engine::GetAPI()->HandleErrors(hr, "[LightPass] : Failed to Create Spotlight Pixel Constant Buffer, ");
}

void LightPass::CreatePointlightBuffers()
{
	//----------------------------------------
	// Pointlight Vertex Constant Buffer
	//----------------------------------------
	D3D11_BUFFER_DESC cbDesc;
	ZeroMemory(&cbDesc, sizeof(cbDesc));
	cbDesc.ByteWidth = sizeof(PointlightConstantBuffer);
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	HRESULT hr = Engine::GetAPI()->GetDevice()->CreateBuffer(&cbDesc, 0, &myConstantBuffers[eBuffer::POINTLIGHT_VERTEX]);
	Engine::GetAPI()->SetDebugName(myConstantBuffers[eBuffer::POINTLIGHT_VERTEX], "LightPass : Pointlight Vertex Constant Buffer");
	Engine::GetAPI()->HandleErrors(hr, "[LightPass] : Failed to Create Pointlight Vertex Constant Buffer, ");


	//----------------------------------------
	// PixelShader Constant Buffer
	//----------------------------------------
	ZeroMemory(&cbDesc, sizeof(cbDesc));
	cbDesc.ByteWidth = sizeof(PixelConstantBuffer);
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	hr = Engine::GetAPI()->GetDevice()->CreateBuffer(&cbDesc, 0, &myConstantBuffers[eBuffer::POINTLIGHT_PIXEL]);
	Engine::GetAPI()->SetDebugName(myConstantBuffers[eBuffer::POINTLIGHT_PIXEL], "LightPass : Pointlight Pixel Constant Buffer");
	Engine::GetAPI()->HandleErrors(hr, "[LightPass] : Failed to Create Pointlight Pixel Constant Buffer, ");
}

bool LightPass::HasInitiated()
{
	return ( myEffect[POINTLIGHT]
		&& myEffect[SPOTLIGHT]
		&& myConstantBuffers[POINTLIGHT_VERTEX]
		&& myConstantBuffers[POINTLIGHT_PIXEL]
		&& myConstantBuffers[SPOTLIGHT_VERTEX]
		&& myConstantBuffers[SPOTLIGHT_PIXEL] );
}

