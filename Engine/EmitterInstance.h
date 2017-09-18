#pragma once
#include "ParticleData.h"
#include "VertexWrapper.h"
#include "IndexWrapper.h"


class Camera;
class Synchronizer;
class Texture;
class Engine;
class CEmitterInstance
{
public:
	CEmitterInstance() = default;

	void Initiate(Synchronizer* aSynchronizer, Texture* depth_texture);
	void CleanUp();
	void Update(float aDeltaTime);

	void Render(CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection, Effect* effect);
	void RenderShadowed(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection);

	void SetPosition(const CU::Vector3f& position);

private:
	void CreateBuffer();
	void UpdateVertexBuffer();
	void CreateConstantBuffer();
	void CreateInputLayout();
	void SetMatrices(CU::Matrix44f& aCameraOrientation, const CU::Matrix44f& aCameraProjection);

	void UpdateParticle(float aDeltaTime);
	void Emit();
	SEmitterData myData;
	VertexBufferWrapper* myVertexBuffer;

	CU::GrowingArray<SParticleObject> myParticles;
	CU::Matrix44f myOrientation;

	float myTimeToEmit;
	IInputLayout* myInputLayout;
	Synchronizer* m_Synchronizer;

	ID3D11Buffer* myConstantBuffer;
	VertexBaseStruct* myConstantStruct;
	Engine* myEngine;


	struct cbParticleVertex
	{
		CU::Matrix44f m_World;
		CU::Matrix44f m_View;
	} m_VertexCB;

	ID3D11Buffer* m_GeometryBuffer;
	struct cbParticleGeometry
	{
		CU::Matrix44f m_Projection;
	} m_GeometryCB;
};
