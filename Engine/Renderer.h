#pragma once


namespace Snowblind
{

	class CSynchronizer;
	class CText;
	class CDeferredRenderer;
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

		CDeferredRenderer* myDeferredRenderer;
		CCamera& myCamera;
		CCamera* my2DCamera;
		CSynchronizer& mySynchronizer;
		CText* myText;

	};
}; 