#pragma once

namespace graphics 
{ 
	class RenderContext; 
}

class Texture;
class Quad;

class IPostprocessPass
{
public:
	virtual ~IPostprocessPass()	{ };
	virtual void Process(Texture* scene, const graphics::RenderContext& rc) = 0;
	virtual void OnResize() = 0;
protected:
	IPostprocessPass()
		: m_Quad(nullptr)
		, m_Effect(nullptr)
	{
	};

	Quad*	m_Quad;
	Effect*	m_Effect;
	WindowSize	m_WindowSize;



};
