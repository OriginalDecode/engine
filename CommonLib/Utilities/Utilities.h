#pragma once

#include "../Math/Vector/Vector.h"
#include "../Math/Matrix/Matrix.h"

#include "../Types.hpp"

#include <thread>
#include <vector>
#include <string>
#include <math.h>
#include <Rpc.h>

namespace cl
{
	bool file_exist(std::string path);

	const int64 nearest_pow(int64 value);

	template<typename T, typename T2>
	void clamp(T& value, T2 min, T2 max)
	{
		value = (value < min) ? min : (value > max) ? max : value;
	}


	template<typename T>
	constexpr T _min(T a, T b) 
	{
		return a > b ? b : a;
	}

	template<typename T>
	constexpr T _max(T a, T b)
	{
		return a < b ? b : a;
	}

	void ToLower(std::string& str);

	template<typename T>
	inline T Lerp(const T start, T end, float alpha)
	{
		return start + (end - start) * alpha;
	}

	float ClampF(float value, float min, float max);
	int ClampI(int value, int min, int max);

	struct Color
	{
		Color() = default;
		Color(uint8 red, uint8 green, uint8 blue, uint8 alpha)
			: r(red), g(green), b(blue), a(alpha) { };
		Color(uint32 c) : color(c){}

#pragma warning( push )
#pragma warning( disable: 4201 )
		union
		{
			uint32 color; 
			struct 
			{
				uint8 r, g, b, a; 
			};
		};
#pragma warning( pop )
	};

	float RadToDegree(float aRadian);
	float DegreeToRad(float aDegree);

	constexpr float _DegToRad(const float x);

	float CalcPointOnCurve(float x, float y, float t);
	CU::Vector2f Bezier(CU::Vector2f p1, CU::Vector2f p2, CU::Vector2f p3, float t);
	CU::Vector3f Bezier(CU::Vector3f p1, CU::Vector3f p2, CU::Vector3f p3, float t);
	CU::Vector3f CubicBezier(CU::Vector3f p1, CU::Vector3f p2, CU::Vector3f p3, CU::Vector3f p4, float t);
	unsigned int binomialCoef(int n, int k);

	void SetThreadName(const std::thread::id& id, const char* name);
	std::string HandleVAArgs(const char* formatted_string, ...);

	inline double TruncDecimal(double d, double p) 
	{
		return std::trunc(d * p) / p; // C++11
	}

	const std::wstring ToWideStr(const std::string& str);
	std::string GuidToString(const GUID& guid);

	GUID StrToGuid(const std::string& str);


	struct File
	{
		char filename[260];
	};
	std::vector<File> FindFilesInDirectory(const char* directory_path);
	uint64 Hash(std::string key);

};

struct Ticket_Mutex
{
	uint64 volatile ticket = 0;
	uint64 volatile serving = 0;
};

inline uint64 AtmoicAdduint64(uint64 volatile *value, uint64 toAdd)
{
	uint64 result = _InterlockedExchange64((__int64 volatile *)value, (*value + toAdd));
	return result;
};

inline void BeginTicketMutex(Ticket_Mutex* mutex)
{
	uint64 ticket = AtmoicAdduint64(&mutex->ticket, 1); //AtomicAdd?
	while (ticket != mutex->serving)
	{
		//should have a timer or something here in debug to auto crash
	}

};

inline void EndTicketMutex(Ticket_Mutex* mutex)
{
	mutex->serving++;
};
