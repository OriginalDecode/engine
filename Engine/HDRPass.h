#pragma once

namespace Hex
{
	class Texture;
}

class HDRPass
{
public:
	HDRPass() = default;

	void Process(Hex::Texture* finished_texture);

};

