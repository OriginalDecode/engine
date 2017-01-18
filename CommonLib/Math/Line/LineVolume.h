#pragma once
#include "Vector.h"
#include "GrowingArray.h"
#include "Line.h"
namespace CommonUtilities
{
	template<typename TYPE>
	class LineVolume
	{
	public:
		LineVolume() = default;
		LineVolume(GrowingArray<Line<TYPE>> aLineList);

		void AddLine(Line<TYPE> aLine);
		void RemoveLine(Line<TYPE> aLine);
		
		bool Inside(Vector2<TYPE> aPosition);
	private:

		GrowingArray<Line<TYPE>, int> myLines;

	};

	template<typename TYPE>
	LineVolume<TYPE>::LineVolume(GrowingArray<Line<TYPE>> aLineList)
	{
		myLines = aLineList;
	}

	template<typename TYPE>
	void LineVolume<TYPE>::AddLine(Line<TYPE> aLine)
	{
		myLines.Add(aLine);
	}

	template<typename TYPE>
	void LineVolume<TYPE>::RemoveLine(Line<TYPE> aLine)
	{
		myLines.RemoveCyclic(aLine);
	}

	template<typename TYPE>
	bool LineVolume<TYPE>::Inside(Vector2<TYPE> aPosition)
	{
		for (unsigned int i = 0; i < myLines.Size(); ++i)
		{
			if (myLines[i].Inside(aPosition) == false)
				return false;
		}
		return true;
	}
};
