#pragma once

namespace Hex
{
	class Texture;
}

class MotionBlurPass
{
public:
	MotionBlurPass() = default;
	void Process(Hex::Texture* finished_texture);
};

