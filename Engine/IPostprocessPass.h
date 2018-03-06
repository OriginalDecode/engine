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
	virtual ~IPostprocessPass() { };

	virtual void Process(Texture* scene, const graphics::RenderContext& rc) = 0;
	virtual void OnResize() = 0;
protected:
	IPostprocessPass() = default;
	Quad* m_Quad = nullptr;



};
