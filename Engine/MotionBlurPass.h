#pragma once


class Texture;

class MotionBlurPass
{
public:
	MotionBlurPass() = default;
	void Process(Texture* finished_texture);
};

