#pragma once
#include <cassert>
#define SA_TEMPLATE template<typename Type, int T2 = 32>
#define SA_TYPE StaticArray<Type, T2>

namespace CommonUtilities
{
	SA_TEMPLATE
		class StaticArray
	{
	public:
		StaticArray();
		StaticArray(const StaticArray& aStaticArray);

		~StaticArray();

		StaticArray& operator=(const StaticArray& aStaticArray);

		inline const Type& operator[](const int& aIndex) const;
		inline Type& operator[](const int& aIndex);


		// Utility functions
		inline void Insert(int aIndex, Type& aObject);
		inline void DeleteAll();
		inline void InsertLast(Type& object);

		typedef Type* iterator;
		typedef const Type* const_iterator;
		iterator begin() { return &myData[0]; }
		const_iterator begin() const { return &myData[0]; }
		iterator end() { return &myData[T2]; }
		const_iterator end() const { return &myData[T2]; }

		bool operator==(const StaticArray<Type>& other);

	private:
		Type myData[T2];
		int m_Capacity = 0;
		int m_LastIndex = 0;
	};

	template<typename Type, int T2>
	bool StaticArray<Type, T2>::operator==(const StaticArray<Type>& other)
	{

		for (int i = 0; i < T2; i++)
		{
			if ( myData[i] != other[i] )
				return false;
		}

		return true;
	}

	SA_TEMPLATE
		SA_TYPE::StaticArray()
	{
		m_Capacity = sizeof(myData) / sizeof(int);
	}

	SA_TEMPLATE
		SA_TYPE::~StaticArray()
	{

	}

	SA_TEMPLATE
		SA_TYPE::StaticArray(const StaticArray& aStaticArray)
	{
		operator=(aStaticArray);
	}

	SA_TEMPLATE
		SA_TYPE& SA_TYPE::operator=(const StaticArray& aStaticArray)
	{
		for (int i = 0; i < T2; ++i)
		{
			myData[i] = aStaticArray.myData[i];
		}
		return *this;
	}

	SA_TEMPLATE
		inline const Type& SA_TYPE::operator[](const int& aIndex) const
	{
		assert(aIndex >= 0 && "Index has to be 0 or more.");
		assert(aIndex < T2 && "a index out of bounds!");
		return myData[aIndex];
	}

	SA_TEMPLATE
		inline Type& SA_TYPE::operator[](const int& aIndex)
	{
		assert(aIndex >= 0 && "Index has to be 0 or more.");
		assert(aIndex < T2 && "a index out of bounds!");
		return myData[aIndex];
	}

	SA_TEMPLATE
		inline void SA_TYPE::Insert(int aIndex, Type& aObject)
	{
		assert(aIndex >= 0 && "Index has to be 0 or more.");
		assert(aIndex < T2 && "a index out of bounds!");
		for (int i = T2 - 2; i >= aIndex; --i)
		{
			myData[i + 1] = myData[i];
		}
		myData[aIndex] = aObject;
	}

	SA_TEMPLATE
		inline void SA_TYPE::InsertLast(Type& object)
	{
		assert(m_LastIndex < m_Capacity && "Can't add to last if container is full.");
		myData[m_LastIndex] = object;
		m_LastIndex++;
	}

	SA_TEMPLATE
		inline void SA_TYPE::DeleteAll()
	{
		for (int i = 0; i < T2; ++i)
		{
			delete myData[i];
			myData[i] = nullptr;
		}
	}

};
namespace CU = CommonUtilities;