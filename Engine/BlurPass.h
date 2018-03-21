#pragma once
#include "IPostprocessPass.h"
class BlurPass : public IPostprocessPass
{
public:
	BlurPass();
	~BlurPass() override;

	void Initiate(const char* effect);

	void Process(Texture* scene, const graphics::RenderContext& rc) override;
	void OnResize() override;




};

