#pragma once
#include <Engine/IPostprocessPass.h>
class EdgeDetectionPass : public IPostprocessPass
{
public:
	EdgeDetectionPass();
	~EdgeDetectionPass() override;

	void Initiate();

	void Process(Texture* pTexture, const graphics::RenderContext& rc) override;
	void OnResize() override;

private:

	WindowSize	m_WindowSize;

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

