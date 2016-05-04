#pragma once


namespace Snowblind
{

	class CSynchronizer;
	class CRenderer
	{
	public:
		CRenderer(CSynchronizer& aSynchronizer, CCamera& aCamera);
		~CRenderer();

		void Render();

	private:

		CCamera& myCamera;
		CSynchronizer& mySynchronizer;
		


	};
};