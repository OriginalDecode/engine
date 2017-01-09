#pragma once
#ifdef SNOWBLIND_DX11
#include "SnowblindParticleData.h"
#include "VertexWrapper.h"
#include "IndexWrapper.h"

namespace Hex
{
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
		void Render(CU::Matrix44f& aPreviousCameraOrientation, CU::Matrix44f& aProjection);

		void SetPosition(const CU::Vector3f& position);

	private:
		void CreateVertexBuffer();
		void UpdateVertexBuffer();
		void CreateConstantBuffer();
		void CreateInputLayout();
		void SetMatrices(CU::Matrix44f& aCameraOrientation, CU::Matrix44f& aCameraProjection);

		void UpdateParticle(float aDeltaTime);
		void Emit();
		SEmitterData myData;
		VertexBufferWrapper* myVertexBuffer;

		CU::GrowingArray<SParticleObject> myParticles;
		CU::Matrix44f myOrientation;

		float myTimeToEmit;
		IInputLayout* myInputLayout;
		Synchronizer* mySynchronizer;

		ID3D11Buffer* myConstantBuffer;
		SVertexBaseStruct* myConstantStruct;
		Engine* myEngine;


		struct cbParticleVertex
		{
			CU::Matrix44f m_World;
			CU::Matrix44f m_View;
		} m_VertexCB;


	};
};
#endif