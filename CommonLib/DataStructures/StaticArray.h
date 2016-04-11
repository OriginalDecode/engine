#pragma once
#include <assert.h>

namespace CommonUtilities
{
	template <typename T, int SizeType = 8>
	class StaticArray
	{
	public:
		StaticArray();
		StaticArray(const StaticArray& aStaticArray);

		~StaticArray();

		StaticArray& operator=(const StaticArray& aStaticArray);
		inline const T& operator[](const int& aIndex) const;
		inline T& operator[](const int& aIndex);

		int Capacity();

		// Utility functions
		inline void Insert(int anIndex, T& aObject);
		inline void DeleteAll();

	private:

		T myData[SizeType];

	};
	
	template<typename T, int SizeType>
	StaticArray<T, SizeType>::StaticArray()
	{
	}
	template <typename T, int SizeType>
	StaticArray<T, SizeType>::~StaticArray()
	{
	}
	template <typename T, int SizeType>
	StaticArray<T, SizeType>::StaticArray(const StaticArray& aStaticArray)
	{
		*this = aStaticArray;
	}

	template <typename T, int SizeType>
	StaticArray<T, SizeType>& StaticArray<T, SizeType>::operator=(const StaticArray& aStaticArray)
	{
		for (unsigned int i = 0; i < SizeType; ++i)
		{
			myData[i] = aStaticArray[i];
		}
		return *this;
	}

	template <typename T, int SizeType>
	inline const T& StaticArray<T, SizeType>::operator[](const int& anIndex) const
	{
		assert(anIndex >= 0 && anIndex < SizeType && "Index is less than 0! [Out of Bounds]");
		assert(SizeType > anIndex && "Index is greater than size");
		return myData[anIndex];
	}

	template <typename T, int SizeType>
	inline T& StaticArray<T, SizeType>::operator[](const int& anIndex)
	{
		assert(anIndex >= 0 && anIndex < SizeType && "Index is less than 0! [Out of Bounds]");
		assert(SizeType > anIndex && "Index is greater than size");

		return myData[anIndex];
	}

	template <typename T, int SizeType>
	inline void StaticArray<T, SizeType>::Insert(int anIndex, T& anObject)
	{
		assert(anIndex >= 0 && anIndex < SizeType && "Index is less than 0! [Out of Bounds]");
		assert(SizeType > anIndex && "Index is greater than size");
		for (int i = (SizeType - 1); i > anIndex; i--)
		{
			myData[i] = myData[i - 1];
		}
		myData[anIndex] = anObject;
	}

	template <typename T, int SizeType>
	inline void StaticArray<T, SizeType>::DeleteAll()
	{
		for (int i = 0; i < SizeType; ++i)
		{
			delete myData[i];
			myData[i] = nullptr;
		}
	}

	template <typename T, int SizeType>
	inline int StaticArray<T, SizeType>::Capacity()
	{
		return SizeType;
	}
}
namespace CU = CommonUtilities;