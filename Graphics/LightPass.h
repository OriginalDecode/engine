#pragma once
#include "RenderPass.h"

//extern IRenderTargetView;

class SpotLight;
class PointLight;
namespace graphics
{
	class LightPass : public RenderPass
	{
	public:

		LightPass(IRenderTargetView* rtv, IDepthStencilView* dsv);
		~LightPass() override;

		void Update() override;
		

	private:

		SpotLight*	m_Spotlight = nullptr;
		PointLight* m_PointLight = nullptr;

		//CU::GrowingArray<CU::Matrix44f> m_Spotlights;
		//CU::GrowingArray<CU::Matrix44f> m_Pointlights;


	};
};