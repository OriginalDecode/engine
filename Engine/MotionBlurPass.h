#pragma once


class Texture;

class MotionBlurPass
{
public:
	MotionBlurPass() = default;
	void Process(Texture* finished_texture, const graphics::RenderContext& render_context);
};

