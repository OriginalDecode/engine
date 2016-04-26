#pragma once
namespace Snowblind
{
	class CSprite;
	class CCamera;

	class CConsole
	{
	public:
		CConsole();
		~CConsole();

		void Initiate(CCamera* aCamera);
		void Render();
		void Update();
		void ToggleConsole();
	private:

		bool myIsActive;
		CSprite* mySprite;
		CCamera* myCamera;
	};

};