#include "Utilities.h"

namespace cl
{
	//if readCharacterBeforeToFind == true it will read everything BEFORE the character/word you entered but if it is false it will read the word you entered and everything after.
	std::string substr(const std::string& aStringToReadFrom, const std::string& toFind, bool readCharactersBeforeToFind, int charsToSkip)
	{
		std::string toCheck;
		if (aStringToReadFrom.rfind(toFind) != aStringToReadFrom.npos)
		{
			if (readCharactersBeforeToFind == true)
			{
				return aStringToReadFrom.substr(charsToSkip, aStringToReadFrom.rfind(toFind));
			}
			return aStringToReadFrom.substr(aStringToReadFrom.rfind(toFind));
		}
		return aStringToReadFrom;
	}

	bool substr(const std::string& aStringToReadFrom, const std::string& toFind)
	{
		if (aStringToReadFrom.rfind(toFind) != aStringToReadFrom.npos)
		{
			return true;
		}
		return false;
	}

	bool file_exist(std::string path)
	{
		std::ifstream stream(path.c_str());
		return stream.good();
	}

	unsigned long long nearest_Pow(unsigned long long aNum)
	{
		aNum--;
		aNum |= aNum >> 1;
		aNum |= aNum >> 2;
		aNum |= aNum >> 4;
		aNum |= aNum >> 8;
		aNum |= aNum >> 16;
		aNum++;
		return aNum;
	}

	unsigned long long nearest_Pow_Under(unsigned long long aNum)
	{
		return nearest_Pow(aNum) / 2;
	}

	// AA RR GG BB
	unsigned int Color32Reverse(unsigned int x)
	{
		return
			((x & 0xFF000000) >> 24) |
			((x & 0x00FF0000) >> 8) |
			((x & 0x0000FF00) << 8) |
			((x & 0x000000FF) << 24);
	}

	int MoveToRed(int x)
	{
		return
			((x & 0xFF000000) >> 24) |
			((x & 0x00FF0000) >> 8) |
			((x & 0x0000FF00) << 16) |
			((x & 0x000000FF) << 16);
	}

	int MoveToGreen(int x)
	{
		return
			((x & 0xFF000000) >> 24) |
			((x & 0x00FF0000) >> 8) |
			((x & 0x0000FF00) << 16) |
			((x & 0x000000FF) << 8);
	}

	float ClampF(float value, float min, float max)
	{
		if (value > max)
			return max;

		if (value < min)
			return min;

		return value;
	}

	int ClampI(int value, int min, int max)
	{
		if (value > max)
			return max;

		if (value < min)
			return min;

		return value;
	}

	SColor::SColor(unsigned int color)
		: m_Red(0)
		, m_Green(0)
		, m_Blue(0)
		, m_Alpha(0)
		, _color(0)
	{
		m_Alpha = (color) & 0xFF;
		m_Red = (color >> 8) & 0xFF;
		m_Green = (color >> 16) & 0xFF;
		m_Blue = (color >> 24) & 0xFF;
		_color = color;
	}

	SColor::SColor(unsigned int red, unsigned int green, unsigned int blue, unsigned int alpha)
	{
		_color = (red << 24) + (green << 16) + (blue << 8) + (alpha);
	}

	SColor::SColor()
		: m_Red(255)
		, m_Green(255)
		, m_Blue(255)
		, m_Alpha(255)
		, _color(0)
	{
		_color = (m_Red << 24) + (m_Green << 16) + (m_Blue << 8) + (m_Alpha);
	}

	void SColor::Convert(unsigned int aColor)
	{
		m_Alpha = (aColor) & 0xFF;
		m_Blue = (aColor >> 8) & 0xFF;
		m_Green = (aColor >> 16) & 0xFF;
		m_Red = (aColor >> 24) & 0xFF;
		_color = aColor;
	}

	void SColor::Convert(unsigned int aRed, unsigned int aGreen, unsigned int aBlue, unsigned int anAlpha)
	{
		_color = (aRed << 24) + (aGreen << 16) + (aBlue << 8) + (anAlpha);
	}

	float RadToDegree(float aRadian)
	{
		return aRadian = aRadian * (180 / 3.1415926535f);
	}

	float DegreeToRad(float aDegree)
	{
		return aDegree = aDegree * (3.1415926535f / 180);
	}



	CU::Vector2f Bezier(CU::Vector2f p1, CU::Vector2f p2, CU::Vector2f p3, float t)
	{
		float xa = CalcPointOnCurve(p1.x, p2.x, t);
		float ya = CalcPointOnCurve(p1.y, p2.y, t);

		float xb = CalcPointOnCurve(p2.x, p3.x, t);
		float yb = CalcPointOnCurve(p2.y, p3.y, t);

		CU::Vector2f toReturn;
		toReturn.x = CalcPointOnCurve(xa, xb, t);
		toReturn.y = CalcPointOnCurve(ya, yb, t);
		return toReturn;
	}

	CU::Vector3f Bezier(CU::Vector3f p1, CU::Vector3f p2, CU::Vector3f p3, float t)
	{
		float u = 1.f - t;

		float tt = t * t;

		float uu = u * u;

		float uuu = uu * u;

		float ttt = tt * t;

		CU::Vector3f toReturn = p1 * uuu;
		toReturn += p2 * (3 * uu * t);
		toReturn += p3 * (3 * u * tt);
		toReturn += p3 * ttt;
		return toReturn;
	}



	CU::Vector3f CubicBezier(CU::Vector3f p1, CU::Vector3f p2, CU::Vector3f p3, CU::Vector3f p4, float t)
	{
		float u = 1.f - t;

		float tt = t * t;

		float uu = u * u;

		float uuu = u * u * u;

		float ttt = t* t * t;

		CU::Vector3f toReturn = p1 * uuu;
		toReturn += p2 * (3 * uu * t);
		toReturn += p3 * (3 * u * tt);
		toReturn += p4 * ttt;
		return toReturn;
	}

	float CalcPointOnCurve(float x, float y, float t)
	{
		float diff = y - x;
		return x + (diff * t);
	}


	unsigned int binomialCoef(int n, int k)
	{
		int r = 1;
		if (k > n)
			return 0;

		for (int d = 1; d <= k; d++)
		{
			r *= n--;
			r /= d;
		}
		return r;
	}
};


#include <Windows.h>
#include <thread>

#pragma region ThreadNaming
const DWORD MS_VC_EXCEPTION = 0x406D1388;
#pragma pack(push,8)
typedef struct tagTHREADNAME_INFO
{
	DWORD dwType; // Must be 0x1000.
	LPCSTR szName; // Pointer to name (in user addr space).
	DWORD dwThreadID; // Thread ID (-1=caller thread).
	DWORD dwFlags; // Reserved for future use, must be zero.
} THREADNAME_INFO;
#pragma pack(pop)
void SetThreadName(DWORD dwThreadID, const char* threadName)
{
	THREADNAME_INFO info;
	info.dwType = 0x1000;
	info.szName = threadName;
	info.dwThreadID = dwThreadID;
	info.dwFlags = 0;
#pragma warning(push)
#pragma warning(disable: 6320 6322)
	__try {
		RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
	}
#pragma warning(pop)
}

#include <vadefs.h>
#include <stdarg.h>

namespace cl
{
	void SetThreadName(const std::thread::id& id, const char* name)
	{

		std::stringstream ss;
		ss << id;
		DWORD word_id;
		ss >> word_id;
		::SetThreadName(word_id, name);
	}

	std::string HandleVAArgs(const char* formatted_string, ...)
	{
		char buffer[SHRT_MAX];
		va_list args;
		va_start(args, formatted_string);
		vsprintf_s(buffer, formatted_string, args);
		va_end(args);
		return buffer;
	}

	const std::wstring ToWideStr(const std::string& str)
	{
		 return std::wstring(str.begin(), str.end()); 
	}

}