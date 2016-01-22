#pragma once
#include "API.h"
#ifndef _WINDEF_
struct HINSTANCE__;
typedef HINSTANCE__* HINSTANCE;
struct HWND__;
typedef HWND__* HWND;
#endif

typedef long HRESULT;

struct IDXGISwapChain;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
struct ID3D11Texture2D;
struct ID3D11DepthStencilState;
struct ID3D11DeviceChild;
struct ID3D11Debug;
struct ID3D11CommandList;

namespace Snowblind
{
	class CDirectX11 :
		public CAPI
	{
	public:
		CDirectX11(HWND aWindowHandle, float aWidth, float aHeight);
		~CDirectX11();
		void						Present() override;
		void						Clear() override;
		ID3D11Device*				GetDevice();
		ID3D11DeviceContext*		GetContext();

	private:

		void						SetDebugName(ID3D11DeviceChild* aChild, const std::string& aDebugName);
		void						CreateDeviceAndSwapchain();
		void						CreateDepthBuffer();
		void						CreateBackBuffer();
		void						CreateViewport();
		void						CreateDeferredContext();
		HWND						myHWND;

		ID3D11Debug					*myDebug;
		ID3D11Device				*myDevice;
		IDXGISwapChain				*mySwapchain;
		ID3D11Texture2D				*myDepthBuffer;

		ID3D11DeviceContext			*myContext;
		ID3D11DeviceContext			*myDeferredContext;

		ID3D11RenderTargetView		*myRenderTarget;
		ID3D11DepthStencilView		*myDepthView;
		ID3D11DepthStencilState		*myDepthState;
		ID3D11CommandList			*myCommandList[2];


		float myWidth;
		float myHeight;

	};

	__forceinline ID3D11Device* CDirectX11::GetDevice()
	{
		return myDevice;
	}

	__forceinline ID3D11DeviceContext* CDirectX11::GetContext()
	{
		return myContext;
	}


};