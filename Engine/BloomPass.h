#pragma once

class Texture;

class BloomPass
{
public:
	BloomPass() = default;

	void Process(Texture* finished_texture);

};

