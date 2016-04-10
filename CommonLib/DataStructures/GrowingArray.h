#pragma once
#include <memory.h>
#include <assert.h>

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

		void Optimize();

		__forceinline int Size() const;

		void Reserve(SizeType aNewSize);
		inline void Resize(SizeType aNewSize);

	private:

		bool mySafeFlag;
		ObjectType *myObjectData;
		SizeType myMaxSize;
		SizeType myCapacity;
		SizeType myAmountOfObjects;
	};

	template<typename ObjectType, typename SizeType = int>
	GrowingArray<ObjectType, SizeType>::GrowingArray()
	{
		myMaxSize = 0;
		myAmountOfObjects = 0;
		myObjectData = nullptr;
		Init(16, true); //Default initialization of GrowingArrays. Size will start at 16 if needed Resize can be called
	};

	template<typename ObjectType, typename SizeType = int>
	GrowingArray<ObjectType, SizeType>::~GrowingArray()
	{
		myMaxSize = 0;
		myAmountOfObjects = 0;
		delete[]myObjectData;
		myObjectData = nullptr;
	};

	template<typename ObjectType, typename SizeType = int>
	GrowingArray<ObjectType, SizeType>::GrowingArray(SizeType aNrOfRecommendedItems, bool aUseSafeModeFlag = true)
	{
		Init(aNrOfRecommendedItems, aUseSafeModeFlag);
	};

	template<typename ObjectType, typename SizeType = int>
	GrowingArray<ObjectType, SizeType>::GrowingArray(const GrowingArray& aGrowingArray)
	{
		myObjectData = nullptr;
		*this = aGrowingArray;
	};

	template<typename ObjectType, typename SizeType = int>
	void GrowingArray<ObjectType, SizeType>::Init(SizeType aNrOfRecommendedItems, bool aUseSafeModeFlag = true)
	{
		mySafeFlag = aUseSafeModeFlag;
		myAmountOfObjects = 0;
		myMaxSize = aNrOfRecommendedItems;
		myCapacity = myMaxSize;
		myObjectData = new ObjectType[myMaxSize];
	};

	template<typename ObjectType, typename SizeType = int>
	void GrowingArray<ObjectType, SizeType>::ReInit(SizeType aNrOfRecommendedItems, bool aUseSafeModeFlag = true)
	{
		delete[]myObjectData;
		Init(aNrOfRecommendedItems, aUseSafeModeFlag);
	};

	template<typename ObjectType, typename SizeType = int>
	GrowingArray<ObjectType, SizeType>& GrowingArray<ObjectType, SizeType>::operator=(const GrowingArray& aGrowingArray)
	{
		delete[]myObjectData;
		mySafeFlag = aGrowingArray.mySafeFlag;
		if (mySafeFlag == false)
		{
			myMaxSize = aGrowingArray.myMaxSize;
			myAmountOfObjects = aGrowingArray.myAmountOfObjects;
			myObjectData = new ObjectType[myMaxSize];

			memcpy(myObjectData, aGrowingArray.myObjectData, sizeof(ObjectType)*aGrowingArray.myAmountOfObjects);
		}
		else
		{
			myMaxSize = aGrowingArray.myMaxSize;
			myAmountOfObjects = aGrowingArray.myAmountOfObjects;
			myObjectData = new ObjectType[myMaxSize];

			for (SizeType i = 0; i < myAmountOfObjects; ++i)
			{
				myObjectData[i] = aGrowingArray[i];
			}
		}
		return *this;
	};

	template<typename ObjectType, typename SizeType = int>
	inline ObjectType& GrowingArray<ObjectType, SizeType>::operator[](const SizeType& aIndex)
	{
		assert(aIndex >= 0 && aIndex < myAmountOfObjects && "Out of Bounds!");
		return myObjectData[aIndex];
	}

	template<typename ObjectType, typename SizeType = int>
	inline const ObjectType& GrowingArray<ObjectType, SizeType>::operator[](const SizeType& aIndex) const
	{
		assert(aIndex >= 0 && aIndex < myAmountOfObjects && "Out of Bounds!");
		return myObjectData[aIndex];
	};

	template<typename ObjectType, typename SizeType = int>
	inline void GrowingArray<ObjectType, SizeType>::Add(const ObjectType& aObject)
	{
		if (myAmountOfObjects >= myMaxSize)
		{
			Resize(myMaxSize * 2);
		}
		myObjectData[myAmountOfObjects] = aObject;
		myAmountOfObjects++;
	};

	template<typename ObjectType, typename SizeType = int>
	inline void GrowingArray<ObjectType, SizeType>::Insert(SizeType aIndex, ObjectType& aObject)
	{

		assert(aIndex >= 0 && aIndex < myAmountOfObjects && "Out of Bounds");
		if (myAmountOfObjects >= myMaxSize)
		{
			Resize(myMaxSize * 2);
		}
		for (SizeType i = myAmountOfObjects; i > aIndex; i--)
		{
			myObjectData[i] = myObjectData[i - 1];
		}
		myObjectData[aIndex] = aObject;
		myAmountOfObjects++;
	};

	template<typename ObjectType, typename SizeType = int>
	inline void GrowingArray<ObjectType, SizeType>::DeleteCyclic(ObjectType& aObject)
	{
		bool dataDeleted = false;
		SizeType i = 0;
		while (dataDeleted == false && i < myAmountOfObjects)
		{
			if (myObjectData[i] == aObject)
			{
				myObjectData[i] = myObjectData[myAmountOfObjects - 1];
				myAmountOfObjects--;
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
		assert(aItemNumber >= 0 && aItemNumber < myAmountOfObjects && "Out of Bounds");
		myObjectData[aItemNumber] = nullptr;
		delete myObjectData[aItemNumber];

		myObjectData[aItemNumber] = myObjectData[myAmountOfObjects - 1];
		myAmountOfObjects--;
	};

	template<typename ObjectType, typename SizeType = int>
	inline void GrowingArray<ObjectType, SizeType>::RemoveCyclic(const ObjectType& aObject)
	{
		bool dataDeleted = false;
		SizeType i = 0;
		while (dataDeleted == false && i < myAmountOfObjects)
		{
			if (myObjectData[i] == aObject)
			{
				myObjectData[i] = myObjectData[myAmountOfObjects - 1];
				myAmountOfObjects--;
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
	inline void GrowingArray<ObjectType, SizeType>::RemoveCyclicAtIndex(SizeType aItemNumber)
	{
		assert(aItemNumber >= 0 && aItemNumber < myAmountOfObjects && "Out of Bounds");
		myObjectData[aItemNumber] = myObjectData[myAmountOfObjects - 1];
		myAmountOfObjects--;
	};

	template<typename ObjectType, typename SizeType = int>
	inline SizeType GrowingArray<ObjectType, SizeType>::Find(const ObjectType& aObject)
	{
		for (SizeType i = 0; i < myAmountOfObjects; ++i)
		{
			if (myObjectData[i] == aObject)
			{
				return i;
			}
		}
		return FoundNone;
	};

	template<typename ObjectType, typename SizeType = int>
	inline ObjectType& GrowingArray<ObjectType, SizeType>::GetLast()
	{
		return myObjectData[myAmountOfObjects - 1];
	};

	template<typename ObjectType, typename SizeType = int>
	inline const ObjectType& GrowingArray<ObjectType, SizeType>::GetLast() const
	{
		return myObjectData[myAmountOfObjects - 1];
	};

	template<typename ObjectType, typename SizeType = int>
	inline void GrowingArray<ObjectType, SizeType>::RemoveAll()
	{
		myAmountOfObjects = 0;
	};

	template<typename ObjectType, typename SizeType = unsigned short>
	inline void GrowingArray<ObjectType, SizeType>::DeleteAll()
	{
		for (SizeType i = 0; i < myAmountOfObjects; ++i)
		{
			delete myObjectData[i];
			myObjectData[i] = nullptr;
		}
		myAmountOfObjects = 0;
	};

	template<typename ObjectType, typename SizeType = int>
	void GrowingArray<ObjectType, SizeType>::Optimize()
	{
		if (myMaxSize > myAmountOfObjects)
		{
			myMaxSize = myAmountOfObjects;
			ObjectType *newMemory = new ObjectType[myMaxSize];
			if (mySafeFlag == true)
			{
				for (SizeType i = 0; i < myMaxSize; ++i)
				{
					newMemory[i] = myObjectData[i];
				}
			}
			else
			{
				memcpy(newMemory, myObjectData, sizeof(ObjectType)*myAmountOfObjects);
			}
			delete[]myObjectData;

			myObjectData = newMemory;
		}
	};

	template<typename ObjectType, typename SizeType = int>
	__forceinline int GrowingArray<ObjectType, SizeType>::Size() const
	{
		return myAmountOfObjects;
	};

	template<typename ObjectType, typename SizeType = int>
	inline void GrowingArray<ObjectType, SizeType>::Resize(SizeType aNewSize)
	{

		myMaxSize = aNewSize;
		ObjectType *newMemory = new ObjectType[myMaxSize];
		if (mySafeFlag == true)
		{
			for (SizeType i = 0; i < myAmountOfObjects; ++i)
			{
				newMemory[i] = myObjectData[i];
			}
		}
		else
		{
			memcpy(newMemory, myObjectData, sizeof(ObjectType)*myAmountOfObjects);
		}
		delete[]myObjectData;

		myObjectData = newMemory;
		myCapacity = myMaxSize;
	};

	template<typename ObjectType, typename SizeType = int>
	void GrowingArray<ObjectType, SizeType>::Reserve(SizeType aNewSize)
	{
		myMaxSize = aNewSize;

		ObjectType *newMemory = new ObjectType[myMaxSize];
		if (mySafeFlag == true)
		{
			for (SizeType i = 0; i < myAmountOfObjects; ++i)
			{
				newMemory[i] = myObjectData[i];
			}
		}
		else
		{
			memcpy(newMemory, myObjectData, sizeof(ObjectType)*myAmountOfObjects);
		}
		delete[]myObjectData;

		myObjectData = newMemory;
		myAmountOfObjects = myMaxSize;
	}

	template<typename ObjectType, typename SizeType = int>
	int GrowingArray<ObjectType, SizeType>::Capacity()
	{
		return myMaxSize;
	}

};

namespace CU = CommonUtilities;