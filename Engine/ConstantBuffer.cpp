#include "stdafx.h"
#include "ConstantBuffer.h"
#include <Engine/IGraphicsDevice.h>
#include <Engine/profile_defines.h>

namespace graphics
{
	void ConstantBuffer::Initiate(const char* debug_name)
	{
		DL_ASSERT_EXP(!m_Variables.empty(), "Register Variables before Initializing");

		s32 size = 0;
		for (const BufferVariable& var : m_Variables)
		{
			size += var.size;
		}

		IGraphicsDevice& device = Engine::GetAPI()->GetDevice();


		s32 mul_of_16 = size % 16;
		if (mul_of_16 != 0)
		{
			size += 16;
			mul_of_16 = size % 16;
			size -= mul_of_16;
				
			mul_of_16 = size % 16;
			DL_ASSERT_EXP(mul_of_16 == 0, "Not mul of 16");

		}


		m_Buffer = device.CreateConstantBuffer(size, debug_name);
	}

	void ConstantBuffer::Bind(s32 index, s32 shader_binding, const RenderContext& rc)
	{
		const s32 idx[6] = { index, index, index, index, index, index };
		Bind(idx, shader_binding, rc);
	}


	void ConstantBuffer::Bind(const s32 index[], s32 shader_binding, const RenderContext& rc)
	{
		PROFILE_FUNCTION(profiler::colors::Orange);

		IGraphicsContext& ctx = rc.GetContext();

		PROFILE_BLOCK("Mapping");
		s8* data = ctx.Map(m_Buffer);
		s32 step = 0;
		for (BufferVariable& var : m_Variables)
		{
			memcpy(&data[step], static_cast<s8*>(var.variable), var.size);
			step += var.size;
		}
		ctx.Unmap(m_Buffer);
		PROFILE_BLOCK_END;


		PROFILE_BLOCK("Setting Constant Buffer");

		if (shader_binding & VERTEX)
			ctx.VSSetConstantBuffer(index[0], 1, &m_Buffer);

		if (shader_binding & PIXEL)
			ctx.PSSetConstantBuffer(index[1], 1, &m_Buffer);

		if (shader_binding & GEOMETRY)
			ctx.GSSetConstantBuffer(index[2], 1, &m_Buffer);

		if (shader_binding & HULL)
			ctx.HSSetConstantBuffer(index[3], 1, &m_Buffer);

		if (shader_binding & DOMAINS)
			ctx.DSSetConstantBuffer(index[4], 1, &m_Buffer);

		if (shader_binding & COMPUTE)
			ctx.CSSetConstantBuffer(index[5], 1, &m_Buffer);

		PROFILE_BLOCK_END;

	}
};