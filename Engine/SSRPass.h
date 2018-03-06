#pragma once
#include <Engine/IPostprocessPass.h>

class SSRPass : public IPostprocessPass
{
public:
	SSRPass();
	~SSRPass() override;

	void Process(Texture* scene, const graphics::RenderContext& rc) override;
	void OnResize() override;
private:

	Texture* m_Reflection = nullptr;
	IBuffer* m_Buffer = nullptr;
	struct
	{
		CU::Matrix44f m_InvProjection;
		CU::Matrix44f m_InvView;
		CU::Matrix44f m_Projection;
		CU::Matrix44f m_View;
		CU::Vector4f m_CameraPos;
	} m_SSRStruct;



};

