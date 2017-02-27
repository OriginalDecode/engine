#pragma once
namespace Hex
{
	class Texture;
}

class BloomPass
{
public:
	BloomPass() = default;

	void Process(Hex::Texture* finished_texture);

};

