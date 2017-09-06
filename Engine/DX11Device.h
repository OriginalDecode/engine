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
		
		void* CreateTextureFromFile(const cl::CHashString<128>& filepath) override;

	private:
		ID3D11Device* m_Device = nullptr;

	};
};