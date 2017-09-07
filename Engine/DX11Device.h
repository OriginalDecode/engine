#pragma once
#include "IGraphicsDevice.h"

struct ID3D11Device;
namespace graphics
{
	class DX11Device : public IGraphicsDevice
	{
	public:
		DX11Device(ID3D11Device* device);


		void* CreateVertexShader(void* pShader, const cl::CHashString<128>& debug_name) override;
		void* CreatePixelShader(void* pShader, const cl::CHashString<128>& debug_name) override;
		void* CreateGeometryShader(void* pShader, const cl::CHashString<128>& debug_name) override;
		void* CreateHullShader(void* pShader, const cl::CHashString<128>& debug_name) override;
		void* CreateDomainShader(void* pShader, const cl::CHashString<128>& debug_name) override;
		void* CreateComputeShader(void* pShader, const cl::CHashString<128>& debug_name) override;
		
		void* CreateTextureFromFile(const cl::CHashString<128>& filepath, bool generate_mips, IGraphicsContext* ctx) override;
		void* CreateTexture2D(void* pOutTexture, Texture2DDesc desc, const cl::CHashString<128>& debug_name) override;

		void ReleasePtr(void* ptr) override;
	private:
		ID3D11Device* m_Device = nullptr;
		
		D3D11_USAGE GetUsage(s32 usage);
		u32 GetBindFlag(s32 binding);
		DXGI_FORMAT GetFormat(s32 format);
		u32 GetCPUAccessFlag(s32 flags);



	};
};