#pragma once
#include <memory.h>
#include <assert.h>
#include "../../DL_Debug/DL_Debug.h"

namespace CommonUtilities
{
	template<typename ObjectType, typename SizeType = int>
	class GrowingArray
	{
	public:
		inline GrowingArray();
		inline GrowingArray(SizeType aNrOfRecommendedItems, bool aUseSafeModeFlag = true);
		inline GrowingArray(const GrowingArray& aGrowingArray);
		inline ~GrowingArray();

		inline GrowingArray& operator=(const GrowingArray& aGrowingArray);

		inline void Init(SizeType aNrOfRecommendedItems, bool aUseSafeModeFlag = true);
		inline void ReInit(SizeType aNrOfRecommendedItems, bool aUseSafeModeFlag = true);

		inline ObjectType& operator[](const SizeType& aIndex);
		inline const ObjectType& operator[](const SizeType& aIndex) const;

		inline void Add(const ObjectType& aObject);
		inline void Insert(SizeType aIndex, ObjectType& aObject);
		inline void DeleteCyclic(ObjectType& aObject);
		inline void DeleteCyclicAtIndex(SizeType aItemNumber);
		inline void RemoveCyclic(const ObjectType& aObject);
		inline void RemoveCyclicAtIndex(SizeType aItemNumber);
		inline SizeType Find(const ObjectType& aObject);

		inline ObjectType& GetLast();
		inline const ObjectType& GetLast() const;

		static const SizeType FoundNone = static_cast<SizeType>(-1);

		inline void RemoveAll();
		inline void DeleteAll();
		inline int Capacity();

		bool Empty() { return (mySize <= 0); }

		void Optimize();

		__forceinline int Size() const;

		void Reserve(SizeType aNewSize);
		inline void Resize(SizeType aNewSize);


		typedef ObjectType* iterator;
		typedef const ObjectType* const_iterator;
		iterator begin() { return &myData[0]; }
		const_iterator begin() const { return &myData[0]; }
		iterator end() { return &myData[mySize]; }
		const_iterator end() const { return &myData[mySize]; }

	private:

		bool mySafeFlag : 1;
		bool m_HasInited : 1;
		ObjectType *myData = nullptr;
		SizeType myCapacity = 16;
		SizeType mySize = 0;
	};

	template<typename ObjectType, typename SizeType = int>
	GrowingArray<ObjectType, SizeType>::GrowingArray()
	{
		m_HasInited = false;
		myCapacity = 0;
		mySize = 0;
		myData = nullptr;
		Init(16);
	};

	template<typename ObjectType, typename SizeType = int>
	GrowingArray<ObjectType, SizeType>::~GrowingArray()
	{
		myCapacity = 0;
		mySize = 0;
		delete[]myData;
		myData = nullptr;
	};

	template<typename ObjectType, typename SizeType = int>
	GrowingArray<ObjectType, SizeType>::GrowingArray(SizeType aNrOfRecommendedItems, bool aUseSafeModeFlag = true)
	{
		Init(aNrOfRecommendedItems, aUseSafeModeFlag);
	};

	template<typename ObjectType, typename SizeType = int>
	GrowingArray<ObjectType, SizeType>::GrowingArray(const GrowingArray& aGrowingArray)
	{
		*this = aGrowingArray;
	};

	template<typename ObjectType, typename SizeType = int>
	void GrowingArray<ObjectType, SizeType>::Init(SizeType aNrOfRecommendedItems, bool aUseSafeModeFlag = true)
	{
		//assert(m_HasInited && "Already initiated!");

		mySafeFlag = aUseSafeModeFlag;
		mySize = 0;
		myCapacity = aNrOfRecommendedItems;
		myCapacity = myCapacity;
		myData = new ObjectType[myCapacity];
		m_HasInited = true;
	};

	template<typename ObjectType, typename SizeType = int>
	void GrowingArray<ObjectType, SizeType>::ReInit(SizeType aNrOfRecommendedItems, bool aUseSafeModeFlag = true)
	{
		delete[]myData;
		m_HasInited = false;
		Init(aNrOfRecommendedItems, aUseSafeModeFlag);
	};

	template<typename ObjectType, typename SizeType = int>
	GrowingArray<ObjectType, SizeType>& GrowingArray<ObjectType, SizeType>::operator=(const GrowingArray& aGrowingArray)
	{

		delete[]myData;
		mySafeFlag = aGrowingArray.mySafeFlag;
		m_HasInited = aGrowingArray.m_HasInited;
		if (mySafeFlag == false)
		{
			myCapacity = aGrowingArray.myCapacity;
			mySize = aGrowingArray.mySize;
			myData = new ObjectType[myCapacity];

			memcpy(myData, aGrowingArray.myData, sizeof(ObjectType)*aGrowingArray.mySize);
		}
		else
		{
			myCapacity = aGrowingArray.myCapacity;
			mySize = aGrowingArray.mySize;
			myData = new ObjectType[myCapacity];

			for (SizeType i = 0; i < mySize; ++i)
			{
				myData[i] = aGrowingArray[i];
			}
		}
		return *this;
	};

	template<typename ObjectType, typename SizeType = int>
	inline ObjectType& GrowingArray<ObjectType, SizeType>::operator[](const SizeType& aIndex)
	{
		assert(aIndex >= 0 && aIndex < mySize && "Out of Bounds!");
		return myData[aIndex];
	}

	template<typename ObjectType, typename SizeType = int>
	inline const ObjectType& GrowingArray<ObjectType, SizeType>::operator[](const SizeType& aIndex) const
	{
		assert(aIndex >= 0 && aIndex < mySize && "Out of Bounds!");
		return myData[aIndex];
	};

	template<typename ObjectType, typename SizeType = int>
	inline void GrowingArray<ObjectType, SizeType>::Add(const ObjectType& aObject)
	{
		DL_ASSERT_EXP(myCapacity > 0 , "Capacity is 0 or less");
		assert(this && "Growingarray not initilized. Failed to add");
		if (mySize >= myCapacity)
		{
			Resize(myCapacity * 2);
		}
		myData[mySize] = aObject;
		mySize++;
	};

	template<typename ObjectType, typename SizeType = int>
	inline void GrowingArray<ObjectType, SizeType>::Insert(SizeType aIndex, ObjectType& aObject)
	{
		assert(aIndex >= 0 && aIndex < mySize && "Out of Bounds");
		if (mySize >= myCapacity)
		{
			Resize(myCapacity * 2);
		}
		for (SizeType i = mySize; i > aIndex; i--)
		{
			myData[i] = myData[i - 1];
		}
		myData[aIndex] = aObject;
		mySize++;
	};

	template<typename ObjectType, typename SizeType = int>
	inline void GrowingArray<ObjectType, SizeType>::DeleteCyclic(ObjectType& aObject)
	{
		bool dataDeleted = false;
		SizeType i = 0;
		while (dataDeleted == false && i < mySize)
		{
			if (myData[i] == aObject)
			{
				myData[i] = myData[mySize - 1];
				mySize--;
				dataDeleted = true;
			}
			else
			{
				i++;
			}
		}
		assert(dataDeleted == true && "No Data to Delete in Vector");
	};

	template<typename ObjectType, typename SizeType = int>
	inline void GrowingArray<ObjectType, SizeType>::DeleteCyclicAtIndex(SizeType aItemNumber)
	{
		assert(aItemNumber >= 0 && aItemNumber < mySize && "Out of Bounds");
		delete myData[aItemNumber];
		myData[aItemNumber] = nullptr;

		myData[aItemNumber] = myData[mySize - 1];
		mySize--;
	};

	template<typename ObjectType, typename SizeType = int>
	inline void GrowingArray<ObjectType, SizeType>::RemoveCyclic(const ObjectType& aObject)
	{
		SizeType i = 0;

		while (i < mySize)
		{
			if (myData[i] == aObject)
			{
				myData[i] = GetLast();
				mySize--;
				break;
			}

			i++;
		}
	};

	template<typename ObjectType, typename SizeType = int>
	inline void GrowingArray<ObjectType, SizeType>::RemoveCyclicAtIndex(SizeType aItemNumber)
	{
		assert(aItemNumber >= 0 && aItemNumber < mySize && "Out of Bounds");
		myData[aItemNumber] = myData[mySize - 1];
		mySize--;
	};

	template<typename ObjectType, typename SizeType = int>
	inline SizeType GrowingArray<ObjectType, SizeType>::Find(const ObjectType& aObject)
	{
		for (SizeType i = 0; i < mySize; ++i)
		{
			if (myData[i] == aObject)
			{
				return i;
			}
		}
		return FoundNone;
	};

	template<typename ObjectType, typename SizeType = int>
	inline ObjectType& GrowingArray<ObjectType, SizeType>::GetLast()
	{
		return myData[mySize - 1];
	};

	template<typename ObjectType, typename SizeType = int>
	inline const ObjectType& GrowingArray<ObjectType, SizeType>::GetLast() const
	{
		return myData[mySize - 1];
	};

	template<typename ObjectType, typename SizeType = int>
	inline void GrowingArray<ObjectType, SizeType>::RemoveAll()
	{
		mySize = 0;
	};

	template<typename ObjectType, typename SizeType = unsigned short>
	inline void GrowingArray<ObjectType, SizeType>::DeleteAll()
	{
		for (SizeType i = 0; i < mySize; ++i)
		{
			delete myData[i];
			myData[i] = nullptr;
		} 
		mySize = 0;
	};

	template<typename ObjectType, typename SizeType = int>
	void GrowingArray<ObjectType, SizeType>::Optimize()
	{
		if (myCapacity == mySize)
			return;

			myCapacity = mySize;
			ObjectType *newMemory = new ObjectType[myCapacity];
			if (mySafeFlag == true)
			{
				for (SizeType i = 0; i < myCapacity; ++i)
				{
					newMemory[i] = myData[i];
				}
			}
			else
			{
				memcpy(newMemory, myData, sizeof(ObjectType)*mySize);
			}
			delete[]myData;

			myData = newMemory;
	};

	template<typename ObjectType, typename SizeType = int>
	__forceinline int GrowingArray<ObjectType, SizeType>::Size() const
	{
		return mySize;
	};

	template<typename ObjectType, typename SizeType = int>
	inline void GrowingArray<ObjectType, SizeType>::Resize(SizeType aNewSize)
	{

		myCapacity = aNewSize;
		ObjectType *newMemory = new ObjectType[myCapacity];
		if (mySafeFlag == true)
		{
			for (SizeType i = 0; i < mySize; ++i)
			{
				newMemory[i] = myData[i];
			}
		}
		else
		{
			memcpy(newMemory, myData, sizeof(ObjectType)*mySize);
		}
		delete[]myData;
		myData = nullptr;
		myData = newMemory;
		myCapacity = myCapacity;
	};

	template<typename ObjectType, typename SizeType = int>
	void GrowingArray<ObjectType, SizeType>::Reserve(SizeType aNewSize)
	{
		myCapacity = aNewSize;

		ObjectType *newMemory = new ObjectType[myCapacity];
		if (mySafeFlag == true)
		{
			for (SizeType i = 0; i < mySize; ++i)
			{
				newMemory[i] = myData[i];
			}
		}
		else
		{
			memcpy(newMemory, myData, sizeof(ObjectType)*mySize);
		}
		delete[]myData;

		myData = newMemory;
		mySize = myCapacity;
	}

	template<typename ObjectType, typename SizeType = int>
	int GrowingArray<ObjectType, SizeType>::Capacity()
	{
		return myCapacity;
	}

};

namespace CU = CommonUtilities;