#pragma once


struct BlendState
{
	void* m_State = nullptr;
};

struct SamplerState
{
	void* m_State = nullptr;
};

struct DepthstencilState
{
	void* m_State = nullptr;
};


class ShaderState
{
public:
	ShaderState(const BlendState& blend_state, const SamplerState& sampler_state, const DepthstencilState& depthstencil_state);
	



	void Use();


	enum
	{
		ALPHA_BLEND = 0x1,


	};

private:

	BlendState			m_BlendState;
	SamplerState		m_SamplerState;
	DepthstencilState	m_DepthstencilState;


};

