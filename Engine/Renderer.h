#pragma once
#include <DataStructures/StaticArray.h>

#include "LightStructs.h"
#include <unordered_map>

namespace CommonUtilities
{
	class TimeManager;
}

namespace Snowblind
{

	class CSynchronizer;
	class CText;
	class CDeferredRenderer;
	class CCamera;
	class CPointLight;
	class CEmitterInstance;
	class CTexture;
	class CSkySphere;
	class CModel;

	class CRenderer
	{
	public:
		CRenderer(CSynchronizer& aSynchronizer, CCamera* aCamera);
		~CRenderer();
		void Add2DCamera(CCamera* aCamera);
		void Render();

		void AddModel(CModel* aModel, const std::string& aModelKey);
	private:
		eDeferredType myDeferredType;
		void Render3DCommands();
		void Render2DCommands();
		void RenderLightCommands();
		void RenderParticles();

		std::unordered_map<std::string, CModel*> myModels;

		CCamera* myCamera;
		CU::Matrix44f myPrevFrame;

		CCamera* my2DCamera;
		
		CU::Vector3f myPrevPos;
		CU::TimeManager* myTimeManager;
		
		CDeferredRenderer* myDeferredRenderer;
		CSynchronizer& mySynchronizer;
		CPointLight* myPointLight;
		CText* myText;
		CTexture* myDepthTexture;
		CSkySphere* mySkysphere;
		CSprite* mySprite;
	};
}; 