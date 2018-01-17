#include "stdafx.h"
#include "ConstantBuffer.h"
#include <Engine/IGraphicsDevice.h>
namespace graphics
{

	void ConstantBuffer::Initiate()
	{
		DL_ASSERT_EXP(!m_Variables.empty(), "Register Variables before Initializing");

		s32 size = 0;
		for (const BufferVariable& var : m_Variables)
		{
			size += var.size;
		}

		IGraphicsDevice& device = Engine::GetAPI()->GetDevice();
		m_Buffer = device.CreateConstantBuffer(size, "Buffer");
	}

	void ConstantBuffer::Bind(s32 index, ShaderBinding shader_binding, const RenderContext& rc)
	{
		const s32 idx[6] = { index, index, index, index, index, index };
		Bind(idx, shader_binding, rc);
	}

	void ConstantBuffer::Bind(const s32 index[], ShaderBinding shader_binding, const RenderContext& rc)
	{
		IGraphicsContext& ctx = rc.GetContext();

		s32 step = 0;
		for (BufferVariable& var : m_Variables)
		{
			ctx.UpdateConstantBuffer(m_Buffer, var.variable, var.size, step);
			step += var.size;
		}

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
	}

};