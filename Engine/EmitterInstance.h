#pragma once
#ifdef SNOWBLIND_DX11
#include "SnowblindParticleData.h"
#include "VertexWrapper.h"
#include "IndexWrapper.h"

namespace Snowblind
{
	class Camera;
	class CSynchronizer;
	class Texture;
	class Engine;
	class CEmitterInstance
	{
	public:
		CEmitterInstance();
		~CEmitterInstance();

		void Initiate(CSynchronizer* aSynchronizer);

		void Update(float aDeltaTime);
		void Render(CU::Matrix44f& aPreviousCameraOrientation, CU::Matrix44f& aProjection, Texture* aDepthTexture);

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
		CSynchronizer* mySynchronizer;

		ID3D11Buffer* myConstantBuffer;
		SVertexBaseStruct* myConstantStruct;
		Engine* myEngine;
	};
};
#endif