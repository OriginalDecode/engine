#pragma once
#include "Hash.h"
#include "..\GrowingArray.h"
#define MAP typename Key, typename Value, int Size = 16
#define MAP_TEMPLATE Key, Value, Size
namespace CommonUtilities
{
	template<MAP>
	class Map
	{
		
	public:
		Map();
		~Map();


		void Insert(const Key &aKey, const Value &aValue);
		Value& Get(const Key &aKey);
		void Delete(const Key &aKey);
		bool KeyExists(const Key &aKey);
		Value& operator[](const Key &aKey); 
		const int& GetBucketSize();
	private:
		struct Keypair
		{
			Key key;
			Value value;
		};
		
		const unsigned int Hash(const Key &aKey);
		CU::GrowingArray<CU::GrowingArray<Keypair>> myBuckets;
		int myBucketSize;
		bool ValueExists(const Value &aValue);

	};

	template<MAP>
	const int& CommonUtilities::Map<>::GetBucketSize()
	{
		return myBucketSize;
	}

	template<MAP>
	Map<MAP_TEMPLATE>::Map()
	{
		myBucketSize = Size;
		myBuckets.Reserve(myBucketSize);
		for (int i = 0; i < myBuckets.Size(); ++i)
		{
			myBuckets[i].Init(3);
		}
	}



	template<MAP>
	Map<MAP_TEMPLATE>::~Map()
	{
	}

	template<MAP>
	void Map<MAP_TEMPLATE>::Insert(const Key &aKey, const Value &aValue)
	{
		
		assert(ValueExists(aValue) == false && "Value already exists in the array");
		
		unsigned int index = cl::Hash(aKey);

		if (KeyExists(aKey) == true)
		{
			for (int i = 0; i < myBuckets[index].Size(); i++)
			{
				if (myBuckets[index][i].key == aKey)
				{
					myBuckets[index][i].value = aValue;
					return;
				}
			}
		}
		Keypair pair;
		pair.key = aKey;
		pair.value = aValue;
		myBuckets[index].Add(pair);
	}

	template<MAP>
	Value& Map<MAP_TEMPLATE>::Get(const Key &aKey)
	{
		assert(KeyExists(aKey) == true && "[Map] : Key does not exist.");


		unsigned int index = cl::Hash(aKey);

		for (int i = 0; i < myBuckets[index].Size(); ++i)
		{
			if (myBuckets[index][i].key == aKey)
				return myBuckets[index][i].value;
		}

		assert(false && "[Map] : Can't find anything in the map.");

		return myBuckets[0][0].value;
	}

	template<MAP>
	Value& Map<MAP_TEMPLATE>::operator[](const Key &aKey)
	{
		if (KeyExists(aKey) == true)
			return Get(aKey);

		Keypair pair;
		pair.value = 0;

		Insert(aKey, pair.value);
		return Get(aKey);
	}

	template<MAP>
	bool Map<MAP_TEMPLATE>::KeyExists(const Key &aKey)
	{
		unsigned int index = cl::Hash(aKey);

		for (int i = 0; i < myBuckets[index].Size(); ++i)
		{
			if (myBuckets[index][i].key == aKey)
				return true;
		}

		return false;

	}

	template<MAP>
	void Map<MAP_TEMPLATE>::Delete(const Key &aKey)
	{
		assert(KeyExists(aKey) != false && "[Map] : Key does not exist.");

		unsigned int index = cl::Hash(aKey);

		if (KeyExists(aKey) == true)
		{
			for (int i = 0; i < myBuckets[index].Size(); i++)
			{
				if (myBuckets[index][i].key == aKey)
				{
					myBuckets[index].RemoveCyclicAtIndex(i);
					return;
				}
			}
		}
	}

	template<MAP>
	const unsigned int Map<MAP_TEMPLATE>::Hash(const Key &aKey)
	{
		int size = strlen(aKey.c_str());
		unsigned int result;
		MurmurHash3_x86_32(aKey.c_str(), size, 0, &result);

		return result % myBucketSize;
	}

	template<MAP>
	bool Map<MAP_TEMPLATE>::ValueExists(const Value &aValue)
	{
		for (int i = 0; i < myBuckets.Size(); ++i)
		{
			for (int j = 0; j < myBuckets[i].Size(); ++j)
			{
				if (myBuckets[i][j].value == aValue)
					return true;
			}
		}

		return false;
	}
}

#undef MAP
#undef MAP_TEMPLATE