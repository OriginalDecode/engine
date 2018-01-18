#include "stdafx.h"
#include "ConstantBuffer.h"
#include <Engine/IGraphicsDevice.h>
#include <Engine/DX11Context.h>
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
		m_Buffer = device.CreateConstantBuffer(size, debug_name);
	}

	void ConstantBuffer::Bind(s32 index, s32 shader_binding, const RenderContext& rc)
	{
		const s32 idx[6] = { index, index, index, index, index, index };
		Bind(idx, shader_binding, rc);
	}

	void ConstantBuffer::Bind(const s32 index[], s32 shader_binding, const RenderContext& rc)
	{
		IGraphicsContext& ctx = rc.GetContext();

		ID3D11DeviceContext* _ctx = static_cast<ID3D11DeviceContext*>(static_cast<DX11Context&>(ctx).GetContext());

		D3D11_MAPPED_SUBRESOURCE msr;
		ZeroMemory(&msr, sizeof(D3D11_MAPPED_SUBRESOURCE));


		ID3D11Buffer* buffer = static_cast<ID3D11Buffer*>(m_Buffer);
		_ctx->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		if (msr.pData)
		{
			s8* data = static_cast<s8*>(msr.pData);

			s32 step = 0;
			for (BufferVariable& var : m_Variables)
			{
				memcpy(&data[step], static_cast<s8*>(var.variable), var.size);
				step += var.size;
			}


		}
		_ctx->Unmap(buffer, 0);

	
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