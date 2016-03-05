#pragma once
#include <assert.h>

namespace CommonUtilities
{
	template <typename T, int Size>
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

		T myData[Size];

	};
	
	template<typename T, int Size>
	StaticArray<T, Size>::StaticArray()
	{
	}
	template <typename T, int Size>
	StaticArray<T, Size>::~StaticArray()
	{
	}
	template <typename T, int Size>
	StaticArray<T, Size>::StaticArray(const StaticArray& aStaticArray)
	{
		*this = aStaticArray;
	}

	template <typename T, int Size>
	StaticArray<T, Size>& StaticArray<T, Size>::operator=(const StaticArray& aStaticArray)
	{
		for (unsigned int i = 0; i < Size; ++i)
		{
			myData[i] = aStaticArray[i];
		}
		return *this;
	}

	template <typename T, int Size>
	inline const T& StaticArray<T, Size>::operator[](const int& anIndex) const
	{
		assert(anIndex >= 0 && anIndex < Size && "Index is less than 0! [Out of Bounds]");
		assert(Size > anIndex && "Index is greater than size");
		return myData[anIndex];
	}

	template <typename T, int Size>
	inline T& StaticArray<T, Size>::operator[](const int& anIndex)
	{
		assert(anIndex >= 0 && anIndex < Size && "Index is less than 0! [Out of Bounds]");
		assert(Size > anIndex && "Index is greater than size");

		return myData[anIndex];
	}

	template <typename T, int Size>
	inline void StaticArray<T, Size>::Insert(int anIndex, T& anObject)
	{
		assert(anIndex >= 0 && anIndex < Size && "Index is less than 0! [Out of Bounds]");
		assert(Size > anIndex && "Index is greater than size");
		for (int i = (Size - 1); i > anIndex; i--)
		{
			myData[i] = myData[i - 1];
		}
		myData[anIndex] = anObject;
	}

	template <typename T, int Size>
	inline void StaticArray<T, Size>::DeleteAll()
	{
		for (int i = 0; i < Size; ++i)
		{
			SAFE_DELETE(myData[i]);
		}
	}

	template <typename T, int Size>
	inline int StaticArray<T, Size>::Capacity()
	{
		return Size;
	}
}
namespace CU = CommonUtilities;