#pragma once
#include <memory.h>
#include <cassert>
namespace CommonUtilities
{
	template<typename ObjectType, typename SizeType = int>
	class GrowingArray
	{
	public:
		GrowingArray() = default;
		GrowingArray(SizeType capacity);
		GrowingArray(const GrowingArray& other);
		~GrowingArray();

		GrowingArray& operator=(const GrowingArray& other);

		void Init(SizeType capacity);
		void ReInit(SizeType capacity);

		ObjectType& operator[](const SizeType& index);
		const ObjectType& operator[](const SizeType& index) const;

		void Add(const ObjectType& object);

		void Insert(SizeType aIndex, ObjectType& object);
		void DeleteCyclic(ObjectType& object);
		void DeleteCyclicAtIndex(SizeType index);
		void RemoveCyclic(const ObjectType& object);
		void RemoveCyclicAtIndex(SizeType index);

		SizeType Find(const ObjectType& object) const;

		ObjectType& GetLast();
		const ObjectType& GetLast() const;

		static const SizeType FoundNone = static_cast<SizeType>(-1);

		void RemoveAll();
		void DeleteAll();

		SizeType Capacity() const { return m_Capacity; }
		SizeType Size() const { return m_Size; }

		bool Empty() const { return (m_Size <= 0); }
		void Optimize();
		void Reserve(SizeType capacity);
		void Resize(SizeType capacity);

		/*
		 * Range iterator implementation
		 * for(object& obj : array)
		 */
		typedef ObjectType* iterator;
		typedef const ObjectType* const_iterator;
		iterator begin() { return &m_Data[0]; }
		const_iterator begin() const { return &m_Data[0]; }
		iterator end() { return &m_Data[m_Size]; }
		const_iterator end() const { return &m_Data[m_Size]; }

	private:
		ObjectType* m_Data		= nullptr;
		SizeType	m_Capacity	= 0;
		SizeType	m_Size		= 0;
	};


	template<typename ObjectType, typename SizeType /*= int*/>
	SizeType GrowingArray<ObjectType, SizeType>::Find(const ObjectType& object) const
	{
		for (SizeType i = 0; i < m_Size; ++i)
		{
			if (m_Data[i] == object)
				return i;
		}
		return FoundNone;
	}
	
	template<typename ObjectType, typename SizeType>
	GrowingArray<ObjectType, SizeType>::~GrowingArray()
	{
		m_Capacity = 0;
		m_Size = 0;
		delete[] m_Data;
		m_Data = nullptr;
	};

	template<typename ObjectType, typename SizeType>
	GrowingArray<ObjectType, SizeType>::GrowingArray(SizeType capacity)
	{
		Init(capacity);
	};

	template<typename ObjectType, typename SizeType>
	GrowingArray<ObjectType, SizeType>::GrowingArray(const GrowingArray& other)
	{
		*this = other;
	};

	template<typename ObjectType, typename SizeType>
	void GrowingArray<ObjectType, SizeType>::Init(SizeType capacity)
	{
		m_Size = 0;
		m_Capacity = capacity;
		m_Data = new ObjectType[capacity];
	};

	template<typename ObjectType, typename SizeType>
	void GrowingArray<ObjectType, SizeType>::ReInit(SizeType capacity)
	{
		delete[] m_Data;
		m_Data = nullptr;
		Init(capacity);
	};

	template<typename ObjectType, typename SizeType>
	GrowingArray<ObjectType, SizeType>& GrowingArray<ObjectType, SizeType>::operator=(const GrowingArray& other)
	{
		delete[] m_Data;
		m_Data = nullptr;

		m_Capacity = other.m_Capacity;
		m_Size = other.m_Size;
		m_Data = new ObjectType[m_Capacity];
		memcpy(m_Data, other.m_Data, sizeof(ObjectType)* other.m_Size);

		return *this;
	};

	template<typename ObjectType, typename SizeType>
	ObjectType& GrowingArray<ObjectType, SizeType>::operator[](const SizeType& index)
	{
		return m_Data[index];
	}

	template<typename ObjectType, typename SizeType>
	const ObjectType& GrowingArray<ObjectType, SizeType>::operator[](const SizeType& index) const
	{
		return m_Data[index];
	};
	
	template<typename ObjectType, typename SizeType>
	void GrowingArray<ObjectType, SizeType>::Add(const ObjectType& object)
	{
		if (m_Size >= m_Capacity)
			Resize(m_Capacity * 2);
		
		m_Data[m_Size++] = object;
	};

	template<typename ObjectType, typename SizeType>
	void GrowingArray<ObjectType, SizeType>::Insert(SizeType index, ObjectType& object)
	{
		m_Data[index] = object;
	};

	template<typename ObjectType, typename SizeType>
	void GrowingArray<ObjectType, SizeType>::DeleteCyclic(ObjectType& object)
	{
		const SizeType index = Find(object);
		delete m_Data[index];
		m_Data[index] = GetLast();
		--m_Size;		
	};

	template<typename ObjectType, typename SizeType>
	void GrowingArray<ObjectType, SizeType>::DeleteCyclicAtIndex(SizeType index)
	{
		delete m_Data[index];
		m_Data[index] = GetLast();
		--m_Size;
	};

	template<typename ObjectType, typename SizeType>
	void GrowingArray<ObjectType, SizeType>::RemoveCyclic(const ObjectType& object)
	{
		SizeType index = Find(object);
		m_Data[index] = GetLast();
		--m_Size;
	};

	template<typename ObjectType, typename SizeType>
	void GrowingArray<ObjectType, SizeType>::RemoveCyclicAtIndex(SizeType index)
	{
		m_Data[index] = GetLast();
		--m_Size;
	};


	template<typename ObjectType, typename SizeType>
	ObjectType& GrowingArray<ObjectType, SizeType>::GetLast()
	{
		return m_Data[m_Size - 1];
	};

	template<typename ObjectType, typename SizeType>
	const ObjectType& GrowingArray<ObjectType, SizeType>::GetLast() const
	{
		return m_Data[m_Size - 1];
	};

	template<typename ObjectType, typename SizeType>
	void GrowingArray<ObjectType, SizeType>::RemoveAll()
	{
		m_Size = 0;
	};

	template<typename ObjectType, typename SizeType>
	void GrowingArray<ObjectType, SizeType>::DeleteAll()
	{
		for(SizeType i = 0; i < m_Size; ++i)
		{
			delete m_Data[i];
			m_Data[i] = nullptr;
		}

		m_Size = 0;
	};

	template<typename ObjectType, typename SizeType>
	void GrowingArray<ObjectType, SizeType>::Optimize()
	{
		if (m_Capacity == m_Size)
			return;

		Resize(m_Size);
	};

	template<typename ObjectType, typename SizeType>
	void GrowingArray<ObjectType, SizeType>::Resize(SizeType capacity)
	{
		m_Capacity = capacity;
		ObjectType* new_memory = new ObjectType[m_Capacity];
		memcpy(new_memory, m_Data, sizeof(ObjectType) * m_Size);
		delete[] m_Data;
		m_Data = new_memory;
	};

	template<typename ObjectType, typename SizeType>
	void GrowingArray<ObjectType, SizeType>::Reserve(SizeType capacity)
	{
		m_Capacity = capacity;
		m_Data = new ObjectType[m_Capacity];
	}

};

namespace CU = CommonUtilities;