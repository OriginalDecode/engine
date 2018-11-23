#pragma once

#include <random>
#include "Utilities.h"
namespace cl
{
	template<typename T>
	T Rand(const T& min, const T& max)
	{
		static std::random_device device;
		static std::mt19937 e2(device());
		std::uniform_real_distribution<> dist(min, max);
		return static_cast<T>(dist(e2));
	}
};

#define RANDOM(a,b) cl::Rand(cl::_min<decltype(a)>(a,b) , cl::_max<decltype(a)>(a,b))