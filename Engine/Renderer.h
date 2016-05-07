#pragma once
#include <DataStructures/StaticArray.h>

#include "LightStructs.h"


namespace Snowblind
{

	class CSynchronizer;
	class CText;
	class CDeferredRenderer;
	class CCamera;
	class CRenderer
	{
	public:
		CRenderer(CSynchronizer& aSynchronizer, CCamera& aCamera);
		~CRenderer();
		void Add2DCamera(CCamera* aCamera);
		void Render();
	private:

		void Render3DCommands();
		void Render2DCommands();
		void RenderLightCommands();
		CDeferredRenderer* myDeferredRenderer;
		CCamera& myCamera;
		CCamera* my2DCamera;
		CSynchronizer& mySynchronizer;
		CText* myText;
	};
}; 