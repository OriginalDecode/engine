#include "stdafx.h"
#include "ConstantBuffer.h"
#include <Engine/IGraphicsDevice.h>
#include <Engine/profile_defines.h>

namespace graphics
{
	void ConstantBuffer::Initiate(const char* debug_name)
	{
		ASSERT(!m_Variables.empty(), "Register Variables before Initializing");

		int32 size = 0;
		for (const BufferVariable& var : m_Variables)
		{
			size += var.size;
		}

		IGraphicsDevice& device = Engine::GetAPI()->GetDevice();


		int32 mul_of_16 = size % 16;
		if (mul_of_16 != 0)
		{
			size += 16;
			mul_of_16 = size % 16;
			size -= mul_of_16;
				
			mul_of_16 = size % 16;
			ASSERT(mul_of_16 == 0, "Not mul of 16");

		}


		m_Buffer = device.CreateConstantBuffer(size, debug_name);
	}

	void ConstantBuffer::Bind(int32 index, int32 shader_binding, const RenderContext& rc)
	{
		const int32 idx[6] = { index, index, index, index, index, index };
		Bind(idx, shader_binding, rc);
	}


	void ConstantBuffer::Bind(const int32 index[], int32 shader_binding, const RenderContext& rc)
	{
		PROFILE_FUNCTION(profiler::colors::Orange);

		IGraphicsContext& ctx = rc.GetContext();

		PROFILE_BLOCK("Mapping");
		int8* data = ctx.Map(m_Buffer);
		int32 step = 0;
		for (BufferVariable& var : m_Variables)
		{
			memcpy(&data[step], static_cast<int8*>(var.variable), var.size);
			step += var.size;
		}
		ctx.Unmap(m_Buffer);
		PROFILE_BLOCK_END;


		PROFILE_BLOCK("Setting Constant Buffer");

		if (shader_binding & EShaderTypeFlag_VERTEX)
			ctx.VSSetConstantBuffer(index[0], 1, &m_Buffer);

		if (shader_binding & EShaderTypeFlag_PIXEL)
			ctx.PSSetConstantBuffer(index[1], 1, &m_Buffer);

		if (shader_binding & EShaderTypeFlag_GEOMETRY)
			ctx.GSSetConstantBuffer(index[2], 1, &m_Buffer);

		if (shader_binding & EShaderTypeFlag_HULL)
			ctx.HSSetConstantBuffer(index[3], 1, &m_Buffer);

		if (shader_binding & EShaderTypeFlag_DOMAIN)
			ctx.DSSetConstantBuffer(index[4], 1, &m_Buffer);

		if (shader_binding & EShaderTypeFlag_COMPUTE)
			ctx.CSSetConstantBuffer(index[5], 1, &m_Buffer);

		PROFILE_BLOCK_END;

	}
};