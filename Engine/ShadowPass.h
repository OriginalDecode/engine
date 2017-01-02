#pragma once
namespace Hex
{
	class ShadowPass
	{
	public:
		ShadowPass() = default;

		bool Initiate();
		bool CleanUp();


	private:

		Texture* m_DepthTexture = nullptr;



	};
};