#pragma once

#include <Engine/snowblind_shared.h>

#include <Engine/Camera.h>

#include <functional>

class ShadowPass
{
public:
	ShadowPass() = default;

	bool Initiate();
	bool CleanUp();
	
	void ProcessShadows(Camera* camera, const RenderContext& render_context);
	void RegisterFunction(std::function<void()> pFunction);
private:

	Texture* m_DepthTexture = nullptr;
	CU::GrowingArray<std::function<void()>> m_FuncPtrs;


};
