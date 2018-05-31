#pragma once

#include <random>
//if you input a higher value in min than max, it will crash. The following must be true. min <= max. Otherwise you will crash.

#define max_value(a,b) (((a) > (b)) ? (a) : (b))
#define min_value(a,b) (((a) < (b)) ? (a) : (b))

#define RANDOM(a,b) Randomizer::GetInstance()->Rand(min_value(a,b) ,max_value(a,b))

#undef MIN
#undef MAX
class Randomizer
{
public:

	static Randomizer* GetInstance();
	static bool Create();
	static bool Destroy();

	template<typename T>
	T Rand(const T& min, const T& max);
	



private:
	Randomizer();
	~Randomizer();

	static Randomizer* myInstance;

	std::random_device myRandomDevice;
	std::mt19937 e2;
};

template<typename T>
T Randomizer::Rand(const T& min, const T& max)
{
	std::uniform_real_distribution<> dist(min, max);

	return static_cast<T>(dist(e2));
}

