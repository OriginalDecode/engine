#pragma once
#include <DataStructures/StaticArray.h>
#include "LightStructs.h"

#ifndef _WINDEF_
struct HINSTANCE__;
typedef HINSTANCE__* HINSTANCE;
struct HWND__;
typedef HWND__* HWND;
#endif



namespace Snowblind
{

	class CSynchronizer;
	class CText;
	class CDeferredRenderer;
	class CCamera;
	class CPointLight;
	class CDirectX11;
	class CRenderer
	{
	public:
		CRenderer(CSynchronizer& aSynchronizer);
		~CRenderer();

		void RenderMain(float aWidth, float aHeight, HWND* aHWND);

		void AddCamera(CCamera* aCamera);
		void Add2DCamera(CCamera* aCamera);
		CDirectX11* GetDirectX();
		void Render();
	private:
		void Initiate(float aWidth, float aHeight, HWND aHWND);

		void Render3DCommands();
		void Render2DCommands();
		void RenderLightCommands();
		CDeferredRenderer* myDeferredRenderer;
		CCamera* myCamera;
		CCamera* my2DCamera;
		CSynchronizer& mySynchronizer;
		CText* myText;
		CPointLight* myPointLight;
		CDirectX11* myDirectX;

		volatile bool myQuitFlag;

	};
}; 