#pragma once
#include <Windows.h>
#include <assert.h>

namespace CommonUtilities
{
	template <typename Type, int MaxSize, typename CountType = unsigned short, bool UseSafeModeFlag = true>
	class VectorOnStack
	{
	public:
		VectorOnStack();
		VectorOnStack(const VectorOnStack& aVectorOnStack);

		~VectorOnStack();

		VectorOnStack & operator=(const VectorOnStack & aVectorOnStack);

		inline const Type& operator[](const CountType & aIndex) const;
		inline Type& operator[](const CountType & aIndex);

		inline void Add(const Type& aObject);
		inline void Insert(CountType aIndex, Type& aObject);

		inline void DeleteCyclic(Type& aObject);
		inline void DeleteCyclicAtIndex(CountType aItemNumber);

		inline void RemoveCyclic(const Type& aObject);
		inline void RemoveCyclicAtIndex(CountType aItemNumber);

		inline void Clear();
		inline void DeleteAll();

		inline CountType Size() const;

	private:

		CountType myCurrSize;
		Type myData[MaxSize];
		bool mySafeFlag;
	};

	template <typename Type, int MaxSize, typename CountType = unsigned short, bool UseSafeModeFlag = true>
	VectorOnStack<Type, MaxSize, CountType, UseSafeModeFlag>::VectorOnStack()
	{
		myCurrSize = 0;
		mySafeFlag = UseSafeModeFlag;
	}
	template <typename Type, int MaxSize, typename CountType = unsigned short, bool UseSafeModeFlag = true>
	VectorOnStack<Type, MaxSize, CountType, UseSafeModeFlag>::~VectorOnStack()
	{
	}

	template <typename Type, int MaxSize, typename CountType = unsigned short, bool UseSafeModeFlag = true>
	VectorOnStack<Type, MaxSize, CountType, UseSafeModeFlag>::VectorOnStack(const VectorOnStack& aVectorOnStack)
	{
		*this = aVectorOnStack;
	}

	template <typename Type, int MaxSize, typename CountType = unsigned short, bool UseSafeModeFlag = true>
	VectorOnStack<Type, MaxSize, CountType, UseSafeModeFlag>& VectorOnStack<Type, MaxSize, CountType, UseSafeModeFlag>::operator=(const VectorOnStack& aVectorOnStack)
	{
		if (mySafeFlag == false)
		{
			memcpy(this, &aVectorOnStack, sizeof(*this));
		}
		else
		{
			for (CountType i = 0; i < aVectorOnStack.myCurrSize; ++i)
			{
				myData[i] = aVectorOnStack[i];

			}
			myCurrSize = aVectorOnStack.myCurrSize;
			mySafeFlag = aVectorOnStack.mySafeFlag;
		}

		return *this;

	}

	template <typename Type, int MaxSize, typename CountType = unsigned short, bool UseSafeModeFlag = true>
	inline const Type& VectorOnStack<Type, MaxSize, CountType, UseSafeModeFlag>::operator[](const CountType& anIndex) const
	{
		assert(anIndex >= 0 && anIndex < myCurrSize && "Out of Bounds!");
		return myData[anIndex];
	}

	template <typename Type, int MaxSize, typename CountType = unsigned short, bool UseSafeModeFlag = true>
	inline Type& VectorOnStack<Type, MaxSize, CountType, UseSafeModeFlag>::operator[](const CountType& anIndex)
	{
		assert(anIndex >= 0 && anIndex < myCurrSize && "Out of Bounds!");
		return myData[anIndex];
	}

	template <typename Type, int MaxSize, typename CountType = unsigned short, bool UseSafeModeFlag = true>
	inline void VectorOnStack<Type, MaxSize, CountType, UseSafeModeFlag>::Add(const Type& aObject)
	{
		assert(MaxSize > myCurrSize && "Out of Bounds!");
		myData[myCurrSize] = aObject;
		myCurrSize++;
	}

	template <typename Type, int MaxSize, typename CountType = unsigned short, bool UseSafeModeFlag = true>
	inline void VectorOnStack<Type, MaxSize, CountType, UseSafeModeFlag>::Insert(CountType anIndex, Type& aObject)
	{
		assert(anIndex >= 0 && myCurrSize < MaxSize && anIndex < myCurrSize && "Out of Bounds! /104");
		for (int i = (MaxSize - 1); i > anIndex; i--)
		{
			myData[i] = myData[i - 1];
		}
		myData[anIndex] = aObject;
		myCurrSize++;
	}

	template <typename Type, int MaxSize, typename CountType = unsigned short, bool UseSafeModeFlag = true>
	inline void VectorOnStack<Type, MaxSize, CountType, UseSafeModeFlag>::DeleteCyclic(Type& aObject)
	{
		bool dataDeleted = false;
		int i = 0;
		while (dataDeleted == false && i < myCurrSize)
		{

			if (myData[i] == aObject)
			{
				SAFE_DELETE(myData[i]);
				myData[i] = myData[myCurrSize - 1];
				myCurrSize--;
				dataDeleted = true;
			}
			else
			{
				i++;
			}
		}
		assert(dataDeleted == true && "No Data to Delete in Vector");
	}

	template <typename Type, int MaxSize, typename CountType = unsigned short, bool UseSafeModeFlag = true>
	inline void VectorOnStack<Type, MaxSize, CountType, UseSafeModeFlag>::DeleteCyclicAtIndex(CountType aItemNumber)
	{
		SAFE_DELETE(myData[aItemNumber]);
		myData[aItemNumber] = myData[myCurrSize - 1];
		myCurrSize--;

	}

	template <typename Type, int MaxSize, typename CountType = unsigned short, bool UseSafeModeFlag = true>
	inline void VectorOnStack<Type, MaxSize, CountType, UseSafeModeFlag>::RemoveCyclic(const Type& aObject)
	{
		bool dataDeleted = false;
		int i = 0;
		while (dataDeleted == false && i < myCurrSize)
		{
			if (myData[i] == aObject)
			{
				myData[i] = myData[myCurrSize - 1];
				myCurrSize--;
				dataDeleted = true;
			}
			else
			{
				i++;
			}
		}
		assert(dataDeleted == true && "No Data to Delete in Vector");
	}

	template <typename Type, int MaxSize, typename CountType = unsigned short, bool UseSafeModeFlag = true>
	inline void VectorOnStack<Type, MaxSize, CountType, UseSafeModeFlag>::RemoveCyclicAtIndex(CountType aItemNumber)
	{
		assert(aItemNumber >= 0 && aItemNumber < myCurrSize && "Out of Bounds!");
		myData[aItemNumber] = myData[myCurrSize - 1];
		myCurrSize--;

	}

	template <typename Type, int MaxSize, typename CountType = unsigned short, bool UseSafeModeFlag = true>
	inline void VectorOnStack<Type, MaxSize, CountType, UseSafeModeFlag>::Clear()
	{
		myCurrSize = 0;
	}

	template <typename Type, int MaxSize, typename CountType = unsigned short, bool UseSafeModeFlag = true>
	inline void VectorOnStack<Type, MaxSize, CountType, UseSafeModeFlag>::DeleteAll()
	{
		for (int i = 0; i < myCurrSize; ++i)
		{
			SAFE_DELETE(myData[i]);
		}
		myCurrSize = 0;
	}

	template <typename Type, int MaxSize, typename CountType = unsigned short, bool UseSafeModeFlag = true>
	inline CountType VectorOnStack<Type, MaxSize, CountType, UseSafeModeFlag>::Size() const
	{
		return myCurrSize;
	}
};
namespace CU = CommonUtilities;
