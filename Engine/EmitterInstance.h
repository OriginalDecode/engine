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
	~CEmitterInstance();
	void Initiate(Synchronizer* aSynchronizer, Texture* depth_texture);
	void Update(float aDeltaTime);

	void Render(CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection, Effect* effect);
	void RenderShadowed(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection);

	void SetPosition(const CU::Vector3f& position);


	VertexWrapper& GetVertexWrapper() { return m_VertexWrapper; }
	Effect* GetEffect() { return myData.shader; }
private:
	void CreateBuffer();

	void UpdateVertexBuffer();
	void UpdateParticle(float aDeltaTime);
	void UpdateConstantBuffer(CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection);
	void Emit();

	SEmitterData myData;
	VertexWrapper m_VertexWrapper;

	CU::GrowingArray<SParticleObject> myParticles;
	CU::Matrix44f myOrientation;

	float myTimeToEmit;

	Synchronizer* m_Synchronizer;
	VertexBaseStruct m_ConstantStruct;

	IBuffer* m_ConstantBuffer;
	struct cbParticleVertex
	{
		CU::Matrix44f m_World;
		CU::Matrix44f m_View;
	} m_VertexCB;

	IBuffer* m_GeometryBuffer;
	struct cbParticleGeometry
	{
		CU::Matrix44f m_Projection;
		int index_x, index_y;
		int x,y;
	} m_GeometryCB;
};
