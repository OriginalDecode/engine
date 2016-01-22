#pragma once
#include "..\Vector\Vector.h"

namespace CommonUtilities
{
	template<typename T>
	class Line
	{
	public:
		Line();
		Line(CU::Math::Vector2<T>aFirstPoint, CU::Math::Vector2<T> aSecondPoint);
		~Line();

		void InitWith2Points(CU::Math::Vector2<T> aFirstPoint, CU::Math::Vector2<T> aSecondPoint);
		void InitWithPointAndDirection(CU::Math::Vector2<T> aPoint, CU::Math::Vector2<T> aDirection);
		bool Inside(CU::Math::Vector2<T> aPosition);
		const bool Inside(CU::Math::Vector2<T> aPosition, const T aRadius) const;
		bool operator==(Line<T> aLine);


		Math::Vector3<T> myData;
		Math::Vector2<T> myStart;
		Math::Vector2<T> myEnd;
		Math::Vector2<T> myNormal;


	};

	template<typename T>
	Line<T>::Line()
	{
	}

	template<typename T>
	Line<T>::~Line()
	{
	}

	template<typename T>
	Line<T>::Line(CU::Math::Vector2<T> aFirstPoint, CU::Math::Vector2<T> aSecondPoint)
	{
		InitWith2Points(aFirstPoint, aSecondPoint);
	}

	template<typename T>
	void Line<T>::InitWith2Points(CU::Math::Vector2<T> aFirstPoint, CU::Math::Vector2<T>aSecondPoint)
	{

		Math::Vector2<T> line = aSecondPoint - aFirstPoint;
		Math::Vector2<T> normal = Math::Vector2<T>(-line.y, line.x);
		Math::Normalize(normal);

		myStart = aFirstPoint;
		myEnd = aSecondPoint;
		myNormal = normal;
		
		//T oldX = normal.x;

		//normal.x = -normal.y;
		//normal.y = oldX;

		myData.x = normal.x;
		myData.y = normal.y;
		myData.z = Math::Dot(aFirstPoint, normal);
	}

	template<typename T>
	void Line<T>::InitWithPointAndDirection(CU::Math::Vector2<T> aPoint, const CU::Math::Vector2<T> aDirection)
	{
		CU::Math::Normalize(aDirection);

		T oldX = aDirection.x;

		aDirection.x = -aDirection.y;
		aDirection.y = oldX;

		myData = { (aDirection.x, aDirection.y) };
		myData.z = CU::Math::Dot(aDirection, aPoint);

	}

	template<typename T>
	bool Line<T>::Inside(CU::Math::Vector2<T> aPosition)
	{
		//CU::Math::Vector2<T> normal(myData.x, myData.y);
		//CU::Math::Vector2<T> point = aPosition - (normal*myData.z);
		//

		if (CU::Math::Dot(aPosition, Vector2f(myData.x, myData.y)) < myData.z)
		{
			return true;
		}
		return false;
	}

	template <typename T>
	const bool Line<T>::Inside(Math::Vector2<T> aPosition, const T aRadius) const
	{
		//Math::Vector2<float> normal = (myData.x, myData.y);
		Math::Vector2<float> normal = myEnd - myStart;
		float tempNormalX = normal.x;

		normal.x = -normal.y;
		normal.y = tempNormalX;

		
		Math::Vector2<float> line = aPosition - myStart;
		
		if (Math::Dot(normal, line) > 0)
		{
			return true;
		}

		float length = Math::Dot(normal, normal);
		Math::Normalize(normal);

		float d1 = Math::Dot(normal, myStart);
		float d2 = Math::Dot(normal, aPosition);

		return abs(d1 - d2) < aRadius;
	}


	template<typename T>
	bool Line<T>::operator==(Line<T> aLine)
	{
		if (myData == aLine.myData)
		{
			return true;
		}
		return false;
	}
};

namespace CU = CommonUtilities;