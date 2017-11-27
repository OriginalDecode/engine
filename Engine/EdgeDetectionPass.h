#pragma once
#include <Engine/Quad.h>

class EdgeDetectionPass
{
public:
	EdgeDetectionPass();
	~EdgeDetectionPass();

	void Initiate();

	void Process(Texture* pTexture, const graphics::RenderContext& rc);
	void OnResize();

private:

	WindowSize	m_WindowSize;
	Quad* m_ScreenQuad = nullptr;
	Texture* m_Result = nullptr;

	Effect* m_EdgeDetectionShader = nullptr;
	IBuffer* m_cbEdgeDetection = nullptr;
	struct
	{
		float m_Width;
		float m_Height;
		float dummy[2];

	} m_EdgeDetectionData;
};

