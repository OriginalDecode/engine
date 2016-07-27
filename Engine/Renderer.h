#pragma once
#include "LightStructs.h"

namespace CommonUtilities
{
	class TimeManager;
}


class CLine3D;

namespace Snowblind
{

	class CCamera;
	class CDeferredRenderer;
	class CEmitterInstance;
	class CModel;
	class CPointLight;
	class CSkySphere;
	class CSynchronizer;
	class CTexture;
	class CText;

	class CRenderer
	{
	public:
		CRenderer(CSynchronizer& aSynchronizer, CCamera* aCamera);
		~CRenderer();
		void Add2DCamera(CCamera* aCamera);
		void Render();

	private:
		void Render3DCommands();
		void Render2DCommands();
		void RenderLightCommands();
		void RenderParticles();
		void RenderLines();

		eDeferredType		myDeferredType;
		CEngine*			myEngine;
		CDirectX11*			myDirectX;

		CCamera*			myCamera;
		CU::Matrix44f		myPrevFrame;

		CCamera*			my2DCamera;
		CU::Vector3f		mySpherePos;
		CU::TimeManager*	myTimeManager;
		
		CDeferredRenderer*	myDeferredRenderer;
		CSynchronizer&		mySynchronizer;
		CPointLight*		myPointLight;
		CText*				myText;
		CTexture*			myDepthTexture;
		CSkySphere*			mySkysphere;
		CSprite*			mySprite;
		CLine3D*			my3DLine;
	};
}; 