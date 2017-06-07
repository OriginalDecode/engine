#pragma once
#include <Math/Vector/Vector.h>

class Texture;
class Effect;

struct SParticleData
{
	CU::Vector3f direction = { 0.f, 0.f, 0.f };
	float speed = 0.f;
	float minStartSize = 0.f, maxStartSize = 0.f;
	float startAlpha = 0.f;
	float alphaDelta = 0.f;
	float sizeDelta = 0.f;
	float lifeTime = 0.f;
	bool affectedByGravity = false;
};

struct SAnimatedParticleData
{
	float size;
	float lifeTime;
	float timePerFrame;
};

struct SParticleObject
{
	CU::Vector3f position;
	float alpha;
	float size;
	CU::Vector3f direction;
	float speed;
	float lifeTime;
};

struct SAnimatedParticleObject
{
	CU::Vector3f position;
	float size;
	CU::Vector2i index;
	CU::Vector2f frameSize;
};


struct SEmitterData
{
	Texture* diffuseTexture = nullptr;
	Effect* shader = nullptr;

	SParticleData particleData;

	int emittsPerSecond = 0;
	CU::Vector3f size = { 0.f,0.f,0.f };
	float lifeTime = 0.f;

};

struct SEmitterGroup
{
	int groupCount = 0;

};