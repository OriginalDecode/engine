#pragma once
#include <CommonLib/HashString.h>

#include <Engine/engine_shared.h>

/*
vulkan has
Vertex = Vertex
Fragment = Pixel
Geometry = Geometry
Compute = Compute
Tesselation Control = Hull
Tesselation Evaluation = Domain
*/
namespace graphics
{
	class IGraphicsDevice
	{
	public:
		virtual void* CreateVertexShader(void* pShader, const cl::CHashString<128>& debug_name) = 0;
		virtual void* CreatePixelShader(void* pShader, const cl::CHashString<128>& debug_name) = 0;
		virtual void* CreateGeometryShader(void* pShader, const cl::CHashString<128>& debug_name) = 0;
		virtual void* CreateHullShader(void* pShader, const cl::CHashString<128>& debug_name) = 0;
		virtual void* CreateDomainShader(void* pShader, const cl::CHashString<128>& debug_name) = 0;
		virtual void* CreateComputeShader(void* pShader, const cl::CHashString<128>& debug_name) = 0;

	

	private:
	};
};