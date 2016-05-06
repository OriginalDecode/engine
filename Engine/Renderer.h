#pragma once


namespace Snowblind
{

	class CSynchronizer;
	class CText;
	class CRenderer
	{
	public:
		CRenderer(CSynchronizer& aSynchronizer, CCamera& aCamera);
		~CRenderer();
		void Add2DCamera(CCamera* aCamera);
		void Render();

	private:

		CCamera& myCamera;
		CCamera* my2DCamera;
		CSynchronizer& mySynchronizer;
		CText* myText;


	};
};