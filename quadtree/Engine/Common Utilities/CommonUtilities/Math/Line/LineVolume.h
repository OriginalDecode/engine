#pragma once
#include <vector>
#include "Line.h"
#include "..\..\Misc\Global.h"
namespace CommonUtilities
{
	template<typename TYPE>
	class LineVolume
	{
	public:
		LineVolume();
		LineVolume(std::vector<Line<TYPE>> aLineList);
		~LineVolume();

		void AddLine(Line<TYPE> aLine);
		void RemoveLine(Line<TYPE> aLine);

		bool Inside(CU::Math::Vector2<TYPE> aPosition);
		const bool Inside(CU::Math::Vector2<TYPE> aPosition, const float aRadius) const;
		
		std::vector<Line<TYPE>> myLines;

	};

	template<typename TYPE>
	LineVolume<TYPE>::LineVolume()
	{
	}

	template<typename TYPE>
	LineVolume<TYPE>::~LineVolume()
	{
	}

	template<typename TYPE>
	LineVolume<TYPE>::LineVolume(std::vector<Line<TYPE>> aLineList)
	{
		myLines = aLineList;
	}

	template<typename TYPE>
	void LineVolume<TYPE>::AddLine(Line<TYPE> aLine)
	{
		myLines.push_back(aLine);
	}

	template<typename TYPE>
	void LineVolume<TYPE>::RemoveLine(Line<TYPE> aLine)
	{
		for (unsigned int i = 0; i < myLines.size(); i++)
		{
			if (myLines[i].GetPoint() == aLine.GetPoint() && myLines[i].GetNormal() == aLine.GetNormal())
			{
				myLines.erase(myLines.begin() + i);
				return;
			}
		}
	}

	template<typename TYPE>
	bool LineVolume<TYPE>::Inside(CU::Math::Vector2<TYPE> aPosition)
	{
		for (unsigned int i = 0; i < myLines.size(); ++i)
		{
			if (myLines[i].Inside(aPosition) == false)
				return false;
		}
		return true;
	}

	template <typename TYPE>
	const bool LineVolume<TYPE>::Inside(CU::Math::Vector2<TYPE> aPosition, const float aRadius) const
	{
		for (unsigned int i = 0; i < myLines.size(); ++i)
		{
			if (myLines[i].Inside(aPosition, aRadius) == false)
				return false;
		}

		return true;
	}
};
