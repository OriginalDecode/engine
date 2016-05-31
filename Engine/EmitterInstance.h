#pragma once
#include "SnowblindParticleData.h"
#include "VertexWrapper.h"
#include "IndexWrapper.h"

struct ID3D11InputLayout;
namespace Snowblind
{
	class CCamera;
	class CSynchronizer;
	class CTexture;
	class CEmitterInstance
	{
	public:
		CEmitterInstance();
		~CEmitterInstance();

		void Initiate(CSynchronizer* aSynchronizer);

		void Update(float aDeltaTime);
		void Render(CCamera* camera, CTexture* aDepthTexture);

	private:
		void CreateVertexBuffer();
		void UpdateVertexBuffer();

		void CreateInputLayout();

		void UpdateParticle(float aDeltaTime);
		void Emit();
		SEmitterData myData;
		SVertexBufferWrapper* myVertexBuffer;

		CU::GrowingArray<SParticleObject> myParticles;
		CU::Matrix44f myOrientation;

		float myTimeToEmit;

		ID3D11InputLayout* myInputLayout;
		CSynchronizer* mySynchronizer;
		
	};
};