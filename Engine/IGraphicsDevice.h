#pragma once
#include <CommonLib/HashString.h>

#include <Engine/engine_shared.h>
#include <Engine/IGraphicsAPI.h>
namespace graphics
{
	class IGraphicsContext;
	class IGraphicsDevice
	{
	public:
		virtual void* CreateVertexShader(void* pShader, const cl::CHashString<128>& debug_name) = 0;
		virtual void* CreatePixelShader(void* pShader, const cl::CHashString<128>& debug_name) = 0;
		virtual void* CreateGeometryShader(void* pShader, const cl::CHashString<128>& debug_name) = 0;
		virtual void* CreateHullShader(void* pShader, const cl::CHashString<128>& debug_name) = 0;
		virtual void* CreateDomainShader(void* pShader, const cl::CHashString<128>& debug_name) = 0;
		virtual void* CreateComputeShader(void* pShader, const cl::CHashString<128>& debug_name) = 0;

		virtual void* CreateTextureFromFile(const cl::CHashString<128>& filepath, bool generate_mips, IGraphicsContext* ctx) = 0;
		virtual void* CreateTexture2D(void* pOutTexture, Texture2DDesc desc, const cl::CHashString<128>& debug_name) = 0; //Needs to take in a description of how to setup the texture as well.



		virtual void ReleasePtr(void* ptr) = 0;

	private:
	};
};

/*
MAPPING FOR VULKAN
Vertex = Vertex
Fragment = Pixel
Geometry = Geometry
Compute = Compute
tessellation Control = Hull
tessellation Evaluation = Domain
*/
