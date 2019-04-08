#pragma once
#include <string>
#include <math.h>
#include "Math/Vector/Vector.h"
#include "Math/Matrix/Matrix.h"
#include "DataStructures/GrowingArray.h"
#include <thread>
#include <iso646.h>
#include <vector>
#include "../standard_datatype.hpp"
namespace cl
{
	template<typename T>
	CU::Math::Vector3<T> AsVector3(const CU::Math::Vector4<T>& rhs)
	{
		return CU::Math::Vector3<T>(rhs.x, rhs.y, rhs.z);
	}
	


	
	bool file_exist(std::string path);

	unsigned long long nearest_Pow(unsigned long long aNum);
	unsigned long long nearest_Pow_Under(unsigned long long aNum);

	unsigned int Color32Reverse(unsigned int x);
	int MoveToRed(int x);
	int MoveToGreen(int x);

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

	/*
		Drag Coefficient
		Sphere = 0.47
		Half-Sphere = 0.42
		Cone = 0.50
		Cube = 1.05
		Angled Cube (45 deg?) 0.80
		Long Cylinder = 0.82
		Short Cylinder = 1.15
		Streamlined Body = 0.04
		Streamlined Half Body = 0.09
	*/
	inline float CalcDrag(float fluidDensity, float velocity, float dragCoefficient, float crossSectionalArea)
	{
		return (fluidDensity * 0.5f) * (velocity * velocity) * dragCoefficient * crossSectionalArea;
	}

	inline float CalcAcceleration(float gravity, float mass)
	{
		float F = mass * gravity;
		return gravity * (F / mass);
	}

	inline float CalcTerminalVelocity(float mass, float gravity, float dragCoefficient, float crossSectionalArea, float fluidDensity)
	{
		return sqrt((2 * mass * gravity) / (fluidDensity * crossSectionalArea * dragCoefficient));
	}

	//if readCharacterBeforeToFind == true it will read everything BEFORE the character/word you entered but if it is false it will read the word you entered and everything after.
	std::string substr(const std::string& aStringToReadFrom, const std::string& toFind, bool readCharactersBeforeToFind, int charsToSkip);
	bool substr(const std::string& aStringToReadFrom, const std::string& toFind);

	struct Color
	{
		Color() = default;
		Color(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
		{
			r = red;
			g = green;
			b = blue;
			a = alpha;
		};

		Color(unsigned int c) : color(c){}

		union
		{
			unsigned int color; 
			unsigned char colors[4];

			struct 
			{
				unsigned char r, g, b, a; 
			};
		};
	};


	struct SColor
	{
		SColor();
		SColor(unsigned int color);
		SColor(unsigned int red, unsigned int green, unsigned int blue, unsigned int alpha);
		unsigned int m_Red;
		unsigned int m_Green;
		unsigned int m_Blue;
		unsigned int m_Alpha;
		int _color;
		void SetRGB(unsigned int r, unsigned int g, unsigned int b)
		{
			m_Red = r;
			m_Green = g;
			m_Blue = b;
		}
		void SetA(unsigned int a)
		{
			m_Alpha = a;
		}
		void Convert(unsigned int aColor);
		void Convert(unsigned int aRed, unsigned int aGreen, unsigned int aBlue, unsigned int anAlpha);



		CU::Vector4f ToVec4()
		{
			return CU::Vector4f(
				(float)m_Red / 255.f, 
				(float)m_Green / 255.f, 
				(float)m_Blue / 255.f, 
				(float)m_Alpha / 255.f);
		}
	};

	inline CU::Vector4f IntToCol(unsigned int col_as_int)
	{
		CU::Vector4f out;
		out.z = (float)((col_as_int) bitand 0xFF);
		out.y = (float)((col_as_int >> 8) bitand 0xFF);
		out.x = (float)((col_as_int >> 16) bitand 0xFF);
		out.w = (float)((col_as_int >> 24) bitand 0xFF);
		return out;

	};

	float RadToDegree(float aRadian);
	float DegreeToRad(float aDegree);

	constexpr float _DegToRad(const float x);

	float CalcPointOnCurve(float x, float y, float t);
	CU::Vector2f Bezier(CU::Vector2f p1, CU::Vector2f p2, CU::Vector2f p3, float t);
	CU::Vector3f Bezier(CU::Vector3f p1, CU::Vector3f p2, CU::Vector3f p3, float t);
	CU::Vector3f Bezier(CU::GrowingArray<CU::Vector3f> points, float t);
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

	void GetFolders(std::vector<std::string>& result, const char* path, bool recursive);
	std::vector<std::string> FindFilesInDirectory(const char* directory_path);

	uint64 Hash(std::string key);

	

};


struct Ticket_Mutex
{
	uint64 volatile ticket = 0;
	uint64 volatile serving = 0;
};

inline uint64 AtmoicAddU64(uint64 volatile *value, uint64 toAdd)
{
	uint64 result = _InterlockedExchange64((__int64 volatile *)value, (*value + toAdd));
	return result;
};

inline void BeginTicketMutex(Ticket_Mutex* mutex)
{
	uint64 ticket = AtmoicAddU64(&mutex->ticket, 1); //AtomicAdd?
	while (ticket != mutex->serving)
	{
		//should have a timer or something here in debug to auto crash
	}

};

inline void EndTicketMutex(Ticket_Mutex* mutex)
{
	mutex->serving++;
};
