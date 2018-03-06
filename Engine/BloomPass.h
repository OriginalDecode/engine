#pragma once
#include <Engine/IPostprocessPass.h>

class BloomPass : public IPostprocessPass
{
public:
	BloomPass();
	~BloomPass() override;


	void Process(Texture* scene, const graphics::RenderContext& rc) override;
	void OnResize() override;

private:

};

