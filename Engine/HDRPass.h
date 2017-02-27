#pragma once
#include <DataStructures/GrowingArray.h>
namespace Hex
{
	class Texture;
}

class HDRPass
{
public:
	HDRPass() = default;
	void Initiate();
	void CleanUp();
	void Process(Hex::Texture* scene_texture);
private:

	void Downsample();


	CU::GrowingArray<Hex::Texture*> m_Downsamples;
	Hex::Texture* m_HDRTexture;

};

